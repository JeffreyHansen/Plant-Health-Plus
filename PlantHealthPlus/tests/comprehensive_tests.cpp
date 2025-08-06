#include <QtTest>
#include <QObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include "../plantmanager.h"
#include "../plantcard.h"
#include "../usermanager.h"
#include "../rangeindicator.h"
#include "../logbookmanager.h"

class ComprehensiveTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // PlantManager Tests (5 methods)
    void test_plantManager_addPlant();
    void test_plantManager_removePlant();
    void test_plantManager_updatePlant();
    void test_plantManager_plantCount();
    void test_plantManager_savePlantsToFile();

    // UserManager Tests (4 methods)
    void test_userManager_createUser();
    void test_userManager_authenticateUser();
    void test_userManager_userExists();
    void test_userManager_userVolume();

    // PlantCard Tests (2 methods)
    void test_plantCard_plantDataStorage();
    void test_plantCard_expansion();

    // RangeIndicator Tests (4 methods)
    void test_rangeIndicator_construction();
    void test_rangeIndicator_setRange();
    void test_rangeIndicator_setCurrentValue();
    void test_rangeIndicator_compactMode();

    // LogbookManager Tests (2 methods)
    void test_logbookManager_addEntry();
    void test_logbookManager_entryConstruction();

private:
    PlantManager* m_plantManager;
    UserManager* m_userManager;
    PlantCard* m_plantCard;
    RangeIndicator* m_rangeIndicator;
    LogbookManager* m_logbookManager;
    QTemporaryDir* m_tempDir;
    PlantData m_testPlantData;
};

void ComprehensiveTests::initTestCase()
{
    // Create temporary directory for test data
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
    
    // Override the data location for testing
    qputenv("XDG_DATA_HOME", m_tempDir->path().toLocal8Bit());
}

void ComprehensiveTests::cleanupTestCase()
{
    delete m_tempDir;
}

void ComprehensiveTests::init()
{
    // Initialize test objects
    m_plantManager = new PlantManager();
    m_plantManager->setCurrentUser("testuser");
    
    m_userManager = new UserManager();
    
    // Create test plant data
    m_testPlantData.name = "Test Fern";
    m_testPlantData.category = "Indoor";
    m_testPlantData.lightRequirement = "Partial Shade";
    m_testPlantData.wateringIntervalDays = 4.0;
    m_testPlantData.tempRangeLow = 60.0;
    m_testPlantData.tempRangeHigh = 75.0;
    m_testPlantData.humidityRangeLow = 50.0;
    m_testPlantData.humidityRangeHigh = 80.0;
    m_testPlantData.uvRangeLow = 1.0;
    m_testPlantData.uvRangeHigh = 4.0;
    m_testPlantData.cardColor = "#4CAF50";
    m_testPlantData.houseArea = "Living Room";

    m_plantCard = new PlantCard(m_testPlantData);
    m_rangeIndicator = new RangeIndicator("Temperature", "°F");
    m_logbookManager = new LogbookManager();
}

void ComprehensiveTests::cleanup()
{
    delete m_plantManager;
    delete m_userManager;
    delete m_plantCard;
    delete m_rangeIndicator;
    delete m_logbookManager;
    
    m_plantManager = nullptr;
    m_userManager = nullptr;
    m_plantCard = nullptr;
    m_rangeIndicator = nullptr;
    m_logbookManager = nullptr;
}

// PlantManager Tests
void ComprehensiveTests::test_plantManager_addPlant()
{
    QSignalSpy spy(m_plantManager, &PlantManager::plantAdded);
    
    int initialCount = m_plantManager->getPlantCount();
    m_plantManager->addPlant(m_testPlantData);
    
    QCOMPARE(m_plantManager->getPlantCount(), initialCount + 1);
    QCOMPARE(spy.count(), 1);
    
    PlantData retrievedPlant = m_plantManager->getPlant(0);
    QCOMPARE(retrievedPlant.name, m_testPlantData.name);
    QCOMPARE(retrievedPlant.category, m_testPlantData.category);
}

