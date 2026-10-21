#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "models.h"

class AddProductDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddProductDialog(QWidget* parent = nullptr);
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

private:
    QLineEdit* nameInput_;
    QDoubleSpinBox* priceInput_;
    QSpinBox* quantityInput_;
};

class EditProductDialog : public QDialog {
    Q_OBJECT
public:
    explicit EditProductDialog(const Product& product, QWidget* parent = nullptr);
    std::string getName() const;
    double getPrice() const;

private:
    QLineEdit* nameInput_;
    QDoubleSpinBox* priceInput_;
};

class RestockDialog : public QDialog {
    Q_OBJECT
public:
    explicit RestockDialog(const Product& product, QWidget* parent = nullptr);
    int getAmount() const;

private:
    QSpinBox* amountInput_;
};

