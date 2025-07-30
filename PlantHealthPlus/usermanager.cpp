#include "usermanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>

UserManager::UserManager()
{
    loadUserData();
}

bool UserManager::createUser(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }
    
    // Check if user already exists
    if (userExists(username)) {
        return false;
    }
    
    // Generate salt and hash password
    QString salt = generateSalt();
    QString hashedPassword = hashPassword(password, salt);
    
    // Create user entry
    QJsonObject userEntry;
    userEntry["passwordHash"] = hashedPassword;
    userEntry["salt"] = salt;
    userEntry["createdDate"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Add to user data
    m_userData[username] = userEntry;
    
    // Save to file
    return saveUserData();
}

bool UserManager::authenticateUser(const QString& username, const QString& password)
{
    if (!userExists(username)) {
        return false;
    }
    
    QJsonObject userEntry = m_userData[username].toObject();
    QString storedHash = userEntry["passwordHash"].toString();
    QString salt = userEntry["salt"].toString();
    
    QString inputHash = hashPassword(password, salt);
    
    if (inputHash == storedHash) {
        m_currentUser = username;
        return true;
    }
    
    return false;
}

bool UserManager::userExists(const QString& username)
{
    return m_userData.contains(username);
}

QString UserManager::getUserPlantsFilePath(const QString& username) const
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(appDataPath);
    }
    
    return appDataPath + "/plants_" + username + ".json";
}

QString UserManager::getUserPlantsFilePath() const
{
    if (m_currentUser.isEmpty()) {
        return QString();
    }
    return getUserPlantsFilePath(m_currentUser);
}

QString UserManager::hashPassword(const QString& password, const QString& salt) const
{
    QString saltedPassword = password + salt;
    QByteArray hash = saltedPassword.toUtf8();
    
    // Apply multiple rounds of hashing for security
    for (int i = 0; i < HASH_ITERATIONS; ++i) {
        hash = QCryptographicHash::hash(hash, QCryptographicHash::Sha256);
    }
    
    return QString::fromLatin1(hash.toHex());
}

QString UserManager::generateSalt() const
{
    QString salt;
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    for (int i = 0; i < SALT_LENGTH; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        salt.append(chars.at(index));
    }
    
    return salt;
}

bool UserManager::saveUserData()
{
    QString filePath = getUsersFilePath();
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open users file for writing:" << filePath;
        return false;
    }
    
    QJsonDocument doc(m_userData);
    file.write(doc.toJson());
    file.close();
    
    return true;
}

bool UserManager::loadUserData()
{
    QString filePath = getUsersFilePath();
    QFile file(filePath);
    
    if (!file.exists()) {
        // File doesn't exist yet, that's okay
        m_userData = QJsonObject();
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open users file for reading:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error in users file:" << error.errorString();
        return false;
    }
    
    m_userData = doc.object();
    return true;
}

QString UserManager::getUsersFilePath() const
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(appDataPath);
    }
    
    return appDataPath + "/users.json";
}

int UserManager::getUserVolume(const QString& username) const
{
    if (m_userData.contains(username)) {
        QJsonObject userObj = m_userData[username].toObject();
        return userObj.value("volume").toInt(50); // Default to 50%
    }
    return 50; // Default volume
}

int UserManager::getUserVolume() const
{
    if (m_currentUser.isEmpty()) {
        return 50; // Default volume
    }
    return getUserVolume(m_currentUser);
}

void UserManager::setUserVolume(const QString& username, int volume)
{
    if (m_userData.contains(username)) {
        QJsonObject userObj = m_userData[username].toObject();
        userObj["volume"] = volume;
        m_userData[username] = userObj;
        saveUserData();
    }
}

void UserManager::setUserVolume(int volume)
{
    if (!m_currentUser.isEmpty()) {
        setUserVolume(m_currentUser, volume);
    }
}
