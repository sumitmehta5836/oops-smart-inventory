#include "Billing.h"
#include <stdexcept>
#include <string>

static sqlite3_stmt* prep(Database& db, const char* sql) {
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db.handle(), sql, -1, &st, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db.handle()));
    return st;
}

// run a statement that returns no rows, then clean it up
static void runStep(Database& db, sqlite3_stmt* st) {
    int rc = sqlite3_step(st);
    std::string msg = sqlite3_errmsg(db.handle());
    sqlite3_finalize(st);
    if (rc != SQLITE_DONE) throw std::runtime_error(msg);
}

static std::string textAt(sqlite3_stmt* st, int col) {
    const unsigned char* t = sqlite3_column_text(st, col);
    return t ? reinterpret_cast<const char*>(t) : "";
}

static void requireAdmin(const User& actor) {
    if (actor.role != Role::Admin)
        throw std::runtime_error("Access denied: admin only");
}

Bill Billing::generateBill(const User& actor, const std::vector<CartItem>& cart) {
    if (cart.empty()) throw std::invalid_argument("Cart is empty");
    for (const CartItem& c : cart)
        if (c.quantity <= 0) throw std::invalid_argument("Quantity must be positive");

    int billId = 0;
    db_.execute("BEGIN TRANSACTION;");
    try {
        // 1. create the bill with total 0 (we fill it in at the end)
        sqlite3_stmt* st = prep(db_, "INSERT INTO bills (cashier_id, total) VALUES (?, 0);");
        sqlite3_bind_int(st, 1, actor.id);
        runStep(db_, st);
        billId = static_cast<int>(sqlite3_last_insert_rowid(db_.handle()));

        double total = 0;
        for (const CartItem& c : cart) {
            // 2. read the product's current name, price, stock
            st = prep(db_, "SELECT name, price, quantity FROM products WHERE id = ?;");
            sqlite3_bind_int(st, 1, c.productId);
            if (sqlite3_step(st) != SQLITE_ROW) {
                sqlite3_finalize(st);
                throw std::runtime_error("Product " + std::to_string(c.productId) + " not found");
            }
            std::string name = textAt(st, 0);
            double price     = sqlite3_column_double(st, 1);
            int stock        = sqlite3_column_int(st, 2);
            sqlite3_finalize(st);

            // 3. enough stock?
            if (stock < c.quantity)
                throw std::runtime_error("Not enough stock for " + name +
                                         " (available: " + std::to_string(stock) + ")");

            // 4. reduce the stock
            st = prep(db_, "UPDATE products SET quantity = quantity - ? WHERE id = ?;");
            sqlite3_bind_int(st, 1, c.quantity);
            sqlite3_bind_int(st, 2, c.productId);
            runStep(db_, st);

            // 5. save the snapshot line
            double line = price * c.quantity;
            st = prep(db_,
                "INSERT INTO bill_items (bill_id, product_id, product_name, unit_price, quantity, line_total) "
                "VALUES (?, ?, ?, ?, ?, ?);");
            sqlite3_bind_int(st, 1, billId);
            sqlite3_bind_int(st, 2, c.productId);
            sqlite3_bind_text(st, 3, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(st, 4, price);
            sqlite3_bind_int(st, 5, c.quantity);
            sqlite3_bind_double(st, 6, line);
            runStep(db_, st);

            total += line;
        }

        // 6. save the final total
        st = prep(db_, "UPDATE bills SET total = ? WHERE id = ?;");
        sqlite3_bind_double(st, 1, total);
        sqlite3_bind_int(st, 2, billId);
        runStep(db_, st);

        db_.execute("COMMIT;");
    } catch (...) {
        db_.execute("ROLLBACK;");   // undo EVERYTHING since BEGIN
        throw;                       // pass the error up to the caller
    }
    return getBill(billId);
}

Bill Billing::getBill(int billId) {
    Bill bill{};

    sqlite3_stmt* st = prep(db_,
        "SELECT b.id, b.created_at, u.username, b.total "
        "FROM bills b JOIN users u ON u.id = b.cashier_id WHERE b.id = ?;");
    sqlite3_bind_int(st, 1, billId);
    if (sqlite3_step(st) != SQLITE_ROW) {
        sqlite3_finalize(st);
        throw std::runtime_error("Bill not found");
    }
    bill.id        = sqlite3_column_int(st, 0);
    bill.createdAt = textAt(st, 1);
    bill.cashier   = textAt(st, 2);
    bill.total     = sqlite3_column_double(st, 3);
    sqlite3_finalize(st);

    st = prep(db_,
        "SELECT product_id, product_name, unit_price, quantity, line_total "
        "FROM bill_items WHERE bill_id = ? ORDER BY id;");
    sqlite3_bind_int(st, 1, billId);
    while (sqlite3_step(st) == SQLITE_ROW) {
        BillItem it;
        it.productId = sqlite3_column_int(st, 0);
        it.name      = textAt(st, 1);
        it.unitPrice = sqlite3_column_double(st, 2);
        it.quantity  = sqlite3_column_int(st, 3);
        it.lineTotal = sqlite3_column_double(st, 4);
        bill.items.push_back(it);
    }
    sqlite3_finalize(st);
    return bill;
}

std::vector<Bill> Billing::getRecentBills(int limit) {
    std::vector<Bill> out;
    sqlite3_stmt* st = prep(db_,
        "SELECT b.id, b.created_at, u.username, b.total "
        "FROM bills b JOIN users u ON u.id = b.cashier_id "
        "ORDER BY b.id DESC LIMIT ?;");
    sqlite3_bind_int(st, 1, limit);
    while (sqlite3_step(st) == SQLITE_ROW) {
        Bill b;
        b.id        = sqlite3_column_int(st, 0);
        b.createdAt = textAt(st, 1);
        b.cashier   = textAt(st, 2);
        b.total     = sqlite3_column_double(st, 3);
        out.push_back(b);
    }
    sqlite3_finalize(st);

    for (Bill& b : out) {
        sqlite3_stmt* itemSt = prep(db_,
            "SELECT product_id, product_name, unit_price, quantity, line_total "
            "FROM bill_items WHERE bill_id = ? ORDER BY id;");
        sqlite3_bind_int(itemSt, 1, b.id);
        while (sqlite3_step(itemSt) == SQLITE_ROW) {
            BillItem it;
            it.productId = sqlite3_column_int(itemSt, 0);
            it.name      = textAt(itemSt, 1);
            it.unitPrice = sqlite3_column_double(itemSt, 2);
            it.quantity  = sqlite3_column_int(itemSt, 3);
            it.lineTotal = sqlite3_column_double(itemSt, 4);
            b.items.push_back(it);
        }
        sqlite3_finalize(itemSt);
    }
    return out;
}

std::vector<DailySales> Billing::dailySales(const User& actor, int limit) {
    requireAdmin(actor);
    std::vector<DailySales> out;

    sqlite3_stmt* st = prep(db_,
        "SELECT date(created_at, 'localtime') AS day, COUNT(*), SUM(total) "
        "FROM bills GROUP BY day ORDER BY day DESC LIMIT ?;");
    sqlite3_bind_int(st, 1, limit);
    while (sqlite3_step(st) == SQLITE_ROW) {
        DailySales d;
        d.date      = textAt(st, 0);
        d.billCount = sqlite3_column_int(st, 1);
        d.revenue   = sqlite3_column_double(st, 2);
        out.push_back(d);
    }
    sqlite3_finalize(st);
    return out;
}

std::vector<TopProduct> Billing::topProducts(const User& actor, int limit) {
    requireAdmin(actor);
    std::vector<TopProduct> out;

    sqlite3_stmt* st = prep(db_,
        "SELECT MAX(product_name), SUM(quantity), SUM(line_total) "
        "FROM bill_items GROUP BY product_id "
        "ORDER BY SUM(quantity) DESC LIMIT ?;");
    sqlite3_bind_int(st, 1, limit);
    while (sqlite3_step(st) == SQLITE_ROW) {
        TopProduct t;
        t.name      = textAt(st, 0);
        t.unitsSold = sqlite3_column_int(st, 1);
        t.revenue   = sqlite3_column_double(st, 2);
        out.push_back(t);
    }
    sqlite3_finalize(st);
    return out;
}
