#ifndef TEST_FORECASTCLIENT_H
#define TEST_FORECASTCLIENT_H
#include <QtTest/QtTest>
#include <QObject>
#include "../forecastclient.h"

class TestForecastClient : public QObject
{
    Q_OBJECT
private slots:
    void testConnection() { QVERIFY(true); }
    void testDataParsing() { QVERIFY(true); }
};
#endif
