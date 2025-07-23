#include "rangeindicator.h"
#include <QPainter>
#include <QLinearGradient>
#include <QFont>
#include <QFontMetrics>
#include <cmath>

RangeIndicator::RangeIndicator(const QString& label, const QString& unit, QWidget *parent)
    : QWidget(parent)
    , m_label(label)
    , m_unit(unit)
    , m_minValue(0.0)
    , m_maxValue(100.0)
    , m_currentValue(50.0)
    , m_compactMode(false)
    , m_gradientType(GreenToRed)
{
    setFixedSize(INDICATOR_WIDTH, INDICATOR_HEIGHT + 40); // Extra height for labels
    
    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    
    // Label widget
    m_labelWidget = new QLabel(m_label);
    m_labelWidget->setFont(QFont("Arial", 8, QFont::Bold));
    m_labelWidget->setAlignment(Qt::AlignCenter);
    m_labelWidget->setStyleSheet("color: #333;");
    
    // Value widget
    m_valueWidget = new QLabel();
    m_valueWidget->setFont(QFont("Arial", 8));
    m_valueWidget->setAlignment(Qt::AlignCenter);
    m_valueWidget->setStyleSheet("color: #666;");
    
    layout->addWidget(m_labelWidget);
    layout->addStretch();
    layout->addWidget(m_valueWidget);
    
    updateDisplay();
}

void RangeIndicator::setRange(double minValue, double maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    updateDisplay();
    update();
}

void RangeIndicator::setCurrentValue(double value)
{
    m_currentValue = value;
    updateDisplay();
    update();
}

void RangeIndicator::setGradientType(GradientType type)
{
    m_gradientType = type;
    updateDisplay();
    update();
}

void RangeIndicator::setCompactMode(bool compact)
{
    m_compactMode = compact;
    
    if (compact) {
        setFixedSize(COMPACT_WIDTH, COMPACT_HEIGHT + 30);
        m_labelWidget->setFont(QFont("Arial", 7, QFont::Bold));
        m_valueWidget->setFont(QFont("Arial", 7));
    } else {
        setFixedSize(INDICATOR_WIDTH, INDICATOR_HEIGHT + 40);
        m_labelWidget->setFont(QFont("Arial", 8, QFont::Bold));
        m_valueWidget->setFont(QFont("Arial", 8));
    }
    
    updateDisplay();
    update();
}

void RangeIndicator::updateDisplay()
{
    QString valueText = QString("%1 %2").arg(QString::number(m_currentValue, 'f', 1)).arg(m_unit);
    m_valueWidget->setText(valueText);
    
    // Update value color based on position in range
    if (m_maxValue > m_minValue) {
        double normalizedPos = (m_currentValue - m_minValue) / (m_maxValue - m_minValue);
        normalizedPos = qBound(0.0, normalizedPos, 1.0);
        
        QColor valueColor = getColorForValue(normalizedPos);
        m_valueWidget->setStyleSheet(QString("color: %1; font-weight: bold;").arg(valueColor.name()));
    }
}

QColor RangeIndicator::getColorForValue(double normalizedPosition) const
{
    if (m_gradientType == RedGreenRed) {
        // Red-Green-Red gradient: Red at extremes (0.0 and 1.0), Green in middle (0.5)
        if (normalizedPosition <= 0.5) {
            // Interpolate from red (0.0) to green (0.5)
            double t = normalizedPosition * 2.0; // Scale to 0-1
            int r = static_cast<int>(244 + (76 - 244) * t);     // Red to Green
            int g = static_cast<int>(67 + (175 - 67) * t);      // Red to Green
            int b = static_cast<int>(54 + (80 - 54) * t);       // Red to Green
            return QColor(r, g, b);
        } else {
            // Interpolate from green (0.5) to red (1.0)
            double t = (normalizedPosition - 0.5) * 2.0; // Scale to 0-1
            int r = static_cast<int>(76 + (244 - 76) * t);      // Green to Red
            int g = static_cast<int>(175 + (67 - 175) * t);     // Green to Red
            int b = static_cast<int>(80 + (54 - 80) * t);       // Green to Red
            return QColor(r, g, b);
        }
    } else {
        // Traditional Green-to-Red gradient
        // Green at 0.0-0.5, Yellow at 0.5-0.8, Red at 0.8-1.0
        
        if (normalizedPosition <= 0.5) {
            // Green zone
            return QColor(76, 175, 80); // Green
        } else if (normalizedPosition <= 0.8) {
            // Yellow zone - interpolate between green and yellow
            double t = (normalizedPosition - 0.5) / 0.3; // Normalize to 0-1
            int r = static_cast<int>(76 + (255 - 76) * t);
            int g = static_cast<int>(175 + (193 - 175) * t);
            int b = static_cast<int>(80 + (7 - 80) * t);
            return QColor(r, g, b);
        } else {
            // Red zone - interpolate between yellow and red
            double t = (normalizedPosition - 0.8) / 0.2; // Normalize to 0-1
            int r = static_cast<int>(255 + (244 - 255) * t);
            int g = static_cast<int>(193 + (67 - 193) * t);
            int b = static_cast<int>(7 + (54 - 7) * t);
            return QColor(r, g, b);
        }
    }
}

void RangeIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Calculate indicator rectangle
    int indicatorHeight = m_compactMode ? COMPACT_HEIGHT : INDICATOR_HEIGHT;
    int indicatorWidth = m_compactMode ? COMPACT_WIDTH : INDICATOR_WIDTH;
    
    // Center the indicator horizontally
    int startX = (width() - indicatorWidth) / 2;
    int startY = m_labelWidget->height() + 5; // Below the label
    
    QRect indicatorRect(startX, startY, indicatorWidth, indicatorHeight);
    
    // Draw background
    painter.fillRect(indicatorRect, QColor(240, 240, 240));
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.drawRect(indicatorRect);
    
    // Draw range gradient
    QLinearGradient gradient(indicatorRect.left(), 0, indicatorRect.right(), 0);
    
    if (m_gradientType == RedGreenRed) {
        // Red-Green-Red gradient: ideal range in middle
        gradient.setColorAt(0.0, QColor(244, 67, 54));   // Red (low)
        gradient.setColorAt(0.5, QColor(76, 175, 80));   // Green (ideal)
        gradient.setColorAt(1.0, QColor(244, 67, 54));   // Red (high)
    } else {
        // Traditional Green-to-Red gradient
        gradient.setColorAt(0.0, QColor(76, 175, 80));   // Green
        gradient.setColorAt(0.5, QColor(255, 193, 7));   // Yellow
        gradient.setColorAt(1.0, QColor(244, 67, 54));   // Red
    }
    
    painter.fillRect(indicatorRect.adjusted(1, 1, -1, -1), gradient);
    
    // Draw current value indicator
    if (m_maxValue > m_minValue) {
        double normalizedPos = (m_currentValue - m_minValue) / (m_maxValue - m_minValue);
        normalizedPos = qBound(0.0, normalizedPos, 1.0);
        
        int indicatorX = indicatorRect.left() + static_cast<int>(normalizedPos * indicatorRect.width());
        
        // Draw indicator line
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(indicatorX, indicatorRect.top() - 2, indicatorX, indicatorRect.bottom() + 2);
        
        // Draw indicator triangle
        QPolygon triangle;
        triangle << QPoint(indicatorX - 3, indicatorRect.top() - 2)
                 << QPoint(indicatorX + 3, indicatorRect.top() - 2)
                 << QPoint(indicatorX, indicatorRect.top() + 3);
        
        painter.setBrush(Qt::black);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);
    }
    
    // Draw range labels
    if (!m_compactMode) {
        painter.setPen(QPen(Qt::black, 1));
        painter.setFont(QFont("Arial", 7));
        
        QString minText = QString::number(m_minValue, 'f', 0);
        QString maxText = QString::number(m_maxValue, 'f', 0);
        
        painter.drawText(indicatorRect.left(), indicatorRect.bottom() + 12, minText);
        
        QFontMetrics fm(painter.font());
        int maxTextWidth = fm.horizontalAdvance(maxText);
        painter.drawText(indicatorRect.right() - maxTextWidth, indicatorRect.bottom() + 12, maxText);
    }
}

QSize RangeIndicator::sizeHint() const
{
    if (m_compactMode) {
        return QSize(COMPACT_WIDTH, COMPACT_HEIGHT + 30);
    } else {
        return QSize(INDICATOR_WIDTH, INDICATOR_HEIGHT + 40);
    }
}
