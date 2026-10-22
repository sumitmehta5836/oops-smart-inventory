#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "Auth.h"
#include "models.h"

// OOP CONCEPT: INHERITANCE
// UsersWidget inherits from QWidget to provide role-based user account provisioning.
// Base Class: QWidget | Derived Class: UsersWidget
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates the user table, new account input forms, and the Auth service reference.
class UsersWidget : public QWidget {
    Q_OBJECT

public:
    explicit UsersWidget(Auth& auth, const User& currentUser, QWidget* parent = nullptr);
    void refreshUsers();

private slots:
    void handleCreateUser();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    Auth& auth_;
    User currentUser_;

    QTableWidget* table_;
    QLineEdit* newUsernameInput_;
    QLineEdit* newPasswordInput_;
    QComboBox* newRoleCombo_;
    QPushButton* createBtn_;

    void setupUi();
};

