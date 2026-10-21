#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "Auth.h"
#include "models.h"

class UsersWidget : public QWidget {
    Q_OBJECT

public:
    explicit UsersWidget(Auth& auth, const User& currentUser, QWidget* parent = nullptr);
    void refreshUsers();

private slots:
    void handleCreateUser();

private:
    Auth& auth_;
    User currentUser_;

    QTableWidget* table_;
    QLineEdit* newUsernameInput_;
    QLineEdit* newPasswordInput_;
    QComboBox* newRoleCombo_;
    QPushButton* createBtn_;

    void setupUi();
};

