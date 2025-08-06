#include <QtTest>
#include <QObject>
#include "../usermanager.h"

class TestUserManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    // Test cases
    void test_createUser();
    void test_authenticateUser();
    void test_userExists();
    void test_userVolume();

private:
    UserManager* m_userManager;
};

void TestUserManager::init()
{
    m_userManager = new UserManager();
}

void TestUserManager::cleanup()
{
    delete m_userManager;
    m_userManager = nullptr;
}

void TestUserManager::test_createUser()
{
    QString username = "testuser123";
    QString password = "testpassword123";

    // Test creating a new user
    bool result = m_userManager->createUser(username, password);
    QVERIFY(result);

    // Verify user exists
    QVERIFY(m_userManager->userExists(username));

    // Test creating duplicate user
    bool duplicateResult = m_userManager->createUser(username, password);
    QVERIFY(!duplicateResult);
}

void TestUserManager::test_authenticateUser()
{
    QString username = "authtest";
    QString password = "authpass123";

    // Create user first
    m_userManager->createUser(username, password);

    // Test correct authentication
    bool authResult = m_userManager->authenticateUser(username, password);
    QVERIFY(authResult);

    // Test wrong password
    bool wrongPassResult = m_userManager->authenticateUser(username, "wrongpass");
    QVERIFY(!wrongPassResult);

    // Test non-existent user
    bool nonExistentResult = m_userManager->authenticateUser("nonexistent", password);
    QVERIFY(!nonExistentResult);
}

void TestUserManager::test_userExists()
{
    QString existingUser = "existinguser";
    QString nonExistentUser = "nonexistentuser";

    // Initially user should not exist
    QVERIFY(!m_userManager->userExists(existingUser));

    // Create user
    m_userManager->createUser(existingUser, "password");

    // Now user should exist
    QVERIFY(m_userManager->userExists(existingUser));

    // Non-existent user should still not exist
    QVERIFY(!m_userManager->userExists(nonExistentUser));
}

void TestUserManager::test_userVolume()
{
    QString username = "volumetest";
    m_userManager->createUser(username, "password");
    m_userManager->setCurrentUser(username);

    // Test setting and getting volume
    int testVolume = 75;
    m_userManager->setUserVolume(testVolume);
    
    int retrievedVolume = m_userManager->getUserVolume();
    QCOMPARE(retrievedVolume, testVolume);

    // Test with specific username
    int anotherVolume = 50;
    m_userManager->setUserVolume(username, anotherVolume);
    
    int retrievedSpecificVolume = m_userManager->getUserVolume(username);
    QCOMPARE(retrievedSpecificVolume, anotherVolume);
}

#include "test_usermanager.moc"
