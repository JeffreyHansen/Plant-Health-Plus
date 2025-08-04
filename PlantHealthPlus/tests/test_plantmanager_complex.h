#ifndef TEST_PLANTMANAGER_H
#define TEST_PLANTMANAGER_H

#include <QtTest/QtTest>
#include <QObject>
#include <QTemporaryDir>
#include "../plantmanager.h"

class TestPlantManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testAddPlant();
    void testRemovePlant();
    void testGetPlants();
    void testGetPlantById();
    void testUpdatePlant();
    void testSaveAndLoadPlants();
    void testPlantSerialization();
    void testInvalidOperations();
    void testPlantValidation();
    void testPlantSearch();

private:
    PlantManager* m_plantManager;
    QTemporaryDir* m_tempDir;
};

void TestPlantManager::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestPlantManager::cleanupTestCase()
{
    delete m_tempDir;
}

void TestPlantManager::init()
{
    m_plantManager = new PlantManager();
}

void TestPlantManager::cleanup()
{
    delete m_plantManager;
}

void TestPlantManager::testAddPlant()
{
    Plant plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    plant.wateringInterval = 7.0;
    plant.lightRequirement = "Bright, indirect light";
    plant.area = "Living Room";
    plant.color = "Light Green";
    plant.tempMin = 65.0;
    plant.tempMax = 80.0;
    plant.humidityMin = 40.0;
    plant.humidityMax = 70.0;
    plant.uvMin = 1.0;
    plant.uvMax = 5.0;
    
    QString plantId = m_plantManager->addPlant(plant);
    QVERIFY(!plantId.isEmpty());
    
    QVector<Plant> plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 1);
    QCOMPARE(plants[0].name, QString("Test Plant"));
    QCOMPARE(plants[0].category, QString("Tropical"));
}

void TestPlantManager::testRemovePlant()
{
    Plant plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    
    QString plantId = m_plantManager->addPlant(plant);
    QVERIFY(!plantId.isEmpty());
    
    QVector<Plant> plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 1);
    
    bool removed = m_plantManager->removePlant(plantId);
    QVERIFY(removed);
    
    plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 0);
}

void TestPlantManager::testGetPlants()
{
    QCOMPARE(m_plantManager->getPlants().size(), 0);
    
    Plant plant1;
    plant1.name = "Plant 1";
    plant1.category = "Tropical";
    
    Plant plant2;
    plant2.name = "Plant 2";
    plant2.category = "Succulent";
    
    m_plantManager->addPlant(plant1);
    m_plantManager->addPlant(plant2);
    
    QVector<Plant> plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 2);
}

void TestPlantManager::testGetPlantById()
{
    Plant plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    
    QString plantId = m_plantManager->addPlant(plant);
    
    Plant* foundPlant = m_plantManager->getPlantById(plantId);
    QVERIFY(foundPlant != nullptr);
    QCOMPARE(foundPlant->name, QString("Test Plant"));
    QCOMPARE(foundPlant->category, QString("Tropical"));
    
    Plant* notFound = m_plantManager->getPlantById("nonexistent");
    QVERIFY(notFound == nullptr);
}

void TestPlantManager::testUpdatePlant()
{
    Plant plant;
    plant.name = "Original Name";
    plant.category = "Tropical";
    
    QString plantId = m_plantManager->addPlant(plant);
    
    Plant* foundPlant = m_plantManager->getPlantById(plantId);
    QVERIFY(foundPlant != nullptr);
    
    foundPlant->name = "Updated Name";
    foundPlant->category = "Succulent";
    
    bool updated = m_plantManager->updatePlant(*foundPlant);
    QVERIFY(updated);
    
    Plant* updatedPlant = m_plantManager->getPlantById(plantId);
    QVERIFY(updatedPlant != nullptr);
    QCOMPARE(updatedPlant->name, QString("Updated Name"));
    QCOMPARE(updatedPlant->category, QString("Succulent"));
}

