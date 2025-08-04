#ifndef TEST_LOGBOOKMANAGER_H
#define TEST_LOGBOOKMANAGER_H

#include <QtTest/QtTest>
#include <QObject>
#include "../logbookmanager.h"

class TestLogbookManager : public QObject
{
    Q_OBJECT

private slots:
    void testAddEntry();
    void testGetEntries();
    void testUpdateEntry();
    void testDeleteEntry();

private:
    LogbookManager* m_manager = nullptr;
};

void TestLogbookManager::testAddEntry()
{
    m_manager = new LogbookManager();
    LogbookEntry entry;
    entry.title = "Test Entry";
    entry.content = "Test content";
    
    QString id = m_manager->addEntry(entry);
    QVERIFY(!id.isEmpty());
    
    delete m_manager;
}

void TestLogbookManager::testGetEntries()
{
    m_manager = new LogbookManager();
    QVector<LogbookEntry> entries = m_manager->getEntries();
    int initialCount = entries.size();
    
    LogbookEntry entry;
    entry.title = "Test Entry";
    m_manager->addEntry(entry);
    
    entries = m_manager->getEntries();
    QCOMPARE(entries.size(), initialCount + 1);
    
    delete m_manager;
}

void TestLogbookManager::testUpdateEntry()
{
    m_manager = new LogbookManager();
    LogbookEntry entry;
    entry.title = "Original Title";
    
    QString id = m_manager->addEntry(entry);
    LogbookEntry* found = m_manager->getEntryById(id);
    QVERIFY(found != nullptr);
    
    found->title = "Updated Title";
    bool updated = m_manager->updateEntry(*found);
    QVERIFY(updated);
    
    delete m_manager;
}

void TestLogbookManager::testDeleteEntry()
{
    m_manager = new LogbookManager();
    LogbookEntry entry;
    entry.title = "Test Entry";
    
    QString id = m_manager->addEntry(entry);
    bool deleted = m_manager->deleteEntry(id);
    QVERIFY(deleted);
    
    LogbookEntry* found = m_manager->getEntryById(id);
    QVERIFY(found == nullptr);
    
    delete m_manager;
}

#endif
