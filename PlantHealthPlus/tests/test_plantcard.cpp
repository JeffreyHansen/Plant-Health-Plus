#include <QtTest>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "../plantcard.h"

class TestPlantCard : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    // Test cases
    void test_plantDataStorage();
    void test_expansion();

private:
    PlantCard* m_plantCard;
    PlantData m_testPlantData;
};

void TestPlantCard::init()
{
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
}

void TestPlantCard::cleanup()
{
    delete m_plantCard;
    m_plantCard = nullptr;
}

void TestPlantCard::test_plantDataStorage()
{
    // Test that plant data is correctly stored and retrieved
    const PlantData& retrievedData = m_plantCard->getPlantData();
    
    QCOMPARE(retrievedData.name, m_testPlantData.name);
    QCOMPARE(retrievedData.category, m_testPlantData.category);
    QCOMPARE(retrievedData.lightRequirement, m_testPlantData.lightRequirement);
    QCOMPARE(retrievedData.wateringIntervalDays, m_testPlantData.wateringIntervalDays);
    QCOMPARE(retrievedData.tempRangeLow, m_testPlantData.tempRangeLow);
    QCOMPARE(retrievedData.tempRangeHigh, m_testPlantData.tempRangeHigh);
    QCOMPARE(retrievedData.humidityRangeLow, m_testPlantData.humidityRangeLow);
    QCOMPARE(retrievedData.humidityRangeHigh, m_testPlantData.humidityRangeHigh);
    QCOMPARE(retrievedData.uvRangeLow, m_testPlantData.uvRangeLow);
    QCOMPARE(retrievedData.uvRangeHigh, m_testPlantData.uvRangeHigh);
    QCOMPARE(retrievedData.cardColor, m_testPlantData.cardColor);
    QCOMPARE(retrievedData.houseArea, m_testPlantData.houseArea);
}

void TestPlantCard::test_expansion()
{
    // Initially should not be expanded
    QVERIFY(!m_plantCard->isExpanded());

    // Test expansion
    m_plantCard->setExpanded(true);
    QVERIFY(m_plantCard->isExpanded());

    // Test collapse
    m_plantCard->setExpanded(false);
    QVERIFY(!m_plantCard->isExpanded());
}

#include "test_plantcard.moc"
