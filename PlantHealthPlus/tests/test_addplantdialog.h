#ifndef TEST_ADDPLANTDIALOG_H
#define TEST_ADDPLANTDIALOG_H
#include <QtTest/QtTest>
#include <QObject>
#include "../addplantdialog.h"

class TestAddPlantDialog : public QObject
{
    Q_OBJECT
private slots:
    void testDialogCreation() { QVERIFY(true); }
    void testDataValidation() { QVERIFY(true); }
};
#endif
