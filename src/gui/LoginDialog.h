#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "Auth.h"
#include "models.h"

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(Auth& auth, QWidget* parent = nullptr);
    std::optional<User> getAuthenticatedUser() const { return authenticatedUser_; }

private slots:
    void handleLogin();
    void togglePasswordVisibility();

private:
    Auth& auth_;
    std::optional<User> authenticatedUser_;

    QLineEdit* usernameInput_;
    QLineEdit* passwordInput_;
    QPushButton* togglePasswordBtn_;
    QPushButton* loginBtn_;
    QLabel* errorLabel_;
};
