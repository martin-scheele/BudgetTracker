#include "LoginDatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

/**
 * @brief LoginDatabaseManager::LoginDatabaseManager
 *        Opens SQLite database connection and creates login table.
 */
LoginDatabaseManager::LoginDatabaseManager()
{
    openDatabase();
    createTable();
}

/**
 * @brief LoginDatabaseManager::~LoginDatabaseManager
 *        Closes SQLite database connection.
 */
LoginDatabaseManager::~LoginDatabaseManager()
{
    closeDatabase();
}

/**
 * @brief LoginDatabaseManager::openDatabase
 *        Opens new SQLite database connection.
 *
 * Creates a login table in a program directory in AppData.
 */
void LoginDatabaseManager::openDatabase()
{
    m_database = new QSqlDatabase;
    *m_database = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(path))
        dir.mkpath(path);
    m_database->setDatabaseName(path + QDir::separator() + "login.sqlite");
    m_database->open();
}

/**
 * @brief LoginDatabaseManager::closeDatabase
 *        Closes SQLite database connection.
 *
 * Deallocates memory.
 */
void LoginDatabaseManager::closeDatabase()
{
    m_database->close();
    delete m_database;
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

/**
 * @brief LoginDatabaseManager::createTable
 *        Creates login table.
 *
 * Inserts default admin user into table.
 */
void LoginDatabaseManager::createTable()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS user ("
                  "userID INTEGER PRIMARY KEY, "
                  "username VARCHAR(20), "
                  "password VARCHAR(20))");
    query.exec();
    query.prepare("INSERT INTO user "
                  "(userID, username, password) "
                  "VALUES (?, ?, ?)");
    query.bindValue(0, 0);
    query.bindValue(1, "admin");
    query.bindValue(2, "admin");
    query.exec();
}

/**
 * @brief LoginDatabaseManager::loginUser
 *        Attemps to login user.
 * @param username user's username
 * @param password user's password
 * @return shared_ptr to user object; nullptr if unsuccessful
 */
std::shared_ptr<User> LoginDatabaseManager::loginUser(const QString& username, const QString& password)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*), userID "
                  "FROM user "
                  "WHERE username = ? "
                  "AND password = ?");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.exec();

    int match = 0;
    int userID = 0;
    if (query.next()) {
        match = query.value(0).toInt();
        userID = query.value(1).toInt();
    }

    if (match > 0) {
        return std::make_shared<User>(userID, username, password);
    }
    else {
        return nullptr;
    }
}

/**
 * @brief LoginDatabaseManager::verifyUsername
 *        Verifies if username already exists in database.
 * @param username username to verify
 * @return true if found, false if not
 */
bool LoginDatabaseManager::verifyUsername(const QString& username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) "
                  "FROM user "
                  "WHERE username = ?");
    query.bindValue(0, username);
    query.exec();

    int match = 0;
    if (query.next()) {
        match = query.value(0).toInt();
    }
    if (match > 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief LoginDatabaseManager::registerUser
 *        Registers new user information into database.
 * @param username new user's username
 * @param password new user's password
 */
void LoginDatabaseManager::registerUser(const QString& username, const QString &password)
{
    QSqlQuery query;
    query.prepare("INSERT INTO user "
                  "(userID, username, password) "
                  "VALUES (NULL, ?, ?)");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.exec();
}

/**
 * @brief LoginDatabaseManager::verifyUserID
 *        Verifies if userID already exists in database.
 * @param userID userID to verify
 * @return true if found, false if not
 */
bool LoginDatabaseManager::verifyUserID(const int userID)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) "
                  "FROM user "
                  "WHERE userID = ?");
    query.bindValue(0, userID);
    query.exec();

    int match = 0;
    if (query.next()) {
        match = query.value(0).toInt();
    }
    if (match > 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief LoginDatabaseManager::changePassword
 *        Changes existing user's password.
 * @param userID userID of user changing their password
 * @param newPassword user's new password
 */
void LoginDatabaseManager::changePassword(const int userID, const QString& newPassword)
{
    QSqlQuery query;
    query.prepare("UPDATE user "
                  "SET password = ? "
                  "WHERE userID = ?");
    query.bindValue(0, newPassword);
    query.bindValue(1, userID);
    query.exec();
}
