#ifndef RANGEINDICATOR_H
#define RANGEINDICATOR_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RangeIndicator : public QWidget
{
    Q_OBJECT

public:
    enum GradientType {
        GreenToRed,      // Traditional: green (safe) to red (dangerous)
        RedGreenRed      // Ideal range: red (low) to green (ideal) to red (high)
    };

    explicit RangeIndicator(const QString& label, const QString& unit, QWidget *parent = nullptr);
    
    void setRange(double minValue, double maxValue);
    void setCurrentValue(double value);
    void setCompactMode(bool compact);
    void setGradientType(GradientType type);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    void updateDisplay();
    QColor getColorForValue(double normalizedPosition) const;
    
    QString m_label;
    QString m_unit;
    double m_minValue;
    double m_maxValue;
    double m_currentValue;
    bool m_compactMode;
    GradientType m_gradientType;
    
    QLabel* m_labelWidget;
    QLabel* m_valueWidget;
    
    static constexpr int INDICATOR_HEIGHT = 20;
    static constexpr int COMPACT_HEIGHT = 12;
    static constexpr int INDICATOR_WIDTH = 120;
    static constexpr int COMPACT_WIDTH = 80;
};

#endif // RANGEINDICATOR_H
