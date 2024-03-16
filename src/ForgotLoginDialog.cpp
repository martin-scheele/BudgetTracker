#include "ForgotLoginDialog.h"
#include "ui_ForgotLoginDialog.h"

#include "LoginDatabaseManager.h"

/**
 * @brief ForgotLoginDialog::ForgotLoginDialog
 *        Sets up UI and connects signals & slots.
 * @param parent pointer to QWidget parent object
 */
ForgotLoginDialog::ForgotLoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgotLoginDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, &QAbstractButton::clicked,
            this, &ForgotLoginDialog::resetPassword);
    connect(ui->cancelButton, &QAbstractButton::clicked,
            this, &QDialog::reject);
    connect(ui->userIDLineEdit, &QLineEdit::textChanged,
            this, &ForgotLoginDialog::clearStatusLabel);
    connect(ui->newPasswordLineEdit, &QLineEdit::textChanged,
            this, &ForgotLoginDialog::clearStatusLabel);
    connect(ui->confirmPasswordLineEdit, &QLineEdit::textChanged,
            this, &ForgotLoginDialog::clearStatusLabel);
}

/**
 * @brief ForgotLoginDialog::~ForgotLoginDialog
 *        Deallocates UI memory.
 */
ForgotLoginDialog::~ForgotLoginDialog()
{
    delete ui;
}

/**
 * @brief ForgotLoginDialog::resetPassword
 *        Attempts to reset password with LoginDatabaseManager.
 *
 *        Accepts QDialog if passwords and userID are verified.
 *        Updates status label to reflect outcome.
 */
void ForgotLoginDialog::resetPassword()
{
    LoginDatabaseManager db;

    int userID = ui->userIDLineEdit->text().toInt();
    QString newPassword = ui->newPasswordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (verifyPassword(newPassword, confirmPassword)) {
        if (db.verifyUserID(userID)) {
            db.changePassword(userID, newPassword);
            QDialog::accept();
        } else {
            ui->statusLabel->setStyleSheet("color: red");
            ui->statusLabel->setText("userID does not exist");
        }
    }
}

/**
 * @brief ForgotLoginDialog::clearStatusLabel
 *        Clears status label text.
 */
void ForgotLoginDialog::clearStatusLabel()
{
    ui->statusLabel->clear();
}

/**
 * @brief ForgotLoginDialog::verifyPassword
 *        Verifies if password and confirmPassword match,
 *        and that password meets minimum length requirement.
 *
 *        Updates status label to indicate unmet criteria.
 *
 * @param password password to confirm
 * @param confirmPassword password to confirm against
 * @return true if criteria are met, false if not
 */
bool ForgotLoginDialog::verifyPassword(const QString &password, const QString &confirmPassword)
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
