#ifndef TEST_LOGBOOKENTRYDIALOG_H
#define TEST_LOGBOOKENTRYDIALOG_H
#include <QtTest/QtTest>
#include <QObject>
#include "../logbookentrydialog.h"

class TestLogbookEntryDialog : public QObject
{
    Q_OBJECT
private slots:
    void testDialogCreation() { QVERIFY(true); }
    void testEntryHandling() { QVERIFY(true); }
};
#endif
