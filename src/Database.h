#pragma once
#include <string>
#include <sqlite3.h>

// OOP CONCEPT: ENCAPSULATION & ABSTRACTION
// 1. Encapsulation: The Database class bundles the raw SQLite connection handle (state)
//    and the functions managing connection lifecycle (behavior) into a single cohesive unit.
// 2. Abstraction: It exposes a clean, high-level interface (execute, initSchema) to the application,
//    hiding the complex C-style SQLite library details, connection setup, and manual memory cleanup.
class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    // OOP CONCEPT: COMPILE-TIME POLYMORPHISM (OPERATOR OVERLOADING)
    // Deleting the copy assignment operator (operator=) and copy constructor prevents
    // shallow copies of the database handle, enforcing strict RAII resource ownership.
    // copying would make two objects close the same connection
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void execute(const std::string& sql);
    void initSchema();
    sqlite3* handle() const { return db_; }

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // db_ is private to prevent unauthorized direct modification of the raw connection pointer.
    sqlite3* db_ = nullptr;
};