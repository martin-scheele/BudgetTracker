#pragma once
#include "User.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

/**
 * @brief The LoginDialog class
 *        Manages user login for BudgetTracker.
 *
 *        Used in main function to specify user for BudgetTracker.
 */
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    // constructor and destructor
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // getter
    std::shared_ptr<User> user();

private slots:
    void login();
    void registration();
    void forgotLogin();
    void clearStatusLabel();

private:
    Ui::LoginDialog *ui;
    std::shared_ptr<User> m_currentUser;  // current user to pass to BudgetTracker
};
