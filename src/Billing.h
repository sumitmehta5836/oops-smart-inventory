#pragma once
#include <vector>
#include "Database.h"
#include "models.h"

// OOP CONCEPT: ENCAPSULATION & ABSTRACTION
// 1. Encapsulation: The Billing class bundles checkout state, financial computations,
//    and sales metrics into a unified module, protecting the database connection db_.
// 2. Abstraction: The complex workflow of transactional checkout (validating stock, deducting
//    quantities, recording line items, issuing bills, handling rollbacks) is hidden behind simple method calls.
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
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // The Database reference db_ is kept private to preserve transactional integrity.
    Database& db_;
};
