#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include "Billing.h"
#include "Inventory.h"
#include "models.h"

class ReportsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ReportsWidget(Billing& billing, Inventory& inventory, const User& currentUser, QWidget* parent = nullptr);
    void refreshReports();

private:
    Billing& billing_;
    Inventory& inventory_;
    User currentUser_;

    QLabel* totalRevenueCardLbl_;
    QLabel* totalBillsCardLbl_;
    QLabel* lowStockCountCardLbl_;

    QTableWidget* dailySalesTable_;
    QTableWidget* topProductsTable_;

    void setupUi();
};

