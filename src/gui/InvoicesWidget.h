#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Billing.h"
#include "models.h"

// OOP CONCEPT: INHERITANCE
// InvoicesWidget inherits from QWidget to display transaction history.
// Base Class: QWidget | Derived Class: InvoicesWidget
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates the invoice log table, filter logic, and Billing service reference.
class InvoicesWidget : public QWidget {
    Q_OBJECT

public:
    explicit InvoicesWidget(Billing& billing, const User& currentUser, QWidget* parent = nullptr);
    void refreshInvoices();

private slots:
    void handleSearch();
    void viewSelectedInvoice();
    void onSelectionChanged();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    Billing& billing_;
    User currentUser_;

    QLineEdit* searchInput_;
    QTableWidget* table_;
    QPushButton* viewBtn_;

    std::vector<Bill> billsList_;

    void setupUi();
    void populateTable(const std::vector<Bill>& list);
};

