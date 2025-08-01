#ifndef CONDITIONCHART_H
#define CONDITIONCHART_H

#include "forecastclient.h"
#include <QtCharts>
#include <QWidget>

enum class ConditionType {
    Temperature,
    UV,
    Humidity
};

class ConditionChart : public QChartView
{
    Q_OBJECT

public:
    explicit ConditionChart(ConditionType type, QWidget *parent = nullptr);

public slots:
    void updateChart(const QList<DayForecast> &data);

private:
    ConditionType type;
    QLineSeries *highSeries;
    QLineSeries *lowSeries;

    void setTemperatureChart(const QList<DayForecast> &data);
    void setHumidityChart(const QList<DayForecast> &data);
    void setUVChart(const QList<DayForecast> &data);
};

#endif
