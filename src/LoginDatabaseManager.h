#pragma once

#include "User.h"

#include <QSqlDatabase>

/**
 * @brief The LoginDatabaseManager class
 *        SQLite login database manager.
 *
 *        Used in LoginDialog, RegistrationDialog, and ForgotLoginDialog
 *        to manage SQLite queries on login database.
 */
class LoginDatabaseManager {
private:
    QSqlDatabase *m_database;

    // database management functions
    void openDatabase();
    void closeDatabase();
    void createTable();

public:
    // constructor and destructor
    LoginDatabaseManager();
    ~LoginDatabaseManager();

    // SQLite query functions
    std::shared_ptr<User> loginUser(const QString& username, const QString& password);
    bool verifyUsername(const QString& username);
    void registerUser(const QString& username, const QString &password);
    bool verifyUserID(const int userID);
    void changePassword(const int userID, const QString& newPassword);
};
