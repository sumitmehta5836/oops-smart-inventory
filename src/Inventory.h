#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "models.h"

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
    Database& db_;

    sqlite3_stmt* prepare(const std::string& sql);
    int runWrite(sqlite3_stmt* stmt);
    std::vector<Product> readAll(sqlite3_stmt* stmt);
};