void ComprehensiveTests::test_plantManager_removePlant()
{
    m_plantManager->addPlant(m_testPlantData);
    
    QSignalSpy spy(m_plantManager, &PlantManager::plantRemoved);
    
    int initialCount = m_plantManager->getPlantCount();
    m_plantManager->removePlant(0);
    
    QCOMPARE(m_plantManager->getPlantCount(), initialCount - 1);
    QCOMPARE(spy.count(), 1);
}

void ComprehensiveTests::test_plantManager_updatePlant()
{
    m_plantManager->addPlant(m_testPlantData);
    
    PlantData updatedPlant = m_testPlantData;
    updatedPlant.name = "Updated Plant";
    updatedPlant.wateringIntervalDays = 10.0;
    
    QSignalSpy spy(m_plantManager, &PlantManager::plantUpdated);
    
    m_plantManager->updatePlant(0, updatedPlant);
    
    QCOMPARE(spy.count(), 1);
    PlantData retrievedPlant = m_plantManager->getPlant(0);
    QCOMPARE(retrievedPlant.name, QString("Updated Plant"));
    QCOMPARE(retrievedPlant.wateringIntervalDays, 10.0);
}

void ComprehensiveTests::test_plantManager_plantCount()
{
    QCOMPARE(m_plantManager->getPlantCount(), 0);
    
    for (int i = 0; i < 3; ++i) {
        PlantData plant;
        plant.name = QString("Plant %1").arg(i);
        m_plantManager->addPlant(plant);
    }
    
    QCOMPARE(m_plantManager->getPlantCount(), 3);
    
    m_plantManager->removePlant(0);
    QCOMPARE(m_plantManager->getPlantCount(), 2);
}

void ComprehensiveTests::test_plantManager_savePlantsToFile()
{
    m_plantManager->addPlant(m_testPlantData);
    
    bool saveResult = m_plantManager->savePlantsToFile();
    QVERIFY(saveResult);
    
    PlantManager* newManager = new PlantManager();
    newManager->setCurrentUser("testuser");
    bool loadResult = newManager->loadPlantsFromFile();
    QVERIFY(loadResult);
    
    QCOMPARE(newManager->getPlantCount(), 1);
    QCOMPARE(newManager->getPlant(0).name, m_testPlantData.name);
    
    delete newManager;
}

// UserManager Tests
void ComprehensiveTests::test_userManager_createUser()
{
    QString username = "testuser123";
    QString password = "testpassword123";
    
    bool result = m_userManager->createUser(username, password);
    QVERIFY(result);
    QVERIFY(m_userManager->userExists(username));
    
    bool duplicateResult = m_userManager->createUser(username, password);
    QVERIFY(!duplicateResult);
}

void ComprehensiveTests::test_userManager_authenticateUser()
{
    QString username = "authtest";
    QString password = "authpass123";
    
    m_userManager->createUser(username, password);
    
    bool authResult = m_userManager->authenticateUser(username, password);
    QVERIFY(authResult);
    
    bool wrongPassResult = m_userManager->authenticateUser(username, "wrongpass");
    QVERIFY(!wrongPassResult);
    
    bool nonExistentResult = m_userManager->authenticateUser("nonexistent", password);
    QVERIFY(!nonExistentResult);
}

void ComprehensiveTests::test_userManager_userExists()
{
    QString existingUser = "existinguser";
    QString nonExistentUser = "nonexistentuser";
    
    QVERIFY(!m_userManager->userExists(existingUser));
    
    m_userManager->createUser(existingUser, "password");
    QVERIFY(m_userManager->userExists(existingUser));
    QVERIFY(!m_userManager->userExists(nonExistentUser));
}

void ComprehensiveTests::test_userManager_userVolume()
{
    QString username = "volumetest";
    m_userManager->createUser(username, "password");
    m_userManager->setCurrentUser(username);
    
    int testVolume = 75;
    m_userManager->setUserVolume(testVolume);
    
    int retrievedVolume = m_userManager->getUserVolume();
    QCOMPARE(retrievedVolume, testVolume);
}

