#include <iostream>
#include <iomanip>
#include "Database.h"
#include "Auth.h"
#include "Inventory.h"
#include "Billing.h"

void printProducts(const std::vector<Product>& list) {
    for (const Product& p : list)
        std::cout << p.id << " | " << p.name << " | Rs " << p.price
                  << " | qty " << p.quantity << "\n";
    std::cout << "---\n";
}

void printBill(const Bill& b) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Bill #" << b.id << " | " << b.createdAt
              << " | cashier: " << b.cashier << " ===\n";
    for (const BillItem& it : b.items)
        std::cout << it.name << "  x" << it.quantity << "  @ " << it.unitPrice
                  << "  = " << it.lineTotal << "\n";
    std::cout << "TOTAL: Rs " << b.total << "\n\n";
}

int main() {
    try {
        Database db("inventory.db");
        db.initSchema();
        Auth auth(db);
        Inventory inv(db);
        Billing billing(db);

        if (!auth.hasUsers()) {
            auth.createUser("admin", "admin123", Role::Admin);
            auth.createUser("cashier1", "cash123", Role::Cashier);
        }
        User admin   = auth.login("admin", "admin123").value();
        User cashier = auth.login("cashier1", "cash123").value();

        inv.addProduct(admin, "Pen", 10.5, 100);      // id 1
        inv.addProduct(admin, "Pencil", 5, 3);        // id 2
        inv.addProduct(admin, "Notebook", 45, 2);     // id 3

        // 1. a normal bill
        Bill b = billing.generateBill(cashier, { {1, 2}, {3, 1} });   // 2 Pen + 1 Notebook
        printBill(b);
        printProducts(inv.getAll());

        // 2. a bill that must FAIL halfway: Pen is fine, Pencil is short
        try {
            billing.generateBill(cashier, { {1, 5}, {2, 10} });
        } catch (const std::exception& e) {
            std::cout << "Bill failed: " << e.what() << "\n";
        }
        std::cout << "Stock after failed bill (Pen must still be 98):\n";
        printProducts(inv.getAll());

        // 3. one more successful bill, so the report has more data
        printBill(billing.generateBill(cashier, { {1, 10}, {2, 3} }));

        // 4. admin reports
        std::cout << "--- Daily sales ---\n";
        for (const DailySales& d : billing.dailySales(admin))
            std::cout << d.date << " | bills: " << d.billCount
                      << " | revenue: Rs " << d.revenue << "\n";

        std::cout << "--- Top products ---\n";
        for (const TopProduct& t : billing.topProducts(admin))
            std::cout << t.name << " | units: " << t.unitsSold
                      << " | revenue: Rs " << t.revenue << "\n";

        try {
            billing.dailySales(cashier);   // should be denied
        } catch (const std::exception& e) {
            std::cout << "Cashier blocked: " << e.what() << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}