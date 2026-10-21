#pragma once
#include <vector>
#include "Database.h"
#include "models.h"

class Billing {
public:
    explicit Billing(Database& db) : db_(db) {}

    // any logged-in user (admin or cashier) can bill
    Bill generateBill(const User& actor, const std::vector<CartItem>& cart);
    Bill getBill(int billId);
    std::vector<Bill> getRecentBills(int limit = 50);

    // admin only
    std::vector<DailySales> dailySales(const User& actor, int limit = 7);
    std::vector<TopProduct> topProducts(const User& actor, int limit = 5);

private:
    Database& db_;
};