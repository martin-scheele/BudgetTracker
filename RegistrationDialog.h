#pragma once

#include <QDialog>

namespace Ui {
class RegistrationDialog;
}

/**
 * @brief The RegistrationDialog class
 *        Manages user registration.
 *
 *        Used in LoginDialog to manage new user registration.
 */
class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    // constructor and destructor
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

private slots:
    void registration();
    void clearStatusLabel();

private:
    Ui::RegistrationDialog *ui;
    bool verifyPassword(const QString &password, const QString &confirmPassword);
};

