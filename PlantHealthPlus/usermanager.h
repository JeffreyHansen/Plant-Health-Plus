#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QRandomGenerator>

class UserManager
{
public:
    UserManager();
    
    // User authentication
    bool createUser(const QString& username, const QString& password);
    bool authenticateUser(const QString& username, const QString& password);
    bool userExists(const QString& username);
    
    // User data management
    QString getCurrentUser() const { return m_currentUser; }
    void setCurrentUser(const QString& username) { m_currentUser = username; }
    void logout() { m_currentUser.clear(); }
    
    // Get user-specific file paths
    QString getUserPlantsFilePath(const QString& username) const;
    QString getUserPlantsFilePath() const; // For current user
    
    // User settings
    int getUserVolume(const QString& username) const;
    int getUserVolume() const; // For current user
    void setUserVolume(const QString& username, int volume);
    void setUserVolume(int volume); // For current user
    
private:
    QString hashPassword(const QString& password, const QString& salt) const;
    QString generateSalt() const;
    bool saveUserData();
    bool loadUserData();
    QString getUsersFilePath() const;
    
    QJsonObject m_userData;
    QString m_currentUser;
    
    static constexpr int SALT_LENGTH = 16;
    static constexpr int HASH_ITERATIONS = 10000;
};

#endif // USERMANAGER_H
