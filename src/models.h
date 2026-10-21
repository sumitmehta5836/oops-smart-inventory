#pragma once
#include <string>
#include <vector>

enum class Role { Admin, Cashier };

struct Product { int id; std::string name; double price; int quantity; };
struct User    { int id; std::string username; Role role; };

// what the cashier puts in the cart
struct CartItem { int productId; int quantity; };

// a saved line on a bill (a snapshot, not linked to the live product)
struct BillItem {
    int productId;
    std::string name;
    double unitPrice;
    int quantity;
    double lineTotal;
};

struct Bill {
    int id;
    std::string createdAt;
    std::string cashier;
    double total;
    std::vector<BillItem> items;
};

struct DailySales {
    std::string date;
    int billCount;
    double revenue;
};

struct TopProduct {
    std::string name;
    int unitsSold;
    double revenue;
};

inline std::string roleToString(Role r) { return r == Role::Admin ? "admin" : "cashier"; }
inline Role stringToRole(const std::string& s) { return s == "admin" ? Role::Admin : Role::Cashier; }