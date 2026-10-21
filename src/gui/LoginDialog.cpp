#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

LoginDialog::LoginDialog(Auth& auth, QWidget* parent)
    : QDialog(parent), auth_(auth) {
    setWindowTitle("Inventory Management - Login");
    setFixedSize(520, 580);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 28, 28, 28);

    // Main Card Frame
    QFrame* card = new QFrame(this);
    card->setObjectName("ContentCard");
    card->setStyleSheet(R"(
        #ContentCard {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 12px;
            padding: 10px;
        }
    )");

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(32, 32, 32, 32);
    cardLayout->setSpacing(16);

    // Brand Header
    QLabel* brandIcon = new QLabel("📦", card);
    brandIcon->setAlignment(Qt::AlignCenter);
    brandIcon->setStyleSheet("font-size: 48px; margin-bottom: 2px;");
    cardLayout->addWidget(brandIcon);

    QLabel* titleLabel = new QLabel("Inventory Management", card);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: 800; color: #ffffff; letter-spacing: 0.5px;");
    cardLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("Sign in to access your inventory and POS terminal", card);
    subtitleLabel->setObjectName("SubtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #94a3b8; font-size: 14px; margin-bottom: 8px;");
    cardLayout->addWidget(subtitleLabel);

    // Separator line
    QFrame* hLine = new QFrame(card);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setStyleSheet("background-color: #334155; max-height: 1px; margin-bottom: 8px;");
    cardLayout->addWidget(hLine);

    // Username Section
    QLabel* userLbl = new QLabel("👤  Username", card);
    userLbl->setStyleSheet("font-size: 14px; font-weight: 700; color: #f1f5f9;");
    cardLayout->addWidget(userLbl);

    usernameInput_ = new QLineEdit(card);
    usernameInput_->setObjectName("LoginInput");
    usernameInput_->setPlaceholderText("Enter your username...");
    usernameInput_->setFixedHeight(48);
    cardLayout->addWidget(usernameInput_);

    cardLayout->addSpacing(4);

    // Password Section
    QLabel* passLbl = new QLabel("🔑  Password", card);
    passLbl->setStyleSheet("font-size: 14px; font-weight: 700; color: #f1f5f9;");
    cardLayout->addWidget(passLbl);

    QHBoxLayout* passContainer = new QHBoxLayout();
    passContainer->setSpacing(8);

    passwordInput_ = new QLineEdit(card);
    passwordInput_->setObjectName("LoginInput");
    passwordInput_->setEchoMode(QLineEdit::Password);
    passwordInput_->setPlaceholderText("Enter your password...");
    passwordInput_->setFixedHeight(48);
    passContainer->addWidget(passwordInput_, 1);

    togglePasswordBtn_ = new QPushButton("👁️", card);
    togglePasswordBtn_->setObjectName("SecondaryButton");
    togglePasswordBtn_->setToolTip("Show / Hide Password");
    togglePasswordBtn_->setFixedSize(48, 48);
    togglePasswordBtn_->setCursor(Qt::PointingHandCursor);
    togglePasswordBtn_->setStyleSheet("font-size: 18px; border-radius: 8px;");
    passContainer->addWidget(togglePasswordBtn_);

    cardLayout->addLayout(passContainer);

    // Error Alert Box
    errorLabel_ = new QLabel("", card);
    errorLabel_->setStyleSheet(R"(
        background-color: rgba(239, 68, 68, 0.15);
        color: #f87171;
        border: 1px solid #ef4444;
        border-radius: 6px;
        padding: 8px 12px;
        font-size: 13px;
        font-weight: 600;
    )");
    errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setVisible(false);
    cardLayout->addWidget(errorLabel_);

    cardLayout->addSpacing(10);

    // Sign In Button
    loginBtn_ = new QPushButton("Sign In to System", card);
    loginBtn_->setObjectName("SuccessButton");
    loginBtn_->setCursor(Qt::PointingHandCursor);
    loginBtn_->setFixedHeight(50);
    loginBtn_->setStyleSheet("font-size: 16px; font-weight: bold; border-radius: 8px;");
    cardLayout->addWidget(loginBtn_);

    mainLayout->addWidget(card);

    // Connections
    connect(loginBtn_, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(usernameInput_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
    connect(passwordInput_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
    connect(togglePasswordBtn_, &QPushButton::clicked, this, &LoginDialog::togglePasswordVisibility);
}

void LoginDialog::togglePasswordVisibility() {
    if (passwordInput_->echoMode() == QLineEdit::Password) {
        passwordInput_->setEchoMode(QLineEdit::Normal);
        togglePasswordBtn_->setText("🙈");
    } else {
        passwordInput_->setEchoMode(QLineEdit::Password);
        togglePasswordBtn_->setText("👁️");
    }
}

void LoginDialog::handleLogin() {
    std::string user = usernameInput_->text().trimmed().toStdString();
    std::string pass = passwordInput_->text().toStdString();

    if (user.empty() || pass.empty()) {
        errorLabel_->setText("Please enter both username and password.");
        errorLabel_->setVisible(true);
        return;
    }

    try {
        auto res = auth_.login(user, pass);
        if (res.has_value()) {
            authenticatedUser_ = res.value();
            accept();
        } else {
            errorLabel_->setText("Invalid username or password. Please try again.");
            errorLabel_->setVisible(true);
        }
    } catch (const std::exception& e) {
        errorLabel_->setText(QString("Error: %1").arg(e.what()));
        errorLabel_->setVisible(true);
    }
}
