#include "MainWindow.h"
#include "Theme.h"
#include "InventoryWidget.h"
#include "BillingWidget.h"
#include "InvoicesWidget.h"
#include "ReportsWidget.h"
#include "UsersWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QFrame>

MainWindow::MainWindow(Database& db, Auth& auth, Inventory& inventory, Billing& billing, const User& currentUser, QWidget* parent)
    : QMainWindow(parent), db_(db), auth_(auth), inventory_(inventory), billing_(billing), currentUser_(currentUser) {
    setWindowTitle("Inventory Management");
    resize(1200, 750);
    setMinimumSize(950, 600);

    setupUi();
    updateStatusBar();
}

void MainWindow::setupUi() {
    QWidget* central = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ================= TOP HEADER BAR =================
    QFrame* headerBar = new QFrame(this);
    headerBar->setObjectName("HeaderBar");
    QHBoxLayout* headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 10, 16, 10);

    // Brand Title
    QLabel* brandLabel = new QLabel("📦 Inventory Management", headerBar);
    brandLabel->setObjectName("BrandLabel");
    headerLayout->addWidget(brandLabel);

    headerLayout->addStretch();

    // User status pill
    QFrame* userPill = new QFrame(headerBar);
    userPill->setObjectName("UserPill");
    QHBoxLayout* pillLay = new QHBoxLayout(userPill);
    pillLay->setContentsMargins(6, 2, 6, 2);
    pillLay->setSpacing(8);

    QLabel* userIcon = new QLabel("👤", userPill);
    pillLay->addWidget(userIcon);

    QLabel* userNameLbl = new QLabel(QString("User: <strong>%1</strong>").arg(QString::fromStdString(currentUser_.username)), userPill);
    userNameLbl->setObjectName("UserNameLabel");
    pillLay->addWidget(userNameLbl);

    QLabel* roleBadge = new QLabel(userPill);
    if (currentUser_.role == Role::Admin) {
        roleBadge->setText("ADMIN");
        roleBadge->setStyleSheet("background-color: #7e22ce; color: #ffffff; font-weight: bold; font-size: 10px; padding: 2px 8px; border-radius: 10px;");
    } else {
        roleBadge->setText("CASHIER");
        roleBadge->setStyleSheet("background-color: #059669; color: #ffffff; font-weight: bold; font-size: 10px; padding: 2px 8px; border-radius: 10px;");
    }
    pillLay->addWidget(roleBadge);
    headerLayout->addWidget(userPill);

    headerLayout->addSpacing(14);

    // Theme Toggle Button
    themeToggleBtn_ = new QPushButton("☀️ Light", headerBar);
    themeToggleBtn_->setObjectName("ThemeToggleBtn");
    themeToggleBtn_->setToolTip("Switch between Dark and Light mode");
    themeToggleBtn_->setCursor(Qt::PointingHandCursor);
    connect(themeToggleBtn_, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    headerLayout->addWidget(themeToggleBtn_);

    headerLayout->addSpacing(8);

    // Logout Button
    QPushButton* logoutBtn = new QPushButton("🚪 Logout", headerBar);
    logoutBtn->setObjectName("SecondaryButton");
    logoutBtn->setStyleSheet("font-size: 12px; padding: 6px 14px;");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::logoutRequested);
    headerLayout->addWidget(logoutBtn);

    rootLayout->addWidget(headerBar);

    // ================= BODY: SIDEBAR + STACKED WIDGET =================
    QHBoxLayout* bodyLayout = new QHBoxLayout();
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // Sidebar
    QFrame* sidebar = new QFrame(this);
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(220);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(10, 16, 10, 16);
    sideLayout->setSpacing(8);

    auto addNavButton = [this, sideLayout](const QString& text, int targetIndex) -> QPushButton* {
        QPushButton* btn = new QPushButton(text, this);
        btn->setObjectName("NavButton");
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, targetIndex]() { switchTab(targetIndex); });
        sideLayout->addWidget(btn);
        navButtons_.push_back(btn);
        return btn;
    };

    addNavButton("📦 Products & Stock", 0);
    addNavButton("🛒 Point of Sale (POS)", 1);
    addNavButton("🧾 Invoices & Bills", 2);

    if (currentUser_.role == Role::Admin) {
        addNavButton("📊 Sales Analytics", 3);
        addNavButton("👥 User Accounts", 4);
    }

    sideLayout->addStretch();

    // App Version badge
    QLabel* verLabel = new QLabel("Version 1.0 (Qt)", sidebar);
    verLabel->setStyleSheet("color: #64748b; font-size: 11px; text-align: center;");
    verLabel->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(verLabel);

    bodyLayout->addWidget(sidebar);

    // Stacked Widget
    stackedWidget_ = new QStackedWidget(this);

    invWidget_ = new InventoryWidget(inventory_, currentUser_, stackedWidget_);
    billWidget_ = new BillingWidget(billing_, inventory_, currentUser_, stackedWidget_);
    invoicesWidget_ = new InvoicesWidget(billing_, currentUser_, stackedWidget_);

    stackedWidget_->addWidget(invWidget_);     // Index 0
    stackedWidget_->addWidget(billWidget_);    // Index 1
    stackedWidget_->addWidget(invoicesWidget_);// Index 2

    if (currentUser_.role == Role::Admin) {
        reportsWidget_ = new ReportsWidget(billing_, inventory_, currentUser_, stackedWidget_);
        usersWidget_ = new UsersWidget(auth_, currentUser_, stackedWidget_);
        stackedWidget_->addWidget(reportsWidget_); // Index 3
        stackedWidget_->addWidget(usersWidget_);   // Index 4
    } else {
        reportsWidget_ = nullptr;
        usersWidget_ = nullptr;
    }

    bodyLayout->addWidget(stackedWidget_, 1);
    rootLayout->addLayout(bodyLayout);

    setCentralWidget(central);

    // ================= STATUS BAR =================
    QStatusBar* status = statusBar();
    statusDbLabel_ = new QLabel("Database: inventory.db (SQLite3 Connected)", this);
    statusDbLabel_->setStyleSheet("color: #94a3b8; font-size: 12px; margin-left: 10px;");
    status->addWidget(statusDbLabel_);

    statusAlertLabel_ = new QLabel("", this);
    statusAlertLabel_->setStyleSheet("color: #f59e0b; font-size: 12px; font-weight: bold; margin-right: 15px;");
    status->addPermanentWidget(statusAlertLabel_);

    // Cross-widget wiring
    connect(invWidget_, &InventoryWidget::inventoryModified, this, &MainWindow::onInventoryChanged);
    connect(billWidget_, &BillingWidget::saleCompleted, this, &MainWindow::onSaleCompleted);

    // Start on Catalog or POS based on role
    if (currentUser_.role == Role::Cashier) {
        switchTab(1); // POS default for Cashier
    } else {
        switchTab(0); // Products default for Admin
    }
}

