#include "UsersWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFrame>
#include <QMessageBox>

UsersWidget::UsersWidget(Auth& auth, const User& currentUser, QWidget* parent)
    : QWidget(parent), auth_(auth), currentUser_(currentUser) {
    setupUi();
    refreshUsers();
}

void UsersWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(18);

    // Header
    QVBoxLayout* titles = new QVBoxLayout();
    QLabel* heading = new QLabel("👥 User Management & Role-Based Access Control", this);
    heading->setObjectName("TitleLabel");
    QLabel* sub = new QLabel("Provision Cashier and Administrator credentials with salted SHA-256 security", this);
    sub->setObjectName("SubtitleLabel");
    titles->addWidget(heading);
    titles->addWidget(sub);
    mainLayout->addLayout(titles);

    // Two Columns: Left (User Accounts Table), Right (Create User Form)
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // Table Column
    QVBoxLayout* leftCol = new QVBoxLayout();
    QLabel* tableTitle = new QLabel("Registered System Accounts", this);
    tableTitle->setObjectName("SectionHeader");
    leftCol->addWidget(tableTitle);

    table_ = new QTableWidget(this);
    table_->setColumnCount(3);
    table_->setHorizontalHeaderLabels({"User ID", "Username", "Role Permissions"});
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->verticalHeader()->setVisible(false);
    leftCol->addWidget(table_);
    contentLayout->addLayout(leftCol, 3);

    // Form Column
    QFrame* formCard = new QFrame(this);
    formCard->setObjectName("ContentCard");
    QVBoxLayout* formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->setContentsMargins(18, 18, 18, 18);
    formCardLayout->setSpacing(14);

    QLabel* formTitle = new QLabel("➕ Create New User Account", formCard);
    formTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #38bdf8;");
    formCardLayout->addWidget(formTitle);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    newUsernameInput_ = new QLineEdit(formCard);
    newUsernameInput_->setPlaceholderText("e.g. cashier2");
    form->addRow("Username:", newUsernameInput_);

    newPasswordInput_ = new QLineEdit(formCard);
    newPasswordInput_->setEchoMode(QLineEdit::Password);
    newPasswordInput_->setPlaceholderText("Password...");
    form->addRow("Password:", newPasswordInput_);

    newRoleCombo_ = new QComboBox(formCard);
    newRoleCombo_->addItem("Cashier", "cashier");
    newRoleCombo_->addItem("Admin", "admin");
    form->addRow("Role:", newRoleCombo_);

    formCardLayout->addLayout(form);

    createBtn_ = new QPushButton("Create Account", formCard);
    createBtn_->setObjectName("SuccessButton");
    connect(createBtn_, &QPushButton::clicked, this, &UsersWidget::handleCreateUser);
    formCardLayout->addWidget(createBtn_);

    QLabel* infoNote = new QLabel("Passwords are automatically salted and hashed using SHA-256 before storing into SQLite.", formCard);
    infoNote->setStyleSheet("color: #64748b; font-size: 11px; margin-top: 8px;");
    infoNote->setWordWrap(true);
    formCardLayout->addWidget(infoNote);

    formCardLayout->addStretch();
    contentLayout->addWidget(formCard, 2);

    mainLayout->addLayout(contentLayout);
}

void UsersWidget::refreshUsers() {
    try {
        auto users = auth_.getAllUsers(currentUser_);
        table_->setRowCount(static_cast<int>(users.size()));

        for (int i = 0; i < static_cast<int>(users.size()); ++i) {
            const auto& u = users[i];

            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(u.id));
            idItem->setTextAlignment(Qt::AlignCenter);
            table_->setItem(i, 0, idItem);

            QTableWidgetItem* userItem = new QTableWidgetItem(QString::fromStdString(u.username));
            table_->setItem(i, 1, userItem);

            QTableWidgetItem* roleItem = new QTableWidgetItem();
            roleItem->setTextAlignment(Qt::AlignCenter);
            if (u.role == Role::Admin) {
                roleItem->setText("ADMIN");
                roleItem->setForeground(QColor("#a855f7")); // Purple
            } else {
                roleItem->setText("CASHIER");
                roleItem->setForeground(QColor("#10b981")); // Emerald
            }
            table_->setItem(i, 2, roleItem);
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Failed to load users: %1").arg(e.what()));
    }
}

void UsersWidget::handleCreateUser() {
    QString user = newUsernameInput_->text().trimmed();
    QString pass = newPasswordInput_->text();
    Role role = (newRoleCombo_->currentData().toString() == "admin") ? Role::Admin : Role::Cashier;

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Username and password cannot be empty.");
        return;
    }

    try {
        auth_.createUser(user.toStdString(), pass.toStdString(), role);
        QMessageBox::information(this, "Success", QString("User '%1' successfully created!").arg(user));
        newUsernameInput_->clear();
        newPasswordInput_->clear();
        refreshUsers();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Creation Failed", QString("Failed to create user: %1").arg(e.what()));
    }
}

