#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <map>
#include "Billing.h"
#include "Inventory.h"
#include "models.h"

class BillingWidget : public QWidget {
    Q_OBJECT

public:
    explicit BillingWidget(Billing& billing, Inventory& inventory, const User& currentUser, QWidget* parent = nullptr);
    void refreshCatalog();

signals:
    void saleCompleted();

private slots:
    void handleSearch();
    void addToCartClicked();
    void removeFromCartClicked();
    void clearCart();
    void completeSale();
    void updateCartTotals();

private:
    Billing& billing_;
    Inventory& inventory_;
    User currentUser_;

    // Left Panel (Catalog)
    QLineEdit* searchInput_;
    QTableWidget* catalogTable_;
    QSpinBox* addQuantitySpin_;
    QPushButton* addToCartBtn_;

    // Right Panel (Cart)
    QTableWidget* cartTable_;
    QLabel* itemCountLabel_;
    QLabel* totalAmountLabel_;
    QPushButton* clearCartBtn_;
    QPushButton* checkoutBtn_;

    std::vector<Product> catalogList_;
    // Map productId -> { Product, quantity }
    struct CartEntry {
        Product product;
        int quantity;
    };
    std::map<int, CartEntry> cart_;

    void setupUi();
    void populateCatalog(const std::vector<Product>& list);
    void refreshCartTable();
};

