#include <qwidget.h>
#include <QtCharts>
#include <QDate>
#include "conditionchart.h"
#include <QDebug>

ConditionChart::ConditionChart(ConditionType type, QWidget *parent) : QChartView(new QChart(), parent), type(type)
{
    setRenderHint(QPainter::Antialiasing);
    chart()->legend()->setVisible(true);
    chart()->setAnimationOptions(QChart::SeriesAnimations);
}

void ConditionChart::updateChart(const QList<DayForecast>& forecasts)
{
    qDebug() << "updateChart called with: " << forecasts.size() << "items";
    switch (type) {
    case ConditionType::Temperature:
        setTemperatureChart(forecasts);
        break;
    case ConditionType::Humidity:
        setHumidityChart(forecasts);
        break;
    case ConditionType::UV:
        setUVChart(forecasts);
        break;
    }
}

void ConditionChart::setTemperatureChart(const QList<DayForecast> &data) {
    // Clear existing chart elements
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }

    // Create high temperature bar set
    QBarSet *highSet = new QBarSet("High Temp");
    highSet->setColor(QColor(QColorConstants::Svg::orangered));
    highSet->setPen(QPen(Qt::black));
    highSet->setPen(QPen(QColor(0x333333), 2));

    // Tooltip for high temps
    connect(highSet, &QBarSet::hovered, this, [highSet](bool status, int index) {
        if (status) {
            QToolTip::showText(QCursor::pos(), QString("High: %1°").arg(highSet->at(index)));
        } else {
            QToolTip::hideText();
        }
    });

    // Create low temperature bar set
    QBarSet *lowSet = new QBarSet("Low Temp");
    lowSet->setColor(QColor(QColorConstants::Svg::lightblue));
    lowSet->setPen(QPen(Qt::black));
    lowSet->setPen(QPen(QColor(0x333333), 2));

    // Create low temperature bar set
    connect(lowSet, &QBarSet::hovered, this, [lowSet](bool status, int index) {
        if (status) {
            QToolTip::showText(QCursor::pos(), QString("Low: %1°").arg(lowSet->at(index)));
        } else {
            QToolTip::hideText();
        }
    });

    // Populate data and categories
    QStringList categories;
    for (const auto &day : data) {
        *highSet << day.highTemp;
        *lowSet << day.lowTemp;
        categories << day.dayName; // Ensure DayForecast has dayName
    }

    // Create and configure series
    QBarSeries *series = new QBarSeries();
    series->append(highSet);
    series->append(lowSet);
    chart()->addSeries(series);

    // Configure X axis (days)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart()->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Calculate Y axis range with buffer
    int maxTemp = std::max_element(data.begin(), data.end(), [](const DayForecast &a, const DayForecast &b){
                      return a.highTemp < b.highTemp;
                  })->highTemp;

    int minTemp = std::min_element(data.begin(), data.end(), [](const DayForecast &a, const DayForecast &b){
                      return a.lowTemp < b.lowTemp;
                  })->lowTemp;

    // Configure Y axis (temperature)
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(minTemp - 5, maxTemp + 5);
    chart()->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart()->setTitle("7-Day Temperature Forecast");
    chart()->update();
}

void ConditionChart::setUVChart(const QList<DayForecast> &data) {
    // Clear existing chart elements
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }

     // Create UV bar set
    QBarSet *uvSet = new QBarSet("UV Index");
    uvSet->setColor(QColor(QColorConstants::Svg::yellow));
    uvSet->setPen(QPen(Qt::black));
    uvSet->setPen(QPen(QColor(0x333333), 2));

    // Tooltip for UV values
    connect(uvSet, &QBarSet::hovered, this, [uvSet](bool status, int index) {
        if (status) {
            QToolTip::showText(QCursor::pos(), QString("%1 out of 15").arg(uvSet->at(index)));
        } else {
            QToolTip::hideText();
        }
    });

    // Populate data and categories
    QStringList categories;
    for (const auto &day : data) {
        *uvSet << day.UV;
        categories << day.dayName;
    }

    // Populate data and categories
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(uvSet);
    chart()->addSeries(barSeries);

    // Configure axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart()->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 15);
    chart()->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chart()->setTitle("7-Day UV Index Forecast");
    chart()->legend()->setVisible(false);
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    chart()->update();
}

void ConditionChart::setHumidityChart(const QList<DayForecast> &data) {
    // Clear existing chart elements
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }


    // Create humidity bar set
    QBarSet *humiditySet = new QBarSet("Humidity");
    humiditySet->setColor(QColor(QColorConstants::Svg::deepskyblue));
    humiditySet->setPen(QPen(Qt::black));
    humiditySet->setPen(QPen(QColor(0x333333), 2));

    // Tooltip for humidity values
    connect(humiditySet, &QBarSet::hovered, this, [=](bool status, int index) {
        if (status) {
            QToolTip::showText(QCursor::pos(), QString("%1%").arg(humiditySet->at(index)));
        } else {
            QToolTip::hideText();
        }
    });

    QStringList categories;

    // Populate data and calculate humidity
    int maxHumidity = 0;
    for (const auto &day : data) {
        *humiditySet << day.humidity;
        categories << day.dayName;
        maxHumidity = std::max(maxHumidity, day.humidity);
    }

    // Create and configure series
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(humiditySet);
    chart()->addSeries(barSeries);


    // Configure axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart()->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxHumidity + 10);
    chart()->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chart()->setTitle("7-Day Humidity Forecast");
    chart()->update();
}
