#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "models.h"

// OOP CONCEPT: INHERITANCE
// AddProductDialog inherits from QDialog to reuse modal window mechanisms.
// Base Class: QDialog | Derived Class: AddProductDialog
//
// OOP CONCEPT: ENCAPSULATION
// Private input controls hide widget pointers; caller accesses user input via public getters.
class AddProductDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddProductDialog(QWidget* parent = nullptr);
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    QLineEdit* nameInput_;
    QDoubleSpinBox* priceInput_;
    QSpinBox* quantityInput_;
};

// OOP CONCEPT: INHERITANCE
// EditProductDialog inherits from QDialog.
// Base Class: QDialog | Derived Class: EditProductDialog
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates product update form inputs, exposing read-only values through getters.
class EditProductDialog : public QDialog {
    Q_OBJECT
public:
    explicit EditProductDialog(const Product& product, QWidget* parent = nullptr);
    std::string getName() const;
    double getPrice() const;

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    QLineEdit* nameInput_;
    QDoubleSpinBox* priceInput_;
};

// OOP CONCEPT: INHERITANCE
// RestockDialog inherits from QDialog.
// Base Class: QDialog | Derived Class: RestockDialog
class RestockDialog : public QDialog {
    Q_OBJECT
public:
    explicit RestockDialog(const Product& product, QWidget* parent = nullptr);
    int getAmount() const;

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    QSpinBox* amountInput_;
};

