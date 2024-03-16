#pragma once

#include <QDialog>

namespace Ui {
class ForgotLoginDialog;
}

/**
 * @brief The ForgotLoginDialog class
 *        Manages user password reset.
 *
 *        Used in LoginDialog to manage user password reset.
 */
class ForgotLoginDialog : public QDialog
{
    Q_OBJECT

public:
    // constructors
    explicit ForgotLoginDialog(QWidget *parent = nullptr);

    // destructors
    ~ForgotLoginDialog();

private slots:
    void resetPassword();
    void clearStatusLabel();

private:
    Ui::ForgotLoginDialog *ui;

    bool verifyPassword(const QString &newPassword, const QString &confirmPassword);
};
