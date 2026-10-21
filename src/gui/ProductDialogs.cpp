#include "ProductDialogs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

// ================= AddProductDialog =================
AddProductDialog::AddProductDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Add New Product");
    setFixedSize(360, 260);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    QLabel* title = new QLabel("Enter Product Details", this);
    title->setObjectName("TitleLabel");
    title->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(title);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    nameInput_ = new QLineEdit(this);
    nameInput_->setPlaceholderText("e.g. Wireless Mouse");
    form->addRow("Product Name:", nameInput_);

    priceInput_ = new QDoubleSpinBox(this);
    priceInput_->setRange(0.01, 1000000.0);
    priceInput_->setDecimals(2);
    priceInput_->setPrefix("Rs ");
    priceInput_->setValue(10.0);
    form->addRow("Unit Price:", priceInput_);

    quantityInput_ = new QSpinBox(this);
    quantityInput_->setRange(0, 100000);
    quantityInput_->setValue(20);
    form->addRow("Initial Stock:", quantityInput_);

    mainLayout->addLayout(form);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setObjectName("SecondaryButton");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(cancelBtn);

    QPushButton* saveBtn = new QPushButton("Save Product", this);
    saveBtn->setObjectName("SuccessButton");
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (nameInput_->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Product name cannot be empty.");
            return;
        }
        accept();
    });
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);
}

std::string AddProductDialog::getName() const {
    return nameInput_->text().trimmed().toStdString();
}

double AddProductDialog::getPrice() const {
    return priceInput_->value();
}

int AddProductDialog::getQuantity() const {
    return quantityInput_->value();
}

// ================= EditProductDialog =================
EditProductDialog::EditProductDialog(const Product& product, QWidget* parent) : QDialog(parent) {
    setWindowTitle("Edit Product");
    setFixedSize(360, 230);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    QLabel* title = new QLabel(QString("Edit Product #%1").arg(product.id), this);
    title->setObjectName("TitleLabel");
    title->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(title);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    nameInput_ = new QLineEdit(this);
    nameInput_->setText(QString::fromStdString(product.name));
    form->addRow("Product Name:", nameInput_);

    priceInput_ = new QDoubleSpinBox(this);
    priceInput_->setRange(0.01, 1000000.0);
    priceInput_->setDecimals(2);
    priceInput_->setPrefix("Rs ");
    priceInput_->setValue(product.price);
    form->addRow("Unit Price:", priceInput_);

    mainLayout->addLayout(form);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setObjectName("SecondaryButton");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(cancelBtn);

    QPushButton* saveBtn = new QPushButton("Update", this);
    saveBtn->setObjectName("SuccessButton");
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (nameInput_->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Product name cannot be empty.");
            return;
        }
        accept();
    });
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);
}

std::string EditProductDialog::getName() const {
    return nameInput_->text().trimmed().toStdString();
}

double EditProductDialog::getPrice() const {
    return priceInput_->value();
}

// ================= RestockDialog =================
RestockDialog::RestockDialog(const Product& product, QWidget* parent) : QDialog(parent) {
    setWindowTitle("Restock Inventory");
    setFixedSize(360, 240);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    QLabel* title = new QLabel("Add Stock Units", this);
    title->setObjectName("TitleLabel");
    title->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(title);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    QLabel* nameLbl = new QLabel(QString::fromStdString(product.name), this);
    nameLbl->setStyleSheet("font-weight: 600; color: #38bdf8;");
    form->addRow("Product:", nameLbl);

    QLabel* curStockLbl = new QLabel(QString("%1 units").arg(product.quantity), this);
    form->addRow("Current Stock:", curStockLbl);

    amountInput_ = new QSpinBox(this);
    amountInput_->setRange(1, 100000);
    amountInput_->setValue(10);
    form->addRow("Restock Amount:", amountInput_);

    mainLayout->addLayout(form);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setObjectName("SecondaryButton");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(cancelBtn);

    QPushButton* restockBtn = new QPushButton("Add to Stock", this);
    restockBtn->setObjectName("SuccessButton");
    connect(restockBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(restockBtn);

    mainLayout->addLayout(btnLayout);
}

int RestockDialog::getAmount() const {
    return amountInput_->value();
}

