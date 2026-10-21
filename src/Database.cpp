#include "Database.h"
#include <stdexcept>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw std::runtime_error("Cannot open DB: " + msg);
    }
}

Database::~Database() {
    if (db_) sqlite3_close(db_);
}

void Database::execute(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err;
        sqlite3_free(err);
        throw std::runtime_error("SQL error: " + msg);
    }
}

void Database::initSchema() {
    execute(R"(
        CREATE TABLE IF NOT EXISTS users (
            id            INTEGER PRIMARY KEY AUTOINCREMENT,
            username      TEXT UNIQUE NOT NULL,
            salt          TEXT NOT NULL,
            password_hash TEXT NOT NULL,
            role          TEXT NOT NULL CHECK (role IN ('admin','cashier'))
        );
        CREATE TABLE IF NOT EXISTS products (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            name     TEXT    NOT NULL,
            price    REAL    NOT NULL CHECK (price >= 0),
            quantity INTEGER NOT NULL DEFAULT 0 CHECK (quantity >= 0)
        );
        CREATE TABLE IF NOT EXISTS bills (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            cashier_id INTEGER NOT NULL,
            created_at TEXT    NOT NULL DEFAULT CURRENT_TIMESTAMP,
            total      REAL    NOT NULL DEFAULT 0
        );
        CREATE TABLE IF NOT EXISTS bill_items (
            id           INTEGER PRIMARY KEY AUTOINCREMENT,
            bill_id      INTEGER NOT NULL,
            product_id   INTEGER NOT NULL,
            product_name TEXT    NOT NULL,
            unit_price   REAL    NOT NULL,
            quantity     INTEGER NOT NULL,
            line_total   REAL    NOT NULL
        );
    )");
}