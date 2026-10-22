#pragma once
#include <optional>
#include <string>
#include <vector>
#include "Database.h"
#include "models.h"

// OOP CONCEPT: ENCAPSULATION & ABSTRACTION
// 1. Encapsulation: The Auth class encapsulates user credential verification and role checking.
//    Internal state (db_) is protected, and interactions occur exclusively through designated methods.
// 2. Abstraction: High-level operations (createUser, login, getAllUsers) are exposed while the underlying
//    cryptographic salt generation, SHA-256 password hashing, and SQL queries are completely hidden.
class Auth {
public:
    explicit Auth(Database& db) : db_(db) {}
    void createUser(const std::string& username, const std::string& password, Role role);
    std::optional<User> login(const std::string& username, const std::string& password);
    bool hasUsers();
    std::vector<User> getAllUsers(const User& actor);

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // The Database reference db_ is private, ensuring external code cannot bypass Auth logic.
    Database& db_;
};