void MainWindow::switchTab(int index) {
    if (index < 0 || index >= stackedWidget_->count()) return;

    stackedWidget_->setCurrentIndex(index);
    for (size_t i = 0; i < navButtons_.size(); ++i) {
        navButtons_[i]->setChecked(static_cast<int>(i) == index);
    }
}

void MainWindow::onInventoryChanged() {
    billWidget_->refreshCatalog();
    if (reportsWidget_) reportsWidget_->refreshReports();
    updateStatusBar();
}

void MainWindow::onSaleCompleted() {
    invWidget_->refreshData();
    invoicesWidget_->refreshInvoices();
    if (reportsWidget_) reportsWidget_->refreshReports();
    updateStatusBar();
}

void MainWindow::updateStatusBar() {
    try {
        auto lowStock = inventory_.getLowStock(5);
        if (!lowStock.empty()) {
            statusAlertLabel_->setText(QString("⚠️ %1 product(s) in low stock!").arg(lowStock.size()));
        } else {
            statusAlertLabel_->setText("✅ All products sufficiently stocked");
        }
    } catch (...) {}
}

void MainWindow::toggleTheme() {
    isDarkMode_ = !isDarkMode_;
    if (isDarkMode_) {
        themeToggleBtn_->setText("☀️ Light");
        qApp->setStyleSheet(Theme::darkStyleSheet());
    } else {
        themeToggleBtn_->setText("🌙 Dark");
        qApp->setStyleSheet(Theme::lightStyleSheet());
    }
}

