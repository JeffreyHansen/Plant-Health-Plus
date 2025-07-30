#ifndef FORECASTCLIENT_H
#define FORECASTCLIENT_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct DayForecast {
    QString dayName;
	QDate date;
	int highTemp;
	int lowTemp;
	int UV;
	int humidity;
};

class ForecastClient : public QObject
{
    Q_OBJECT

public:
    explicit ForecastClient(QObject *parent = nullptr);
    void getURL();


signals:
    void forecastsReady(const QList<DayForecast> &forecasts);

private:
    QNetworkAccessManager* manager;
    void onAPIReply(QNetworkReply *reply);

};

#endif // FORECASTCLIENT_H
