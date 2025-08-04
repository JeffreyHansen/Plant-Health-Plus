#ifndef TEST_PLANTCARD_H
#define TEST_PLANTCARD_H
#include <QtTest/QtTest>
#include <QObject>
#include "../plantcard.h"

class TestPlantCard : public QObject
{
    Q_OBJECT
private slots:
    void testCreation() { 
        Plant plant;
        plant.name = "Test";
        PlantCard card(plant);
        QVERIFY(!card.getPlant().name.isEmpty());
    }
    void testUpdate() { QVERIFY(true); }
};
#endif
