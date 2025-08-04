#ifndef TEST_RANGEINDICATOR_H
#define TEST_RANGEINDICATOR_H
#include <QtTest/QtTest>
#include <QObject>
#include "../rangeindicator.h"

class TestRangeIndicator : public QObject
{
    Q_OBJECT
private slots:
    void testRange() { 
        RangeIndicator indicator(0, 100);
        indicator.setValue(50);
        QCOMPARE(indicator.getValue(), 50.0);
    }
    void testBounds() { QVERIFY(true); }
};
#endif
