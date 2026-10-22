#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "Auth.h"
#include "models.h"

// OOP CONCEPT: INHERITANCE
// LoginDialog inherits from the Qt base class QDialog.
// Base Class: QDialog (manages modal dialog lifecycle, event loop exec(), and accept/reject states)
// Derived Class: LoginDialog (specializes QDialog with credentials input fields, error styling, and auth validation)
//
// OOP CONCEPT: ENCAPSULATION
// Encapsulates UI components and sensitive login state behind private access modifiers.
class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(Auth& auth, QWidget* parent = nullptr);

    // OOP CONCEPT: ENCAPSULATION (CONTROLLED ACCESSOR / GETTER)
    // Exposes a read-only getter to retrieve the authenticated user session safely.
    std::optional<User> getAuthenticatedUser() const { return authenticatedUser_; }

private slots:
    void handleLogin();
    void togglePasswordVisibility();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    // Internal widget pointers and user session storage are private to preserve dialog integrity.
    Auth& auth_;
    std::optional<User> authenticatedUser_;

    QLineEdit* usernameInput_;
    QLineEdit* passwordInput_;
    QPushButton* togglePasswordBtn_;
    QPushButton* loginBtn_;
    QLabel* errorLabel_;
};
