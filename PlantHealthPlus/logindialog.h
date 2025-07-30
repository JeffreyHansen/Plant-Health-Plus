#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    
    QString getUsername() const { return m_username; }
    QString getPassword() const { return m_password; }
    bool isNewUser() const { return m_isNewUser; }

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void togglePasswordVisibility();

private:
    void setupUI();
    bool validateInput();
    
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_registerButton;
    QPushButton* m_togglePasswordButton;
    QLabel* m_statusLabel;
    QLabel* m_confirmLabel;
    
    QString m_username;
    QString m_password;
    bool m_isNewUser;
    bool m_isRegisterMode;
};

#endif // LOGINDIALOG_H
