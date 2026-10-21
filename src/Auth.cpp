#include "Auth.h"
#include "picosha2.h"
#include <random>
#include <stdexcept>

static std::string makeSalt() {
    const char* hex = "0123456789abcdef";
    std::random_device rd;
    std::string salt;
    for (int i = 0; i < 16; i++) salt += hex[rd() % 16];
    return salt;
}

static std::string hashPassword(const std::string& salt, const std::string& password) {
    return picosha2::hash256_hex_string(salt + password);
}

void Auth::createUser(const std::string& username, const std::string& password, Role role) {
    std::string salt = makeSalt();
    std::string hash = hashPassword(salt, password);
    std::string roleStr = roleToString(role);

    const char* sql =
        "INSERT INTO users (username, salt, password_hash, role) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db_.handle()));

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, salt.c_str(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hash.c_str(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, roleStr.c_str(),  -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE)
        throw std::runtime_error("Could not create user (username taken?)");
}

std::optional<User> Auth::login(const std::string& username, const std::string& password) {
    const char* sql =
        "SELECT id, salt, password_hash, role FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db_.handle()));
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    std::optional<User> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string salt   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string stored = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string role   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        if (hashPassword(salt, password) == stored)
            result = User{ sqlite3_column_int(stmt, 0), username, stringToRole(role) };
    }
    sqlite3_finalize(stmt);
    return result;
}

bool Auth::hasUsers() {
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db_.handle(), "SELECT COUNT(*) FROM users;", -1, &stmt, nullptr);
    sqlite3_step(stmt);
    bool any = sqlite3_column_int(stmt, 0) > 0;
    sqlite3_finalize(stmt);
    return any;
}

std::vector<User> Auth::getAllUsers(const User& actor) {
    if (actor.role != Role::Admin)
        throw std::runtime_error("Access denied: admin only");

    std::vector<User> users;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, username, role FROM users ORDER BY id ASC;";
    if (sqlite3_prepare_v2(db_.handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db_.handle()));

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        users.push_back(User{ id, username, stringToRole(roleStr) });
    }
    sqlite3_finalize(stmt);
    return users;
}