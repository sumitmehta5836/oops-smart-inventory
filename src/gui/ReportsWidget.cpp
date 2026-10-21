#include "ReportsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFrame>
#include <QMessageBox>

ReportsWidget::ReportsWidget(Billing& billing, Inventory& inventory, const User& currentUser, QWidget* parent)
    : QWidget(parent), billing_(billing), inventory_(inventory), currentUser_(currentUser) {
    setupUi();
    refreshReports();
}

void ReportsWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(18);

    // Header
    QHBoxLayout* topBar = new QHBoxLayout();
    QVBoxLayout* titles = new QVBoxLayout();
    QLabel* heading = new QLabel("📊 Sales Analytics & Executive Reports", this);
    heading->setObjectName("TitleLabel");
    QLabel* sub = new QLabel("Real-time revenue metrics, daily trends, and product performance analysis", this);
    sub->setObjectName("SubtitleLabel");
    titles->addWidget(heading);
    titles->addWidget(sub);
    topBar->addLayout(titles);
    topBar->addStretch();

    QPushButton* refreshBtn = new QPushButton("🔄 Refresh Analytics", this);
    refreshBtn->setObjectName("SecondaryButton");
    connect(refreshBtn, &QPushButton::clicked, this, &ReportsWidget::refreshReports);
    topBar->addWidget(refreshBtn);

    mainLayout->addLayout(topBar);

    // KPI Cards Row
    QHBoxLayout* cardsLayout = new QHBoxLayout();

    auto createStatCard = [](const QString& title, const QString& initialValue, const QString& color) -> std::pair<QFrame*, QLabel*> {
        QFrame* card = new QFrame();
        card->setObjectName("StatCard");
        QVBoxLayout* lay = new QVBoxLayout(card);
        lay->setContentsMargins(16, 14, 16, 14);
        lay->setSpacing(6);

        QLabel* titleLbl = new QLabel(title, card);
        titleLbl->setStyleSheet("color: #94a3b8; font-size: 12px; font-weight: 600; text-transform: uppercase;");
        lay->addWidget(titleLbl);

        QLabel* valLbl = new QLabel(initialValue, card);
        valLbl->setStyleSheet(QString("font-size: 22px; font-weight: bold; color: %1;").arg(color));
        lay->addWidget(valLbl);

        return { card, valLbl };
    };

    auto revCard = createStatCard("Total Revenue Recorded", "Rs 0.00", "#10b981");
    totalRevenueCardLbl_ = revCard.second;
    cardsLayout->addWidget(revCard.first);

    auto billsCard = createStatCard("Total Invoices Generated", "0 Bills", "#38bdf8");
    totalBillsCardLbl_ = billsCard.second;
    cardsLayout->addWidget(billsCard.first);

    auto lowStockCard = createStatCard("Low Stock Warnings", "0 Items", "#f59e0b");
    lowStockCountCardLbl_ = lowStockCard.second;
    cardsLayout->addWidget(lowStockCard.first);

    mainLayout->addLayout(cardsLayout);

    // Two Tables side by side: Daily Sales & Top Products
    QHBoxLayout* tablesLayout = new QHBoxLayout();

    // Daily Sales Table
    QVBoxLayout* dailyCol = new QVBoxLayout();
    QLabel* dailyTitle = new QLabel("📅 Daily Sales History", this);
    dailyTitle->setObjectName("SectionHeader");
    dailyCol->addWidget(dailyTitle);

    dailySalesTable_ = new QTableWidget(this);
    dailySalesTable_->setColumnCount(3);
    dailySalesTable_->setHorizontalHeaderLabels({"Date", "Bills Count", "Daily Revenue"});
    dailySalesTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    dailySalesTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    dailySalesTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    dailySalesTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dailySalesTable_->verticalHeader()->setVisible(false);
    dailyCol->addWidget(dailySalesTable_);
    tablesLayout->addLayout(dailyCol);

    // Top Products Table
    QVBoxLayout* topCol = new QVBoxLayout();
    QLabel* topTitle = new QLabel("🏆 Top Selling Products", this);
    topTitle->setObjectName("SectionHeader");
    topCol->addWidget(topTitle);

    topProductsTable_ = new QTableWidget(this);
    topProductsTable_->setColumnCount(3);
    topProductsTable_->setHorizontalHeaderLabels({"Product Name", "Units Sold", "Total Revenue"});
    topProductsTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    topProductsTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    topProductsTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    topProductsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    topProductsTable_->verticalHeader()->setVisible(false);
    topCol->addWidget(topProductsTable_);
    tablesLayout->addLayout(topCol);

    mainLayout->addLayout(tablesLayout);
}

void ReportsWidget::refreshReports() {
    try {
        // Daily Sales
        auto daily = billing_.dailySales(currentUser_, 30);
        dailySalesTable_->setRowCount(static_cast<int>(daily.size()));
        double totalRev = 0.0;
        int totalBills = 0;

        for (int i = 0; i < static_cast<int>(daily.size()); ++i) {
            const auto& d = daily[i];
            totalRev += d.revenue;
            totalBills += d.billCount;

            QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(d.date));
            dailySalesTable_->setItem(i, 0, dateItem);

            QTableWidgetItem* countItem = new QTableWidgetItem(QString::number(d.billCount));
            countItem->setTextAlignment(Qt::AlignCenter);
            dailySalesTable_->setItem(i, 1, countItem);

            QTableWidgetItem* revItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(d.revenue, 'f', 2)));
            revItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            revItem->setForeground(QColor("#10b981"));
            dailySalesTable_->setItem(i, 2, revItem);
        }

        totalRevenueCardLbl_->setText(QString("Rs %1").arg(QString::number(totalRev, 'f', 2)));
        totalBillsCardLbl_->setText(QString("%1 Bills").arg(totalBills));

        // Top Products
        auto top = billing_.topProducts(currentUser_, 10);
        topProductsTable_->setRowCount(static_cast<int>(top.size()));
        for (int i = 0; i < static_cast<int>(top.size()); ++i) {
            const auto& t = top[i];

            QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(t.name));
            topProductsTable_->setItem(i, 0, nameItem);

            QTableWidgetItem* unitsItem = new QTableWidgetItem(QString("%1 units").arg(t.unitsSold));
            unitsItem->setTextAlignment(Qt::AlignCenter);
            topProductsTable_->setItem(i, 1, unitsItem);

            QTableWidgetItem* revItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(t.revenue, 'f', 2)));
            revItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            revItem->setForeground(QColor("#38bdf8"));
            topProductsTable_->setItem(i, 2, revItem);
        }

        // Low stock count
        auto lowStock = inventory_.getLowStock(5);
        lowStockCountCardLbl_->setText(QString("%1 Items").arg(lowStock.size()));

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Reports Error", QString("Failed to generate analytics: %1").arg(e.what()));
    }
}

