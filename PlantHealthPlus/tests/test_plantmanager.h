#ifndef TEST_PLANTMANAGER_H
#define TEST_PLANTMANAGER_H

#include <QtTest/QtTest>
#include "../plantmanager.h"
#include "../plantcard.h"

class TestPlantManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testAddPlant();
    void testRemovePlant();
    void testGetPlants();
    void testUpdatePlant();
    void testGetPlant();

private:
    PlantManager* m_plantManager;
};

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
    PlantData plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    plant.wateringIntervalDays = 7.0;
    plant.lightRequirement = "Bright, indirect light";
    plant.houseArea = "Living Room";
    plant.cardColor = "Light Green";
    
    m_plantManager->addPlant(plant);
    
    const QVector<PlantData>& plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 1);
    QCOMPARE(plants[0].name, QString("Test Plant"));
    QCOMPARE(plants[0].category, QString("Tropical"));
}

void TestPlantManager::testRemovePlant()
{
    PlantData plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    
    m_plantManager->addPlant(plant);
    QCOMPARE(m_plantManager->getPlants().size(), 1);
    
    m_plantManager->removePlant(0);
    QCOMPARE(m_plantManager->getPlants().size(), 0);
}

void TestPlantManager::testGetPlants()
{
    QCOMPARE(m_plantManager->getPlants().size(), 0);
    
    PlantData plant1;
    plant1.name = "Plant 1";
    plant1.category = "Tropical";
    
    PlantData plant2;
    plant2.name = "Plant 2";
    plant2.category = "Desert";
    
    m_plantManager->addPlant(plant1);
    m_plantManager->addPlant(plant2);
    
    const QVector<PlantData>& plants = m_plantManager->getPlants();
    QCOMPARE(plants.size(), 2);
}

void TestPlantManager::testUpdatePlant()
{
    PlantData plant;
    plant.name = "Original Name";
    plant.category = "Tropical";
    
    m_plantManager->addPlant(plant);
    
    PlantData updatedPlant;
    updatedPlant.name = "Updated Name";
    updatedPlant.category = "Desert";
    
    m_plantManager->updatePlant(0, updatedPlant);
    
    const QVector<PlantData>& plants = m_plantManager->getPlants();
    QCOMPARE(plants[0].name, QString("Updated Name"));
    QCOMPARE(plants[0].category, QString("Desert"));
}

void TestPlantManager::testGetPlant()
{
    PlantData plant;
    plant.name = "Test Plant";
    plant.category = "Tropical";
    
    m_plantManager->addPlant(plant);
    
    PlantData retrievedPlant = m_plantManager->getPlant(0);
    QCOMPARE(retrievedPlant.name, QString("Test Plant"));
    QCOMPARE(retrievedPlant.category, QString("Tropical"));
}

#endif // TEST_PLANTMANAGER_H
