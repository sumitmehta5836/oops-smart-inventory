#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "models.h"

// OOP CONCEPT: ENCAPSULATION & ABSTRACTION
// 1. Encapsulation: The Inventory class manages catalog data, coupling it with access rules
//    (role checks and price/quantity validation) to maintain state consistency.
// 2. Abstraction: The public methods provide high-level product catalog operations, hiding
//    the complexity of SQL statement compilation, parameter binding, and cursor traversal.
class Inventory {
public:
    explicit Inventory(Database& db) : db_(db) {}

    // admin only
    void addProduct(const User& actor, const std::string& name, double price, int quantity);
    void updateProduct(const User& actor, int id, const std::string& name, double price);
    void restock(const User& actor, int id, int amount);
    void removeProduct(const User& actor, int id);

    // any logged-in user
    std::vector<Product> getAll();
    std::vector<Product> search(const std::string& keyword);
    std::vector<Product> getLowStock(int threshold = 5);

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING & INTERNAL HELPER METHODS)
    // Low-level database helpers and connection references are private to hide SQLite mechanics.
    Database& db_;

    sqlite3_stmt* prepare(const std::string& sql);
    int runWrite(sqlite3_stmt* stmt);
    std::vector<Product> readAll(sqlite3_stmt* stmt);
};