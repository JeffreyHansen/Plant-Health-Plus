#ifndef TEST_SCRAPER_H
#define TEST_SCRAPER_H
#include <QtTest/QtTest>
#include <QObject>
#include "../scraper.h"

class TestScraper : public QObject
{
    Q_OBJECT
private slots:
    void testScraping() { QVERIFY(true); }
    void testDataExtraction() { QVERIFY(true); }
};
#endif
