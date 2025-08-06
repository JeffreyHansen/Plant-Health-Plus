#include <QtTest>
#include <QObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QStandardPaths>
#include "../plantmanager.h"
#include "../plantcard.h"

class TestPlantManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases
    void test_addPlant();
    void test_removePlant();
    void test_updatePlant();
    void test_plantCount();
    void test_savePlantsToFile();

private:
    PlantManager* m_plantManager;
    QTemporaryDir* m_tempDir;
    QString m_originalDataPath;
};

void TestPlantManager::initTestCase()
{
    // Create temporary directory for test data
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
    
    // Override the data location for testing
    m_originalDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qputenv("XDG_DATA_HOME", m_tempDir->path().toLocal8Bit());
}

void TestPlantManager::cleanupTestCase()
{
    delete m_tempDir;
    // Restore original data path
    qputenv("XDG_DATA_HOME", m_originalDataPath.toLocal8Bit());
}

void TestPlantManager::init()
{
    m_plantManager = new PlantManager();
    m_plantManager->setCurrentUser("testuser");
}

void TestPlantManager::cleanup()
{
    delete m_plantManager;
    m_plantManager = nullptr;
}

void TestPlantManager::test_addPlant()
{
    // Create test plant data
    PlantData testPlant;
    testPlant.name = "Test Rose";
    testPlant.category = "Flower";
    testPlant.lightRequirement = "Full Sun";
    testPlant.wateringIntervalDays = 3.0;
    testPlant.tempRangeLow = 65.0;
    testPlant.tempRangeHigh = 80.0;

    // Set up signal spy to check if plantAdded signal is emitted
    QSignalSpy spy(m_plantManager, &PlantManager::plantAdded);

    // Test adding plant
    int initialCount = m_plantManager->getPlantCount();
    m_plantManager->addPlant(testPlant);

    // Verify plant was added
    QCOMPARE(m_plantManager->getPlantCount(), initialCount + 1);
    QCOMPARE(spy.count(), 1);

    // Verify plant data
    PlantData retrievedPlant = m_plantManager->getPlant(0);
    QCOMPARE(retrievedPlant.name, testPlant.name);
    QCOMPARE(retrievedPlant.category, testPlant.category);
    QCOMPARE(retrievedPlant.lightRequirement, testPlant.lightRequirement);
}

void TestPlantManager::test_removePlant()
{
    // Add a test plant first
    PlantData testPlant;
    testPlant.name = "Test Orchid";
    testPlant.category = "Flower";
    m_plantManager->addPlant(testPlant);

    // Set up signal spy
    QSignalSpy spy(m_plantManager, &PlantManager::plantRemoved);

    // Test removing plant
    int initialCount = m_plantManager->getPlantCount();
    m_plantManager->removePlant(0);

    // Verify plant was removed
    QCOMPARE(m_plantManager->getPlantCount(), initialCount - 1);
    QCOMPARE(spy.count(), 1);
}

void TestPlantManager::test_updatePlant()
{
    // Add a test plant first
    PlantData testPlant;
    testPlant.name = "Test Cactus";
    testPlant.category = "Succulent";
    testPlant.wateringIntervalDays = 7.0;
    m_plantManager->addPlant(testPlant);

    // Update plant data
    PlantData updatedPlant = testPlant;
    updatedPlant.name = "Updated Cactus";
    updatedPlant.wateringIntervalDays = 10.0;

    // Set up signal spy
    QSignalSpy spy(m_plantManager, &PlantManager::plantUpdated);

    // Test updating plant
    m_plantManager->updatePlant(0, updatedPlant);

    // Verify plant was updated
    QCOMPARE(spy.count(), 1);
    PlantData retrievedPlant = m_plantManager->getPlant(0);
    QCOMPARE(retrievedPlant.name, QString("Updated Cactus"));
    QCOMPARE(retrievedPlant.wateringIntervalDays, 10.0);
}

void TestPlantManager::test_plantCount()
{
    // Initially should be empty
    QCOMPARE(m_plantManager->getPlantCount(), 0);

    // Add multiple plants
    for (int i = 0; i < 5; ++i) {
        PlantData plant;
        plant.name = QString("Plant %1").arg(i);
        plant.category = "Test";
        m_plantManager->addPlant(plant);
    }

    // Verify count
    QCOMPARE(m_plantManager->getPlantCount(), 5);

    // Remove some plants
    m_plantManager->removePlant(0);
    m_plantManager->removePlant(0);

    // Verify updated count
    QCOMPARE(m_plantManager->getPlantCount(), 3);
}

void TestPlantManager::test_savePlantsToFile()
{
    // Add test plants
    PlantData plant1;
    plant1.name = "Save Test Plant 1";
    plant1.category = "Indoor";
    plant1.wateringIntervalDays = 5.0;
    m_plantManager->addPlant(plant1);

    PlantData plant2;
    plant2.name = "Save Test Plant 2";
    plant2.category = "Outdoor";
    plant2.wateringIntervalDays = 3.0;
    m_plantManager->addPlant(plant2);

    // Test saving
    bool saveResult = m_plantManager->savePlantsToFile();
    QVERIFY(saveResult);

    // Create new manager and load data
    PlantManager* newManager = new PlantManager();
    newManager->setCurrentUser("testuser");
    bool loadResult = newManager->loadPlantsFromFile();
    QVERIFY(loadResult);

    // Verify loaded data
    QCOMPARE(newManager->getPlantCount(), 2);
    QCOMPARE(newManager->getPlant(0).name, QString("Save Test Plant 1"));
    QCOMPARE(newManager->getPlant(1).name, QString("Save Test Plant 2"));

    delete newManager;
}

#include "test_plantmanager.moc"
