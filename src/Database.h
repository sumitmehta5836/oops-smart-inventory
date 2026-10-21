#pragma once
#include <string>
#include <sqlite3.h>

class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    // copying would make two objects close the same connection
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void execute(const std::string& sql);
    void initSchema();
    sqlite3* handle() const { return db_; }

private:
    sqlite3* db_ = nullptr;
};