void TestPlantManager::testSaveAndLoadPlants()
{
    Plant plant1;
    plant1.name = "Plant 1";
    plant1.category = "Tropical";
    
    Plant plant2;
    plant2.name = "Plant 2";
    plant2.category = "Succulent";
    
    m_plantManager->addPlant(plant1);
    m_plantManager->addPlant(plant2);
    
    QString filename = m_tempDir->filePath("test_plants.json");
    bool saved = m_plantManager->savePlantsToFile(filename);
    QVERIFY(saved);
    
    PlantManager newManager;
    bool loaded = newManager.loadPlantsFromFile(filename);
    QVERIFY(loaded);
    
    QVector<Plant> loadedPlants = newManager.getPlants();
    QCOMPARE(loadedPlants.size(), 2);
}

void TestPlantManager::testPlantSerialization()
{
    Plant plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    plant.wateringInterval = 7.5;
    plant.lightRequirement = "Bright, indirect light";
    plant.area = "Living Room";
    plant.color = "Light Green";
    plant.tempMin = 65.0;
    plant.tempMax = 80.0;
    plant.humidityMin = 40.0;
    plant.humidityMax = 70.0;
    plant.uvMin = 1.0;
    plant.uvMax = 5.0;
    plant.notes = "Test notes";
    
    QString plantId = m_plantManager->addPlant(plant);
    
    QString filename = m_tempDir->filePath("serialize_test.json");
    bool saved = m_plantManager->savePlantsToFile(filename);
    QVERIFY(saved);
    
    PlantManager newManager;
    bool loaded = newManager.loadPlantsFromFile(filename);
    QVERIFY(loaded);
    
    QVector<Plant> loadedPlants = newManager.getPlants();
    QCOMPARE(loadedPlants.size(), 1);
    
    Plant loadedPlant = loadedPlants[0];
    QCOMPARE(loadedPlant.name, plant.name);
    QCOMPARE(loadedPlant.category, plant.category);
    QCOMPARE(loadedPlant.wateringInterval, plant.wateringInterval);
    QCOMPARE(loadedPlant.lightRequirement, plant.lightRequirement);
    QCOMPARE(loadedPlant.area, plant.area);
    QCOMPARE(loadedPlant.color, plant.color);
    QCOMPARE(loadedPlant.tempMin, plant.tempMin);
    QCOMPARE(loadedPlant.tempMax, plant.tempMax);
    QCOMPARE(loadedPlant.humidityMin, plant.humidityMin);
    QCOMPARE(loadedPlant.humidityMax, plant.humidityMax);
    QCOMPARE(loadedPlant.uvMin, plant.uvMin);
    QCOMPARE(loadedPlant.uvMax, plant.uvMax);
    QCOMPARE(loadedPlant.notes, plant.notes);
}

void TestPlantManager::testInvalidOperations()
{
    bool removed = m_plantManager->removePlant("nonexistent");
    QVERIFY(!removed);
    
    Plant invalidPlant;
    bool updated = m_plantManager->updatePlant(invalidPlant);
    QVERIFY(!updated);
}

void TestPlantManager::testPlantValidation()
{
    Plant plant;
    // Test with empty name
    plant.name = "";
    plant.category = "Tropical";
    
    QString plantId = m_plantManager->addPlant(plant);
    QVERIFY(plantId.isEmpty()); // Should not add plant with empty name
    
    // Test with valid name
    plant.name = "Valid Plant";
    plantId = m_plantManager->addPlant(plant);
    QVERIFY(!plantId.isEmpty());
}

void TestPlantManager::testPlantSearch()
{
    Plant plant1;
    plant1.name = "Rose";
    plant1.category = "Flowering";
    
    Plant plant2;
    plant2.name = "Rosemary";
    plant2.category = "Herb";
    
    Plant plant3;
    plant3.name = "Basil";
    plant3.category = "Herb";
    
    m_plantManager->addPlant(plant1);
    m_plantManager->addPlant(plant2);
    m_plantManager->addPlant(plant3);
    
    // Test search by name
    QVector<Plant> allPlants = m_plantManager->getPlants();
    QVector<Plant> roseResults;
    for (const Plant& plant : allPlants) {
        if (plant.name.contains("Rose", Qt::CaseInsensitive)) {
            roseResults.append(plant);
        }
    }
    QCOMPARE(roseResults.size(), 2);
    
    // Test search by category
    QVector<Plant> herbResults;
    for (const Plant& plant : allPlants) {
        if (plant.category == "Herb") {
            herbResults.append(plant);
        }
    }
    QCOMPARE(herbResults.size(), 2);
}

#endif // TEST_PLANTMANAGER_H
