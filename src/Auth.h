#pragma once
#include <optional>
#include <string>
#include <vector>
#include "Database.h"
#include "models.h"

class Auth {
public:
    explicit Auth(Database& db) : db_(db) {}
    void createUser(const std::string& username, const std::string& password, Role role);
    std::optional<User> login(const std::string& username, const std::string& password);
    bool hasUsers();
    std::vector<User> getAllUsers(const User& actor);

private:
    Database& db_;
};