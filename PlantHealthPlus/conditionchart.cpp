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
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }

    QBarSet *highSet = new QBarSet("High Temp");
    QBarSet *lowSet = new QBarSet("Low Temp");
    QStringList categories;

    for (const auto &day : data) {
        *highSet << day.highTemp;
        *lowSet << day.lowTemp;
        categories << day.dayName; // Ensure DayForecast has dayName
    }

    QBarSeries *series = new QBarSeries();
    series->append(highSet);
    series->append(lowSet);
    chart()->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart()->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    int maxTemp = std::max_element(data.begin(), data.end(), [](const DayForecast &a, const DayForecast &b){
                      return a.highTemp < b.highTemp;
                  })->highTemp;

    int minTemp = std::min_element(data.begin(), data.end(), [](const DayForecast &a, const DayForecast &b){
                      return a.lowTemp < b.lowTemp;
                  })->lowTemp;

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(minTemp - 5, maxTemp + 5);
    chart()->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart()->setTitle("7-Day Temperature Forecast");

    chart()->update();
}

void ConditionChart::setUVChart(const QList<DayForecast> &data) {
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }

    QBarSet *uvSet = new QBarSet("UV Index");
    QStringList categories;
    for (const auto &day : data) {
        *uvSet << day.UV;
        categories << day.dayName;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(uvSet);

    chart()->addSeries(barSeries);

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
    chart()->removeAllSeries();
    const auto axes = chart()->axes();
    for (QAbstractAxis* axis : axes) {
        chart()->removeAxis(axis);
    }

    QBarSet *humiditySet = new QBarSet("Humidity");
    QStringList categories;

    int maxHumidity = 0;
    for (const auto &day : data) {
        *humiditySet << day.humidity;
        categories << day.dayName;
        maxHumidity = std::max(maxHumidity, day.humidity);
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(humiditySet);

    chart()->addSeries(barSeries);

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
