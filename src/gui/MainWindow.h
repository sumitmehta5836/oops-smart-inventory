#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "Database.h"
#include "Auth.h"
#include "Inventory.h"
#include "Billing.h"
#include "models.h"

class InventoryWidget;
class BillingWidget;
class InvoicesWidget;
class ReportsWidget;
class UsersWidget;

// OOP CONCEPT: INHERITANCE
// MainWindow inherits from Qt's QMainWindow base class.
// Base Class: QMainWindow (provides standard desktop window features like status bars, central widgets, and event dispatch)
// Derived Class: MainWindow (extends the base class with customized role-based views, theme toggles, and inventory tab management)
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates references to the core backend services (Database, Auth, Inventory, Billing),
// current user credentials, and UI sub-widgets, preventing external code from tampering with window internals.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Database& db, Auth& auth, Inventory& inventory, Billing& billing, const User& currentUser, QWidget* parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void switchTab(int index);
    void onInventoryChanged();
    void onSaleCompleted();
    void toggleTheme();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // Internal state variables and child widget pointers are hidden from external access.
    Database& db_;
    Auth& auth_;
    Inventory& inventory_;
    Billing& billing_;
    User currentUser_;

    bool isDarkMode_ = true;
    QPushButton* themeToggleBtn_ = nullptr;

    QStackedWidget* stackedWidget_;
    std::vector<QPushButton*> navButtons_;

    InventoryWidget* invWidget_;
    BillingWidget* billWidget_;
    InvoicesWidget* invoicesWidget_;
    ReportsWidget* reportsWidget_;
    UsersWidget* usersWidget_;

    QLabel* statusDbLabel_;
    QLabel* statusAlertLabel_;

    void setupUi();
    void updateStatusBar();
};

