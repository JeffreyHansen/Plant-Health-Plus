#include "logindialog.h"
#include "usermanager.h"
#include <QMessageBox>
#include <QGroupBox>
#include <QApplication>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , m_isNewUser(false)
    , m_isRegisterMode(false)
{
    setupUI();
    setWindowTitle("Plant Health+ Login");
    setModal(true);
    setFixedSize(350, 300);
}

void LoginDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Plant Health+");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    color: #2E7D32;"
        "    margin: 20px 0px;"
        "}"
    );
    
    // Login form
    QGroupBox* formGroup = new QGroupBox("Login");
    QFormLayout* formLayout = new QFormLayout(formGroup);
    
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Enter username...");
    formLayout->addRow("Username:", m_usernameEdit);
    
    // Password field with toggle visibility
    QHBoxLayout* passwordLayout = new QHBoxLayout();
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Enter password...");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    
    m_togglePasswordButton = new QPushButton("👁");
    m_togglePasswordButton->setFixedSize(30, 30);
    m_togglePasswordButton->setStyleSheet(
        "QPushButton {"
        "    border: 1px solid #ccc;"
        "    border-radius: 4px;"
        "    background-color: #f9f9f9;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e9e9e9;"
        "}"
    );
    
    passwordLayout->addWidget(m_passwordEdit);
    passwordLayout->addWidget(m_togglePasswordButton);
    formLayout->addRow("Password:", passwordLayout);
    
    // Confirm password (initially hidden)
    m_confirmLabel = new QLabel("Confirm Password:");
    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setPlaceholderText("Confirm password...");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setVisible(false);
    m_confirmLabel->setVisible(false);
    formLayout->addRow(m_confirmLabel, m_confirmPasswordEdit);
    
    // Status label
    m_statusLabel = new QLabel();
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    m_statusLabel->setVisible(false);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_loginButton = new QPushButton("Login");
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
    );
    
    m_registerButton = new QPushButton("Register New User");
    m_registerButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
    );
    
    buttonLayout->addWidget(m_registerButton);
    buttonLayout->addWidget(m_loginButton);
    
    // Add everything to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    connect(m_togglePasswordButton, &QPushButton::clicked, this, &LoginDialog::togglePasswordVisibility);
    
    // Set focus and enter key handling
    m_usernameEdit->setFocus();
    connect(m_usernameEdit, &QLineEdit::returnPressed, m_passwordEdit, QOverload<>::of(&QLineEdit::setFocus));
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(m_confirmPasswordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::onLoginClicked()
{
    if (!validateInput()) {
        return;
    }
    
    m_username = m_usernameEdit->text().trimmed();
    m_password = m_passwordEdit->text();
    
    UserManager userManager;
    
    if (m_isRegisterMode) {
        // Register new user
        if (userManager.createUser(m_username, m_password)) {
            m_isNewUser = true;
            accept();
        } else {
            m_statusLabel->setText("Username already exists!");
            m_statusLabel->setVisible(true);
        }
    } else {
        // Login existing user
        if (userManager.authenticateUser(m_username, m_password)) {
            m_isNewUser = false;
            accept();
        } else {
            m_statusLabel->setText("Invalid username or password!");
            m_statusLabel->setVisible(true);
        }
    }
}

void LoginDialog::onRegisterClicked()
{
    m_isRegisterMode = !m_isRegisterMode;
    
    if (m_isRegisterMode) {
        // Switch to register mode
        setWindowTitle("Plant Health+ Registration");
        m_confirmPasswordEdit->setVisible(true);
        m_confirmLabel->setVisible(true);
        m_registerButton->setText("Back to Login");
        m_loginButton->setText("Register");
        setFixedSize(350, 350);
    } else {
        // Switch back to login mode
        setWindowTitle("Plant Health+ Login");
        m_confirmPasswordEdit->setVisible(false);
        m_confirmLabel->setVisible(false);
        m_registerButton->setText("Register New User");
        m_loginButton->setText("Login");
        setFixedSize(350, 300);
    }
    
    m_statusLabel->setVisible(false);
}

void LoginDialog::togglePasswordVisibility()
{
    if (m_passwordEdit->echoMode() == QLineEdit::Password) {
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
        m_confirmPasswordEdit->setEchoMode(QLineEdit::Normal);
        m_togglePasswordButton->setText("🙈");
    } else {
        m_passwordEdit->setEchoMode(QLineEdit::Password);
        m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
        m_togglePasswordButton->setText("👁");
    }
}

bool LoginDialog::validateInput()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty()) {
        m_statusLabel->setText("Please enter a username!");
        m_statusLabel->setVisible(true);
        m_usernameEdit->setFocus();
        return false;
    }
    
    if (username.length() < 3) {
        m_statusLabel->setText("Username must be at least 3 characters!");
        m_statusLabel->setVisible(true);
        m_usernameEdit->setFocus();
        return false;
    }
    
    if (password.isEmpty()) {
        m_statusLabel->setText("Please enter a password!");
        m_statusLabel->setVisible(true);
        m_passwordEdit->setFocus();
        return false;
    }
    
    if (password.length() < 4) {
        m_statusLabel->setText("Password must be at least 4 characters!");
        m_statusLabel->setVisible(true);
        m_passwordEdit->setFocus();
        return false;
    }
    
    if (m_isRegisterMode) {
        QString confirmPassword = m_confirmPasswordEdit->text();
        if (password != confirmPassword) {
            m_statusLabel->setText("Passwords do not match!");
            m_statusLabel->setVisible(true);
            m_confirmPasswordEdit->setFocus();
            return false;
        }
    }
    
    m_statusLabel->setVisible(false);
    return true;
}
