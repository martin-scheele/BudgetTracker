#pragma once

#include <QString>

/**
 * @brief The User class
 *        Holds basic user data.
 *
 * Holds user information
 */
class User {

public:
    // constructor and destructor
    User();
    User(int userID, QString username, QString password);

    // getters
    int getUserID() const;
    QString getUsername() const;
    QString getPassword() const;

    // setters
    void setUserID(const int userID);
    void setUsername(const QString &username);
    void setPassword(const QString &password);

private:
    // data members
    int m_userID;
    QString m_username;
    QString m_password;
};
