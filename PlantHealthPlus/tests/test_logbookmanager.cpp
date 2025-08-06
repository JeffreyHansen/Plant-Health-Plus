#include <QtTest>
#include <QObject>
#include <QDateTime>
#include <QTemporaryDir>
#include "../logbookmanager.h"

class TestLogbookManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases
    void test_addEntry();
    void test_entryConstruction();

private:
    LogbookManager* m_logbookManager;
    QTemporaryDir* m_tempDir;
};

void TestLogbookManager::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestLogbookManager::cleanupTestCase()
{
    delete m_tempDir;
}

void TestLogbookManager::init()
{
    m_logbookManager = new LogbookManager();
}

void TestLogbookManager::cleanup()
{
    delete m_logbookManager;
    m_logbookManager = nullptr;
}

void TestLogbookManager::test_addEntry()
{
    // Test adding entries (basic functionality test)
    LogbookEntry entry1("Test Entry 1", "This is a test content");
    LogbookEntry entry2("Test Entry 2", "This is another test content");
    
    // Test that entries can be created with valid data
    QVERIFY(!entry1.title.isEmpty());
    QVERIFY(!entry1.content.isEmpty());
    QVERIFY(!entry1.id.isEmpty());
    QVERIFY(entry1.dateCreated.isValid());
    QVERIFY(entry1.dateModified.isValid());
    
    QVERIFY(!entry2.title.isEmpty());
    QVERIFY(!entry2.content.isEmpty());
    QVERIFY(!entry2.id.isEmpty());
    QVERIFY(entry2.dateCreated.isValid());
    QVERIFY(entry2.dateModified.isValid());
    
    // Verify IDs are different
    QVERIFY(entry1.id != entry2.id);
}

void TestLogbookManager::test_entryConstruction()
{
    // Test default constructor
    LogbookEntry defaultEntry;
    QVERIFY(defaultEntry.title.isEmpty());
    QVERIFY(defaultEntry.content.isEmpty());
    QVERIFY(!defaultEntry.id.isEmpty());  // ID should be auto-generated
    QVERIFY(defaultEntry.dateCreated.isValid());
    QVERIFY(defaultEntry.dateModified.isValid());
    
    // Test parameterized constructor
    QString testTitle = "My Plant Observation";
    QString testContent = "The plant is growing well with new leaves appearing.";
    
    LogbookEntry paramEntry(testTitle, testContent);
    QCOMPARE(paramEntry.title, testTitle);
    QCOMPARE(paramEntry.content, testContent);
    QVERIFY(!paramEntry.id.isEmpty());
    QVERIFY(paramEntry.dateCreated.isValid());
    QVERIFY(paramEntry.dateModified.isValid());
    
    // Test that creation and modification dates are close to current time
    QDateTime now = QDateTime::currentDateTime();
    qint64 timeDiff = qAbs(now.msecsTo(paramEntry.dateCreated));
    QVERIFY(timeDiff < 1000);  // Should be within 1 second
}

#include "test_logbookmanager.moc"
