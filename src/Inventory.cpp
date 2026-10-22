#include "Inventory.h"
#include <stdexcept>

static void requireAdmin(const User& actor) {
    if (actor.role != Role::Admin)
        throw std::runtime_error("Access denied: admin only");
}

sqlite3_stmt* Inventory::prepare(const std::string& sql) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.handle(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db_.handle()));
    return stmt;
}

int Inventory::runWrite(sqlite3_stmt* stmt) {
    int rc = sqlite3_step(stmt);
    std::string msg = sqlite3_errmsg(db_.handle());
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) throw std::runtime_error(msg);
    return sqlite3_changes(db_.handle());
}

std::vector<Product> Inventory::readAll(sqlite3_stmt* stmt) {
    std::vector<Product> list;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Product p;
        p.id       = sqlite3_column_int(stmt, 0);
        p.name     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        p.price    = sqlite3_column_double(stmt, 2);
        p.quantity = sqlite3_column_int(stmt, 3);
        list.push_back(p);
    }
    sqlite3_finalize(stmt);
    return list;
}

// OOP CONCEPT: ENCAPSULATION & ABSTRACTION
// 1. Encapsulation: Validates input constraints (non-empty name, non-negative price/quantity)
//    and role permissions to ensure class data integrity.
// 2. Abstraction: Callers invoke a single method without dealing with SQLite statement preparation or parameter binding.
void Inventory::addProduct(const User& actor, const std::string& name,
                           double price, int quantity) {
    requireAdmin(actor);
    if (name.empty())  throw std::invalid_argument("Name cannot be empty");
    if (price < 0)     throw std::invalid_argument("Price cannot be negative");
    if (quantity < 0)  throw std::invalid_argument("Quantity cannot be negative");

    sqlite3_stmt* st = prepare("INSERT INTO products (name, price, quantity) VALUES (?, ?, ?);");
    sqlite3_bind_text(st, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(st, 2, price);
    sqlite3_bind_int(st, 3, quantity);
    runWrite(st);
}

void Inventory::updateProduct(const User& actor, int id,
                              const std::string& name, double price) {
    requireAdmin(actor);
    if (name.empty() || price < 0) throw std::invalid_argument("Invalid name or price");

    sqlite3_stmt* st = prepare("UPDATE products SET name = ?, price = ? WHERE id = ?;");
    sqlite3_bind_text(st, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(st, 2, price);
    sqlite3_bind_int(st, 3, id);
    if (runWrite(st) == 0) throw std::runtime_error("Product not found");
}

void Inventory::restock(const User& actor, int id, int amount) {
    requireAdmin(actor);
    if (amount <= 0) throw std::invalid_argument("Restock amount must be positive");

    sqlite3_stmt* st = prepare("UPDATE products SET quantity = quantity + ? WHERE id = ?;");
    sqlite3_bind_int(st, 1, amount);
    sqlite3_bind_int(st, 2, id);
    if (runWrite(st) == 0) throw std::runtime_error("Product not found");
}

void Inventory::removeProduct(const User& actor, int id) {
    requireAdmin(actor);
    sqlite3_stmt* st = prepare("DELETE FROM products WHERE id = ?;");
    sqlite3_bind_int(st, 1, id);
    if (runWrite(st) == 0) throw std::runtime_error("Product not found");
}

std::vector<Product> Inventory::getAll() {
    return readAll(prepare("SELECT id, name, price, quantity FROM products ORDER BY name;"));
}

// OOP CONCEPT: ABSTRACTION
// Hides SQL query formatting, wildcard pattern generation, and row-to-object deserialization.
std::vector<Product> Inventory::search(const std::string& keyword) {
    sqlite3_stmt* st = prepare(
        "SELECT id, name, price, quantity FROM products WHERE name LIKE ? ORDER BY name;");
    std::string pattern = "%" + keyword + "%";
    sqlite3_bind_text(st, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
    return readAll(st);
}

std::vector<Product> Inventory::getLowStock(int threshold) {
    sqlite3_stmt* st = prepare(
        "SELECT id, name, price, quantity FROM products WHERE quantity < ? ORDER BY quantity;");
    sqlite3_bind_int(st, 1, threshold);
    return readAll(st);
}