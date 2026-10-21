#include "InventoryWidget.h"
#include "ProductDialogs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

InventoryWidget::InventoryWidget(Inventory& inventory, const User& currentUser, QWidget* parent)
    : QWidget(parent), inventory_(inventory), currentUser_(currentUser) {
    setupUi();
    refreshData();
}

void InventoryWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Top Header & Controls
    QHBoxLayout* topBar = new QHBoxLayout();

    QVBoxLayout* titles = new QVBoxLayout();
    QLabel* heading = new QLabel("📦 Product Inventory Management", this);
    heading->setObjectName("TitleLabel");
    QLabel* sub = new QLabel("Manage product prices, stock thresholds, and listings", this);
    sub->setObjectName("SubtitleLabel");
    titles->addWidget(heading);
    titles->addWidget(sub);
    topBar->addLayout(titles);
    topBar->addStretch();

    // Admin-specific actions in header
    bool isAdmin = (currentUser_.role == Role::Admin);

    if (isAdmin) {
        addBtn_ = new QPushButton("➕ Add Product", this);
        addBtn_->setObjectName("SuccessButton");
        connect(addBtn_, &QPushButton::clicked, this, &InventoryWidget::handleAdd);
        topBar->addWidget(addBtn_);

        editBtn_ = new QPushButton("✏️ Edit", this);
        editBtn_->setObjectName("SecondaryButton");
        editBtn_->setEnabled(false);
        connect(editBtn_, &QPushButton::clicked, this, &InventoryWidget::handleEdit);
        topBar->addWidget(editBtn_);

        restockBtn_ = new QPushButton("📦 Restock", this);
        restockBtn_->setObjectName("SecondaryButton");
        restockBtn_->setEnabled(false);
        connect(restockBtn_, &QPushButton::clicked, this, &InventoryWidget::handleRestock);
        topBar->addWidget(restockBtn_);

        deleteBtn_ = new QPushButton("🗑️ Delete", this);
        deleteBtn_->setObjectName("DangerButton");
        deleteBtn_->setEnabled(false);
        connect(deleteBtn_, &QPushButton::clicked, this, &InventoryWidget::handleDelete);
        topBar->addWidget(deleteBtn_);
    } else {
        QLabel* cashierNotice = new QLabel("🔒 Cashier Mode (Read-only Catalog)", this);
        cashierNotice->setStyleSheet("color: #38bdf8; background-color: #1e3a8a; padding: 6px 12px; border-radius: 6px; font-weight: bold;");
        topBar->addWidget(cashierNotice);
    }

    QPushButton* refreshBtn = new QPushButton("🔄 Refresh", this);
    refreshBtn->setObjectName("SecondaryButton");
    connect(refreshBtn, &QPushButton::clicked, this, &InventoryWidget::refreshData);
    topBar->addWidget(refreshBtn);

    mainLayout->addLayout(topBar);

    // Search and Filter Bar
    QHBoxLayout* filterBar = new QHBoxLayout();
    searchInput_ = new QLineEdit(this);
    searchInput_->setPlaceholderText("🔍 Search products by name...");
    searchInput_->setClearButtonEnabled(true);
    connect(searchInput_, &QLineEdit::textChanged, this, &InventoryWidget::handleSearch);
    filterBar->addWidget(searchInput_, 3);

    lowStockFilterCheck_ = new QCheckBox("⚠️ Show Low Stock Only (< 5 units)", this);
    lowStockFilterCheck_->setStyleSheet("color: #f59e0b; font-weight: 600; padding-left: 8px;");
    connect(lowStockFilterCheck_, &QCheckBox::toggled, this, &InventoryWidget::handleFilterToggle);
    filterBar->addWidget(lowStockFilterCheck_, 1);

    mainLayout->addLayout(filterBar);

    // Table
    table_ = new QTableWidget(this);
    table_->setColumnCount(5);
    table_->setHorizontalHeaderLabels({"ID", "Product Name", "Unit Price", "Stock Quantity", "Status"});
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->verticalHeader()->setVisible(false);

    connect(table_, &QTableWidget::itemSelectionChanged, this, &InventoryWidget::onSelectionChanged);
    if (isAdmin) {
        connect(table_, &QTableWidget::cellDoubleClicked, this, [this](int, int) { handleEdit(); });
    }

    mainLayout->addWidget(table_);
}

