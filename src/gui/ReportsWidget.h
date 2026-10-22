#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include "Billing.h"
#include "Inventory.h"
#include "models.h"

// OOP CONCEPT: INHERITANCE
// ReportsWidget inherits from QWidget to provide an executive analytics dashboard.
// Base Class: QWidget | Derived Class: ReportsWidget
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates KPI summary cards, sales history tables, and analytics data fetching.
class ReportsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ReportsWidget(Billing& billing, Inventory& inventory, const User& currentUser, QWidget* parent = nullptr);
    void refreshReports();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
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

