#include "LoginDialog.h"
#include "BudgetTracker.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginDialog login;
    login.show();
    if (login.exec() == QDialog::Accepted) {
        BudgetTracker tracker(login.user());
        tracker.show();
        return app.exec();
    }
    else
        return 0;
}
