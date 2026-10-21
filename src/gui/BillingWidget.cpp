#include "BillingWidget.h"
#include "InvoiceDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>

BillingWidget::BillingWidget(Billing& billing, Inventory& inventory, const User& currentUser, QWidget* parent)
    : QWidget(parent), billing_(billing), inventory_(inventory), currentUser_(currentUser) {
    setupUi();
    refreshCatalog();
}

void BillingWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Top Header
    QVBoxLayout* titles = new QVBoxLayout();
    QLabel* heading = new QLabel("🛒 Point of Sale & Billing", this);
    heading->setObjectName("TitleLabel");
    QLabel* sub = new QLabel(QString("Active Cashier: %1 | Create transactions and issue instant invoices")
                                 .arg(QString::fromStdString(currentUser_.username)), this);
    sub->setObjectName("SubtitleLabel");
    titles->addWidget(heading);
    titles->addWidget(sub);
    mainLayout->addLayout(titles);

    // Splitter: Left (Catalog) vs Right (Cart)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // ================= LEFT: PRODUCT PICKER =================
    QWidget* leftContainer = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 8, 0);

    QLabel* catalogTitle = new QLabel("Available Products", leftContainer);
    catalogTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #38bdf8;");
    leftLayout->addWidget(catalogTitle);

    searchInput_ = new QLineEdit(leftContainer);
    searchInput_->setPlaceholderText("🔍 Search catalog by name...");
    searchInput_->setClearButtonEnabled(true);
    connect(searchInput_, &QLineEdit::textChanged, this, &BillingWidget::handleSearch);
    leftLayout->addWidget(searchInput_);

    catalogTable_ = new QTableWidget(leftContainer);
    catalogTable_->setColumnCount(4);
    catalogTable_->setHorizontalHeaderLabels({"ID", "Product Name", "Price", "Stock"});
    catalogTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    catalogTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    catalogTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    catalogTable_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    catalogTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    catalogTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    catalogTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    catalogTable_->verticalHeader()->setVisible(false);
    connect(catalogTable_, &QTableWidget::cellDoubleClicked, this, [this](int, int) { addToCartClicked(); });
    leftLayout->addWidget(catalogTable_);

    // Add to Cart Bar
    QHBoxLayout* addBar = new QHBoxLayout();
    QLabel* qtyLbl = new QLabel("Qty:", leftContainer);
    addBar->addWidget(qtyLbl);

    addQuantitySpin_ = new QSpinBox(leftContainer);
    addQuantitySpin_->setRange(1, 9999);
    addQuantitySpin_->setValue(1);
    addBar->addWidget(addQuantitySpin_);

    addToCartBtn_ = new QPushButton("➕ Add to Cart", leftContainer);
    addToCartBtn_->setObjectName("SuccessButton");
    connect(addToCartBtn_, &QPushButton::clicked, this, &BillingWidget::addToCartClicked);
    addBar->addWidget(addToCartBtn_);

    leftLayout->addLayout(addBar);
    splitter->addWidget(leftContainer);

    // ================= RIGHT: ACTIVE CART =================
    QWidget* rightContainer = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(8, 0, 0, 0);

    QLabel* cartTitle = new QLabel("Current Sale Cart", rightContainer);
    cartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #10b981;");
    rightLayout->addWidget(cartTitle);

    cartTable_ = new QTableWidget(rightContainer);
    cartTable_->setColumnCount(5);
    cartTable_->setHorizontalHeaderLabels({"Item", "Price", "Qty", "Total", "Action"});
    cartTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    cartTable_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    cartTable_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    cartTable_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    cartTable_->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    cartTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable_->verticalHeader()->setVisible(false);
    rightLayout->addWidget(cartTable_);

    // Cart Summary Box
    QFrame* summaryFrame = new QFrame(rightContainer);
    summaryFrame->setObjectName("ContentCard");
    QVBoxLayout* summaryLayout = new QVBoxLayout(summaryFrame);

    QHBoxLayout* line1 = new QHBoxLayout();
    itemCountLabel_ = new QLabel("Items in Cart: 0", summaryFrame);
    itemCountLabel_->setStyleSheet("font-size: 13px; color: #94a3b8;");
    line1->addWidget(itemCountLabel_);
    line1->addStretch();
    clearCartBtn_ = new QPushButton("🗑️ Clear Cart", summaryFrame);
    clearCartBtn_->setObjectName("DangerButton");
    clearCartBtn_->setStyleSheet("font-size: 11px; padding: 4px 10px;");
    connect(clearCartBtn_, &QPushButton::clicked, this, &BillingWidget::clearCart);
    line1->addWidget(clearCartBtn_);
    summaryLayout->addLayout(line1);

    QHBoxLayout* line2 = new QHBoxLayout();
    QLabel* totalTitle = new QLabel("Grand Total:", summaryFrame);
    totalTitle->setObjectName("GrandTotalLabel");
    totalAmountLabel_ = new QLabel("Rs 0.00", summaryFrame);
    totalAmountLabel_->setStyleSheet("font-size: 22px; font-weight: bold; color: #10b981;");
    line2->addWidget(totalTitle);
    line2->addStretch();
    line2->addWidget(totalAmountLabel_);
    summaryLayout->addLayout(line2);

    checkoutBtn_ = new QPushButton("💳 Complete Sale & Generate Invoice", summaryFrame);
    checkoutBtn_->setObjectName("SuccessButton");
    checkoutBtn_->setFixedHeight(42);
    checkoutBtn_->setEnabled(false);
    connect(checkoutBtn_, &QPushButton::clicked, this, &BillingWidget::completeSale);
    summaryLayout->addWidget(checkoutBtn_);

    rightLayout->addWidget(summaryFrame);
    splitter->addWidget(rightContainer);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter);
}

