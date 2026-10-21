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