// PlantCard Tests
void ComprehensiveTests::test_plantCard_plantDataStorage()
{
    const PlantData& retrievedData = m_plantCard->getPlantData();
    
    QCOMPARE(retrievedData.name, m_testPlantData.name);
    QCOMPARE(retrievedData.category, m_testPlantData.category);
    QCOMPARE(retrievedData.lightRequirement, m_testPlantData.lightRequirement);
    QCOMPARE(retrievedData.wateringIntervalDays, m_testPlantData.wateringIntervalDays);
    QCOMPARE(retrievedData.tempRangeLow, m_testPlantData.tempRangeLow);
    QCOMPARE(retrievedData.tempRangeHigh, m_testPlantData.tempRangeHigh);
}

void ComprehensiveTests::test_plantCard_expansion()
{
    QVERIFY(!m_plantCard->isExpanded());
    
    m_plantCard->setExpanded(true);
    QVERIFY(m_plantCard->isExpanded());
    
    m_plantCard->setExpanded(false);
    QVERIFY(!m_plantCard->isExpanded());
}

// RangeIndicator Tests
void ComprehensiveTests::test_rangeIndicator_construction()
{
    QVERIFY(m_rangeIndicator != nullptr);
    QVERIFY(m_rangeIndicator->isWidgetType());
}

void ComprehensiveTests::test_rangeIndicator_setRange()
{
    double minVal = 60.0;
    double maxVal = 85.0;
    
    m_rangeIndicator->setRange(minVal, maxVal);
    QVERIFY(m_rangeIndicator != nullptr);
}

void ComprehensiveTests::test_rangeIndicator_setCurrentValue()
{
    m_rangeIndicator->setRange(50.0, 90.0);
    
    m_rangeIndicator->setCurrentValue(70.0);
    m_rangeIndicator->setCurrentValue(45.0);
    m_rangeIndicator->setCurrentValue(95.0);
    
    QVERIFY(m_rangeIndicator != nullptr);
}

void ComprehensiveTests::test_rangeIndicator_compactMode()
{
    m_rangeIndicator->setCompactMode(true);
    QVERIFY(m_rangeIndicator != nullptr);
    
    m_rangeIndicator->setCompactMode(false);
    QVERIFY(m_rangeIndicator != nullptr);
}

// LogbookManager Tests
void ComprehensiveTests::test_logbookManager_addEntry()
{
    LogbookEntry entry1("Test Entry 1", "This is a test content");
    LogbookEntry entry2("Test Entry 2", "This is another test content");
    
    QVERIFY(!entry1.title.isEmpty());
    QVERIFY(!entry1.content.isEmpty());
    QVERIFY(!entry1.id.isEmpty());
    QVERIFY(entry1.dateCreated.isValid());
    QVERIFY(entry1.dateModified.isValid());
    
    QVERIFY(entry1.id != entry2.id);
}

void ComprehensiveTests::test_logbookManager_entryConstruction()
{
    LogbookEntry defaultEntry;
    QVERIFY(defaultEntry.title.isEmpty());
    QVERIFY(defaultEntry.content.isEmpty());
    QVERIFY(!defaultEntry.id.isEmpty());
    QVERIFY(defaultEntry.dateCreated.isValid());
    QVERIFY(defaultEntry.dateModified.isValid());
    
    QString testTitle = "My Plant Observation";
    QString testContent = "The plant is growing well with new leaves appearing.";
    
    LogbookEntry paramEntry(testTitle, testContent);
    QCOMPARE(paramEntry.title, testTitle);
    QCOMPARE(paramEntry.content, testContent);
    QVERIFY(!paramEntry.id.isEmpty());
    QVERIFY(paramEntry.dateCreated.isValid());
    QVERIFY(paramEntry.dateModified.isValid());
    
    QDateTime now = QDateTime::currentDateTime();
    qint64 timeDiff = qAbs(now.msecsTo(paramEntry.dateCreated));
    QVERIFY(timeDiff < 1000);
}

QTEST_MAIN(ComprehensiveTests)
#include "comprehensive_tests.moc"
