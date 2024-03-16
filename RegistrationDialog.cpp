#include "RegistrationDialog.h"
#include "ui_RegistrationDialog.h"

#include "LoginDatabaseManager.h"
#include <QSqlQuery>

/**
 * @brief RegistrationDialog::RegistrationDialog
 *        Sets up UI and connects signals & slots
 * @param parent pointer to QWidget parent object
 */
RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, &QAbstractButton::clicked,
            this, &RegistrationDialog::registration);
    connect(ui->cancelButton, &QAbstractButton::clicked,
            this, &QDialog::reject);
    connect(ui->usernameLineEdit, &QLineEdit::textChanged,
            this, &RegistrationDialog::clearStatusLabel);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged,
            this, &RegistrationDialog::clearStatusLabel);
    connect(ui->confirmPasswordLineEdit, &QLineEdit::textChanged,
            this, &RegistrationDialog::clearStatusLabel);
}

/**
 * @brief RegistrationDialog::~RegistrationDialog
 *        Deallocates UI memory.
 */
RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

/**
 * @brief RegistrationDialog::registration
 *        Registers new user with LoginDatabaseManager.
 *
 *        Accepts QDialog if password is verified and username
 *        does not already exist. Updates status label if invalid.
 */
void RegistrationDialog::registration()
{
    LoginDatabaseManager db;

    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (verifyPassword(password, confirmPassword)) {
        if (!db.verifyUsername(username)) {
            db.registerUser(username, password);
            QDialog::accept();
        } else {
            ui->statusLabel->setStyleSheet("color: red");
            ui->statusLabel->setText("Username already exists");
        }
    }
}

/**
 * @brief RegistrationDialog::clearStatusLabel
 *        Clears status label text.
 */
void RegistrationDialog::clearStatusLabel()
{
    ui->statusLabel->clear();
}

/**
 * @brief RegistrationDialog::verifyPassword
 *        Verifies if password and confirmPassword match,
 *        and that password meets minimum length requirement.
 *
 *        Updates status label to indicate unmet criteria.
 *
 * @param password password to confirm
 * @param confirmPassword password to confirm against
 * @return true if criteria are met, false if not
 */
bool RegistrationDialog::verifyPassword(const QString &password, const QString &confirmPassword)
{
    if (password.length() < 5) {
        ui->statusLabel->setStyleSheet("color: red");
        ui->statusLabel->setText("Password must be >8 characters");
        return false;
    } else if (password != confirmPassword) {
        ui->statusLabel->setStyleSheet("color: red");
        ui->statusLabel->setText("Passwords do not match");
        return false;
    } else {
        return true;
    }
}
