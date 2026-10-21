#include "InvoicesWidget.h"
#include "InvoiceDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

InvoicesWidget::InvoicesWidget(Billing& billing, const User& currentUser, QWidget* parent)
    : QWidget(parent), billing_(billing), currentUser_(currentUser) {
    setupUi();
    refreshInvoices();
}

void InvoicesWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Header
    QHBoxLayout* topBar = new QHBoxLayout();
    QVBoxLayout* titles = new QVBoxLayout();
    QLabel* heading = new QLabel("🧾 Invoices & Transaction History", this);
    heading->setObjectName("TitleLabel");
    QLabel* sub = new QLabel("Lookup past customer invoices, view detailed itemized receipts, and reprint", this);
    sub->setObjectName("SubtitleLabel");
    titles->addWidget(heading);
    titles->addWidget(sub);
    topBar->addLayout(titles);
    topBar->addStretch();

    viewBtn_ = new QPushButton("👁️ View Invoice Details", this);
    viewBtn_->setObjectName("SuccessButton");
    viewBtn_->setEnabled(false);
    connect(viewBtn_, &QPushButton::clicked, this, &InvoicesWidget::viewSelectedInvoice);
    topBar->addWidget(viewBtn_);

    QPushButton* refreshBtn = new QPushButton("🔄 Refresh", this);
    refreshBtn->setObjectName("SecondaryButton");
    connect(refreshBtn, &QPushButton::clicked, this, &InvoicesWidget::refreshInvoices);
    topBar->addWidget(refreshBtn);

    mainLayout->addLayout(topBar);

    // Search bar
    QHBoxLayout* searchBar = new QHBoxLayout();
    searchInput_ = new QLineEdit(this);
    searchInput_->setPlaceholderText("🔍 Filter invoices by Bill ID or Cashier name...");
    searchInput_->setClearButtonEnabled(true);
    connect(searchInput_, &QLineEdit::textChanged, this, &InvoicesWidget::handleSearch);
    searchBar->addWidget(searchInput_);
    mainLayout->addLayout(searchBar);

    // Table
    table_ = new QTableWidget(this);
    table_->setColumnCount(5);
    table_->setHorizontalHeaderLabels({"Bill #", "Date & Time", "Cashier", "Items Count", "Grand Total"});
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->verticalHeader()->setVisible(false);

    connect(table_, &QTableWidget::itemSelectionChanged, this, &InvoicesWidget::onSelectionChanged);
    connect(table_, &QTableWidget::cellDoubleClicked, this, [this](int, int) { viewSelectedInvoice(); });

    mainLayout->addWidget(table_);
}

void InvoicesWidget::refreshInvoices() {
    try {
        billsList_ = billing_.getRecentBills(100);
        handleSearch();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to load invoices: %1").arg(e.what()));
    }
}

void InvoicesWidget::handleSearch() {
    QString q = searchInput_->text().trimmed().toLower();
    std::vector<Bill> filtered;
    for (const auto& b : billsList_) {
        QString billIdStr = QString::number(b.id);
        QString cashierStr = QString::fromStdString(b.cashier).toLower();
        if (q.isEmpty() || billIdStr.contains(q) || cashierStr.contains(q)) {
            filtered.push_back(b);
        }
    }
    populateTable(filtered);
}

void InvoicesWidget::populateTable(const std::vector<Bill>& list) {
    table_->setRowCount(static_cast<int>(list.size()));
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        const auto& b = list[i];

        // Bill #
        QTableWidgetItem* idItem = new QTableWidgetItem(QString("#%1").arg(b.id));
        idItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(i, 0, idItem);

        // Date
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(b.createdAt));
        table_->setItem(i, 1, dateItem);

        // Cashier
        QTableWidgetItem* cashierItem = new QTableWidgetItem(QString::fromStdString(b.cashier));
        cashierItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(i, 2, cashierItem);

        // Items Count
        int totalQty = 0;
        for (const auto& it : b.items) totalQty += it.quantity;
        QTableWidgetItem* itemsItem = new QTableWidgetItem(QString("%1 items (%2 lines)").arg(totalQty).arg(b.items.size()));
        itemsItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(i, 3, itemsItem);

        // Grand Total
        QTableWidgetItem* totalItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(b.total, 'f', 2)));
        totalItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        totalItem->setForeground(QColor("#10b981"));
        table_->setItem(i, 4, totalItem);
    }
    onSelectionChanged();
}

void InvoicesWidget::onSelectionChanged() {
    viewBtn_->setEnabled(table_->currentRow() >= 0);
}

void InvoicesWidget::viewSelectedInvoice() {
    int row = table_->currentRow();
    if (row < 0 || row >= static_cast<int>(billsList_.size())) return;

    // Get bill id from table
    auto* item = table_->item(row, 0);
    if (!item) return;
    int billId = item->text().replace("#", "").toInt();

    try {
        Bill b = billing_.getBill(billId);
        InvoiceDialog dialog(b, this);
        dialog.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Could not open invoice #%1: %2").arg(billId).arg(e.what()));
    }
}