void InventoryWidget::refreshData() {
    try {
        if (lowStockFilterCheck_->isChecked()) {
            currentList_ = inventory_.getLowStock(5);
        } else {
            std::string q = searchInput_->text().trimmed().toStdString();
            if (!q.empty()) {
                currentList_ = inventory_.search(q);
            } else {
                currentList_ = inventory_.getAll();
            }
        }
        populateTable(currentList_);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to load inventory: %1").arg(e.what()));
    }
}

void InventoryWidget::populateTable(const std::vector<Product>& list) {
    table_->setRowCount(static_cast<int>(list.size()));
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        const auto& p = list[i];

        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(p.id));
        idItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(i, 0, idItem);

        // Name
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(p.name));
        table_->setItem(i, 1, nameItem);

        // Price
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(p.price, 'f', 2)));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table_->setItem(i, 2, priceItem);

        // Quantity
        QTableWidgetItem* qtyItem = new QTableWidgetItem(QString::number(p.quantity));
        qtyItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(i, 3, qtyItem);

        // Status Badge
        QTableWidgetItem* statusItem = new QTableWidgetItem();
        statusItem->setTextAlignment(Qt::AlignCenter);
        if (p.quantity == 0) {
            statusItem->setText("Out of Stock");
            statusItem->setForeground(QColor("#ef4444")); // Red
        } else if (p.quantity < 5) {
            statusItem->setText("Low Stock");
            statusItem->setForeground(QColor("#f59e0b")); // Amber
        } else {
            statusItem->setText("In Stock");
            statusItem->setForeground(QColor("#10b981")); // Emerald
        }
        table_->setItem(i, 4, statusItem);
    }
    onSelectionChanged();
}

std::optional<Product> InventoryWidget::getSelectedProduct() const {
    int row = table_->currentRow();
    if (row < 0 || row >= static_cast<int>(currentList_.size())) {
        return std::nullopt;
    }
    return currentList_[row];
}

void InventoryWidget::onSelectionChanged() {
    bool hasSelection = (table_->currentRow() >= 0);
    if (currentUser_.role == Role::Admin) {
        editBtn_->setEnabled(hasSelection);
        restockBtn_->setEnabled(hasSelection);
        deleteBtn_->setEnabled(hasSelection);
    }
}

void InventoryWidget::handleSearch() {
    refreshData();
}

void InventoryWidget::handleFilterToggle() {
    refreshData();
}

void InventoryWidget::handleAdd() {
    AddProductDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            inventory_.addProduct(currentUser_, dialog.getName(), dialog.getPrice(), dialog.getQuantity());
            refreshData();
            emit inventoryModified();
            QMessageBox::information(this, "Success", "Product added successfully!");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to add product: %1").arg(e.what()));
        }
    }
}

void InventoryWidget::handleEdit() {
    auto prod = getSelectedProduct();
    if (!prod) return;

    EditProductDialog dialog(prod.value(), this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            inventory_.updateProduct(currentUser_, prod->id, dialog.getName(), dialog.getPrice());
            refreshData();
            emit inventoryModified();
            QMessageBox::information(this, "Success", "Product updated successfully!");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to update product: %1").arg(e.what()));
        }
    }
}

void InventoryWidget::handleRestock() {
    auto prod = getSelectedProduct();
    if (!prod) return;

    RestockDialog dialog(prod.value(), this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            inventory_.restock(currentUser_, prod->id, dialog.getAmount());
            refreshData();
            emit inventoryModified();
            QMessageBox::information(this, "Success", QString("Added %1 units to stock!").arg(dialog.getAmount()));
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to restock: %1").arg(e.what()));
        }
    }
}

void InventoryWidget::handleDelete() {
    auto prod = getSelectedProduct();
    if (!prod) return;

    auto reply = QMessageBox::question(this, "Confirm Deletion",
        QString("Are you sure you want to delete '%1' (ID: %2)?").arg(QString::fromStdString(prod->name)).arg(prod->id),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        try {
            inventory_.removeProduct(currentUser_, prod->id);
            refreshData();
            emit inventoryModified();
            QMessageBox::information(this, "Success", "Product deleted successfully.");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to delete: %1").arg(e.what()));
        }
    }
}