void BillingWidget::refreshCatalog() {
    try {
        std::string q = searchInput_->text().trimmed().toStdString();
        if (!q.empty()) {
            catalogList_ = inventory_.search(q);
        } else {
            catalogList_ = inventory_.getAll();
        }
        populateCatalog(catalogList_);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to load catalog: %1").arg(e.what()));
    }
}

void BillingWidget::populateCatalog(const std::vector<Product>& list) {
    catalogTable_->setRowCount(static_cast<int>(list.size()));
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        const auto& p = list[i];

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(p.id));
        idItem->setTextAlignment(Qt::AlignCenter);
        catalogTable_->setItem(i, 0, idItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(p.name));
        catalogTable_->setItem(i, 1, nameItem);

        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(p.price, 'f', 2)));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        catalogTable_->setItem(i, 2, priceItem);

        QTableWidgetItem* qtyItem = new QTableWidgetItem(QString::number(p.quantity));
        qtyItem->setTextAlignment(Qt::AlignCenter);
        if (p.quantity == 0) {
            qtyItem->setForeground(QColor("#ef4444"));
        } else if (p.quantity < 5) {
            qtyItem->setForeground(QColor("#f59e0b"));
        } else {
            qtyItem->setForeground(QColor("#10b981"));
        }
        catalogTable_->setItem(i, 3, qtyItem);
    }
}

void BillingWidget::handleSearch() {
    refreshCatalog();
}

void BillingWidget::addToCartClicked() {
    int row = catalogTable_->currentRow();
    if (row < 0 || row >= static_cast<int>(catalogList_.size())) {
        QMessageBox::warning(this, "Selection", "Please select a product from the list.");
        return;
    }

    const Product& p = catalogList_[row];
    int reqQty = addQuantitySpin_->value();

    if (p.quantity <= 0) {
        QMessageBox::warning(this, "Out of Stock", QString("'%1' is out of stock!").arg(QString::fromStdString(p.name)));
        return;
    }

    int curInCart = (cart_.find(p.id) != cart_.end()) ? cart_[p.id].quantity : 0;
    if (curInCart + reqQty > p.quantity) {
        QMessageBox::warning(this, "Stock Limit",
            QString("Cannot add %1 units. Available in stock: %2 (Already in cart: %3)")
            .arg(reqQty).arg(p.quantity).arg(curInCart));
        return;
    }

    cart_[p.id] = CartEntry{ p, curInCart + reqQty };
    refreshCartTable();
    addQuantitySpin_->setValue(1);
}

