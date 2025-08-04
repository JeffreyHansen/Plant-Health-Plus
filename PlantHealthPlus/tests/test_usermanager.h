#ifndef TEST_USERMANAGER_H
#define TEST_USERMANAGER_H

#include <QtTest/QtTest>
#include <QObject>
#include <QTemporaryDir>
#include "../usermanager.h"

class TestUserManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testCreateUser();
    void testAuthenticateUser();
    void testUserExists();
    void testInvalidCredentials();
    void testEmptyCredentials();
    void testUserVolume();
    void testMultipleUsers();
    void testPasswordSecurity();
    void testUserDataPersistence();
    void testSpecialCharacters();

private:
    UserManager* m_userManager;
    QTemporaryDir* m_tempDir;
};

void TestUserManager::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestUserManager::cleanupTestCase()
{
    delete m_tempDir;
}

void TestUserManager::init()
{
    m_userManager = new UserManager();
}

void TestUserManager::cleanup()
{
    delete m_userManager;
}

void TestUserManager::testCreateUser()
{
    bool created = m_userManager->createUser("testuser", "password123");
    QVERIFY(created);
    
    bool exists = m_userManager->userExists("testuser");
    QVERIFY(exists);
}

void TestUserManager::testAuthenticateUser()
{
    m_userManager->createUser("testuser", "password123");
    
    bool authenticated = m_userManager->authenticateUser("testuser", "password123");
    QVERIFY(authenticated);
    
    bool wrongPassword = m_userManager->authenticateUser("testuser", "wrongpassword");
    QVERIFY(!wrongPassword);
}

void TestUserManager::testUserExists()
{
    QVERIFY(!m_userManager->userExists("nonexistent"));
    
    m_userManager->createUser("testuser", "password123");
    QVERIFY(m_userManager->userExists("testuser"));
    QVERIFY(!m_userManager->userExists("anotheruser"));
}

void TestUserManager::testInvalidCredentials()
{
    bool authenticated = m_userManager->authenticateUser("nonexistent", "password");
    QVERIFY(!authenticated);
}

void TestUserManager::testEmptyCredentials()
{
    bool created = m_userManager->createUser("", "password");
    QVERIFY(!created);
    
    created = m_userManager->createUser("user", "");
    QVERIFY(!created);
    
    bool authenticated = m_userManager->authenticateUser("", "password");
    QVERIFY(!authenticated);
    
    authenticated = m_userManager->authenticateUser("user", "");
    QVERIFY(!authenticated);
}

void TestUserManager::testUserVolume()
{
    m_userManager->createUser("testuser", "password123");
    
    // Test default volume
    int defaultVolume = m_userManager->getUserVolume();
    QVERIFY(defaultVolume >= 0 && defaultVolume <= 100);
    
    // Test setting volume
    m_userManager->setUserVolume(75);
    int newVolume = m_userManager->getUserVolume();
    QCOMPARE(newVolume, 75);
    
    // Test invalid volume values
    m_userManager->setUserVolume(-10);
    int volume = m_userManager->getUserVolume();
    QVERIFY(volume >= 0);
    
    m_userManager->setUserVolume(150);
    volume = m_userManager->getUserVolume();
    QVERIFY(volume <= 100);
}

void TestUserManager::testMultipleUsers()
{
    bool user1Created = m_userManager->createUser("user1", "pass1");
    bool user2Created = m_userManager->createUser("user2", "pass2");
    
    QVERIFY(user1Created);
    QVERIFY(user2Created);
    
    QVERIFY(m_userManager->userExists("user1"));
    QVERIFY(m_userManager->userExists("user2"));
    
    QVERIFY(m_userManager->authenticateUser("user1", "pass1"));
    QVERIFY(m_userManager->authenticateUser("user2", "pass2"));
    
    QVERIFY(!m_userManager->authenticateUser("user1", "pass2"));
    QVERIFY(!m_userManager->authenticateUser("user2", "pass1"));
}

void TestUserManager::testPasswordSecurity()
{
    m_userManager->createUser("testuser", "password123");
    
    // Password should not be stored in plain text
    // This is a basic check - in real implementation, 
    // passwords should be hashed
    bool authenticated = m_userManager->authenticateUser("testuser", "password123");
    QVERIFY(authenticated);
    
    // Case sensitivity
    bool wrongCase = m_userManager->authenticateUser("testuser", "Password123");
    QVERIFY(!wrongCase);
}

void TestUserManager::testUserDataPersistence()
{
    m_userManager->createUser("persistentuser", "password123");
    m_userManager->setUserVolume(80);
    
    // Create a new user manager instance to test persistence
    UserManager newManager;
    
    // Check if user still exists
    bool exists = newManager.userExists("persistentuser");
    QVERIFY(exists);
    
    // Check if authentication still works
    bool authenticated = newManager.authenticateUser("persistentuser", "password123");
    QVERIFY(authenticated);
}

void TestUserManager::testSpecialCharacters()
{
    // Test usernames and passwords with special characters
    bool created = m_userManager->createUser("user@test.com", "p@ssw0rd!");
    QVERIFY(created);
    
    bool authenticated = m_userManager->authenticateUser("user@test.com", "p@ssw0rd!");
    QVERIFY(authenticated);
    
    // Test unicode characters
    bool unicodeCreated = m_userManager->createUser("üser", "pässwörd");
    QVERIFY(unicodeCreated);
    
    bool unicodeAuth = m_userManager->authenticateUser("üser", "pässwörd");
    QVERIFY(unicodeAuth);
}

#endif // TEST_USERMANAGER_H
