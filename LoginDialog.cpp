#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "LoginDatabaseManager.h"
#include "RegistrationDialog.h"
#include "ForgotLoginDialog.h"

/**
 * @brief LoginDialog::LoginDialog
 *        Sets up UI and connects signals & slots.
 * @param parent pointer to QWidget parent object
 */
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginDialog::login);
    connect(ui->usernameLineEdit, &QLineEdit::textChanged,
            this, &LoginDialog::clearStatusLabel);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged,
            this, &LoginDialog::clearStatusLabel);
    connect(ui->registrationButton, &QPushButton::clicked,
            this, &LoginDialog::registration);
    connect(ui->forgotLoginButton, &QPushButton::clicked,
            this, &LoginDialog::forgotLogin);
}

/**
 * @brief LoginDialog::~LoginDialog
 *        Deallocates UI memory.
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * @brief LoginDialog::user
 *        Getter for "current" user, i.e. logged-in user.
 * @return shared_ptr to user object
 */
std::shared_ptr<User> LoginDialog::user()
{
    return m_currentUser;
}

/**
 * @brief LoginDialog::login
 *        Attemps to login, accepting QDialog if successful.
 */
void LoginDialog::login()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    LoginDatabaseManager db;
    // attempt login
    m_currentUser = db.loginUser(username, password);
    if (m_currentUser == nullptr) {
        ui->statusLabel->setStyleSheet("color: red");
        ui->statusLabel->setText("Login failed");
    } else {
        QDialog::accept();
    }
}

/**
 * @brief LoginDialog::registration
 *        Registers new user with RegistraionDialog
 */
void LoginDialog::registration()
{
    RegistrationDialog rDialog;
    if (rDialog.exec() == QDialog::Accepted) {
        ui->statusLabel->setStyleSheet("color: green");
        ui->statusLabel->setText("Registered successfully");
        ui->usernameLineEdit->setFocus();
    }
}

/**
 * @brief LoginDialog::forgotLogin
 *        Resets user password with ForgotLoginDialog
 */
void LoginDialog::forgotLogin()
{
    ForgotLoginDialog fDialog;
    if (fDialog.exec() == QDialog::Accepted) {
        ui->statusLabel->setStyleSheet("color: green");
        ui->statusLabel->setText("Password reset successfully");
        ui->usernameLineEdit->setFocus();
    }
}

/**
 * @brief LoginDialog::clearStatusLabel
 *        Clears status label text.
 */
void LoginDialog::clearStatusLabel()
{
    ui->statusLabel->clear();
}