void BillingWidget::refreshCartTable() {
    cartTable_->setRowCount(static_cast<int>(cart_.size()));
    int row = 0;
    for (auto& pair : cart_) {
        int pid = pair.first;
        const auto& entry = pair.second;

        // Name
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(entry.product.name));
        cartTable_->setItem(row, 0, nameItem);

        // Price
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(entry.product.price, 'f', 2)));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        cartTable_->setItem(row, 1, priceItem);

        // Quantity Spinbox directly in cell for quick tweak
        QSpinBox* qtySpin = new QSpinBox(cartTable_);
        qtySpin->setRange(1, entry.product.quantity);
        qtySpin->setValue(entry.quantity);
        connect(qtySpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, pid](int newQty) {
            if (cart_.find(pid) != cart_.end()) {
                cart_[pid].quantity = newQty;
                updateCartTotals();
            }
        });
        cartTable_->setCellWidget(row, 2, qtySpin);

        // Line Total
        double lineTotal = entry.product.price * entry.quantity;
        QTableWidgetItem* totalItem = new QTableWidgetItem(QString("Rs %1").arg(QString::number(lineTotal, 'f', 2)));
        totalItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        cartTable_->setItem(row, 3, totalItem);

        // Remove button
        QPushButton* removeBtn = new QPushButton("✕", cartTable_);
        removeBtn->setObjectName("DangerButton");
        removeBtn->setFixedSize(28, 26);
        connect(removeBtn, &QPushButton::clicked, this, [this, pid]() {
            cart_.erase(pid);
            refreshCartTable();
        });
        QWidget* btnWidget = new QWidget(cartTable_);
        QHBoxLayout* btnLay = new QHBoxLayout(btnWidget);
        btnLay->setContentsMargins(0, 0, 0, 0);
        btnLay->setAlignment(Qt::AlignCenter);
        btnLay->addWidget(removeBtn);
        cartTable_->setCellWidget(row, 4, btnWidget);

        row++;
    }
    updateCartTotals();
}

void BillingWidget::updateCartTotals() {
    double total = 0.0;
    int totalItems = 0;

    for (const auto& pair : cart_) {
        total += pair.second.product.price * pair.second.quantity;
        totalItems += pair.second.quantity;
    }

    itemCountLabel_->setText(QString("Items in Cart: %1").arg(totalItems));
    totalAmountLabel_->setText(QString("Rs %1").arg(QString::number(total, 'f', 2)));
    checkoutBtn_->setEnabled(!cart_.empty());

    // Also update line totals in the cart table
    int row = 0;
    for (const auto& pair : cart_) {
        double lineTotal = pair.second.product.price * pair.second.quantity;
        auto* item = cartTable_->item(row, 3);
        if (item) {
            item->setText(QString("Rs %1").arg(QString::number(lineTotal, 'f', 2)));
        }
        row++;
    }
}

void BillingWidget::removeFromCartClicked() {
    // Handled by cell widget buttons
}

void BillingWidget::clearCart() {
    if (cart_.empty()) return;
    cart_.clear();
    refreshCartTable();
}

void BillingWidget::completeSale() {
    if (cart_.empty()) return;

    std::vector<CartItem> items;
    for (const auto& pair : cart_) {
        items.push_back(CartItem{ pair.first, pair.second.quantity });
    }

    try {
        Bill bill = billing_.generateBill(currentUser_, items);

        // Clear cart & update UI
        cart_.clear();
        refreshCartTable();
        refreshCatalog();

        emit saleCompleted();

        // Show Invoice Dialog
        InvoiceDialog dialog(bill, this);
        dialog.exec();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Billing Failed",
            QString("Transaction failed and rolled back safely:\n\n%1").arg(e.what()));
        refreshCatalog();
    }
}

