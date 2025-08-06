#include <QtTest>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include "../rangeindicator.h"

class TestRangeIndicator : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    // Test cases
    void test_construction();
    void test_setRange();
    void test_setCurrentValue();
    void test_compactMode();

private:
    RangeIndicator* m_indicator;
};

void TestRangeIndicator::init()
{
    m_indicator = new RangeIndicator("Temperature", "°F");
}

void TestRangeIndicator::cleanup()
{
    delete m_indicator;
    m_indicator = nullptr;
}

void TestRangeIndicator::test_construction()
{
    // Test that the widget is created successfully
    QVERIFY(m_indicator != nullptr);
    QVERIFY(m_indicator->isWidgetType());
}

void TestRangeIndicator::test_setRange()
{
    // Test setting range values
    double minVal = 60.0;
    double maxVal = 85.0;
    
    // This should not crash
    m_indicator->setRange(minVal, maxVal);
    
    // Widget should still be valid
    QVERIFY(m_indicator != nullptr);
}

void TestRangeIndicator::test_setCurrentValue()
{
    // Set a range first
    m_indicator->setRange(50.0, 90.0);
    
    // Test setting various current values
    m_indicator->setCurrentValue(70.0);  // Normal value
    m_indicator->setCurrentValue(45.0);  // Below range
    m_indicator->setCurrentValue(95.0);  // Above range
    
    // Widget should still be valid after all value changes
    QVERIFY(m_indicator != nullptr);
}

void TestRangeIndicator::test_compactMode()
{
    // Test compact mode toggle
    m_indicator->setCompactMode(true);
    QVERIFY(m_indicator != nullptr);
    
    m_indicator->setCompactMode(false);
    QVERIFY(m_indicator != nullptr);
}

#include "test_rangeindicator.moc"
