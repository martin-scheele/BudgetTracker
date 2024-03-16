#include "User.h"

/**
 * @brief User::User
 *        Default constructor.
 */
User::User() {}

/**
 * @brief User::User
 *        All-member constructor.
 * @param userID user's ID number
 * @param username user's username
 * @param password user's password
 */
User::User(int userID, QString username, QString password) :
    m_userID{userID}, m_username{username}, m_password{password} {}

/**
 * @brief User::getUserID
 *        Getter for userID.
 * @return int userID
 */
int User::getUserID() const
{
    return m_userID;
}

/**
 * @brief User::getUsername
 *        Getter for username.
 * @return QString username
 */
QString User::getUsername() const
{
    return m_username;
}

/**
 * @brief User::getPassword
 *        Getter for password.
 * @return QString password
 */
QString User::getPassword() const
{
    return m_password;
}

/**
 * @brief User::setUserID
 *        Setter for userID.
 * @param userID user's new ID number
 */
void User::setUserID(int userID)
{
    m_userID = userID;
}

/**
 * @brief User::setUsername
 *        Setter for username.
 * @param username user's new username
 */
void User::setUsername(const QString &username)
{
    m_username = username;
}

/**
 * @brief User::setPassword
 *        Setter for password.
 * @param password user's new password
 */
void User::setPassword(const QString &password)
{
    m_password = password;
}
