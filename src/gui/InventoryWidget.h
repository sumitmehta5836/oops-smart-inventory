#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include "Inventory.h"
#include "models.h"

// OOP CONCEPT: INHERITANCE
// InventoryWidget inherits from the QWidget base class.
// Base Class: QWidget (furnishes UI layout placement, event handling, and painting)
// Derived Class: InventoryWidget (specializes QWidget into a full product inventory management view)
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates the product table, search inputs, dialog launchers, and Inventory service reference.
class InventoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit InventoryWidget(Inventory& inventory, const User& currentUser, QWidget* parent = nullptr);
    void refreshData();

signals:
    void inventoryModified();

private slots:
    void handleSearch();
    void handleFilterToggle();
    void handleAdd();
    void handleEdit();
    void handleRestock();
    void handleDelete();
    void onSelectionChanged();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // Widget controls, state collections, and backend service handles are kept private.
    Inventory& inventory_;
    User currentUser_;

    QLineEdit* searchInput_;
    QCheckBox* lowStockFilterCheck_;
    QTableWidget* table_;

    QPushButton* addBtn_;
    QPushButton* editBtn_;
    QPushButton* restockBtn_;
    QPushButton* deleteBtn_;

    std::vector<Product> currentList_;

    void setupUi();
    void populateTable(const std::vector<Product>& list);
    std::optional<Product> getSelectedProduct() const;
};

