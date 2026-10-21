#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include "Inventory.h"
#include "models.h"

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

