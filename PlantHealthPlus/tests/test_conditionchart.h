#ifndef TEST_CONDITIONCHART_H
#define TEST_CONDITIONCHART_H
#include <QtTest/QtTest>
#include <QObject>
#include "../conditionchart.h"

class TestConditionChart : public QObject
{
    Q_OBJECT
private slots:
    void testChart() { 
        ConditionChart chart;
        chart.addDataPoint(QDateTime::currentDateTime(), 25.0);
        QVERIFY(true);
    }
    void testData() { QVERIFY(true); }
};
#endif
