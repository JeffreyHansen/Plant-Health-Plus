#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QDateTime>
#include <algorithm>
#include <qurlquery.h>
#include "forecastclient.h"

ForecastClient::ForecastClient(QObject* parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &::ForecastClient::onAPIReply);
}

void ForecastClient::getURL() {
    qDebug() << "ForecastClient: Starting to fetch weather forecast data...";

    // Configure API endpoint and query parameters
    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", "32.7157");
    query.addQueryItem("longitude", "-117.1647");
    query.addQueryItem("daily", "temperature_2m_max,temperature_2m_min,uv_index_max,relative_humidity_2m_mean");
    query.addQueryItem("timezone", "America/Los_Angeles");
    query.addQueryItem("temperature_unit", "fahrenheit");
    url.setQuery(query);
    QNetworkRequest request(url);
    manager->get(request);
}

void ForecastClient::onAPIReply(QNetworkReply *reply) {
    // Check for network errors
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error: " << reply->errorString();
        reply->deleteLater();
        return;
    }

    // Read and parse JSON response
    QByteArray jsonData = reply->readAll();
    qDebug() << "Raw JSON reply:" << jsonData;
    reply->deleteLater();

    // Validate JSON structure
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qDebug() << "Invalid JSON format.";
        return;
    }

    // Validate JSON structure
    QJsonObject root = doc.object();
    QJsonObject daily = root["daily"].toObject();

    // Parse arrays for each weather metric
    QJsonArray dates = daily["time"].toArray();
    QJsonArray tempsMax = daily["temperature_2m_max"].toArray();
    QJsonArray tempsMin = daily["temperature_2m_min"].toArray();
    QJsonArray uvMax = daily["uv_index_max"].toArray();
    QJsonArray humidityMean = daily["relative_humidity_2m_mean"].toArray();

    QList<DayForecast> forecastList;

    // Process all available days (using smallest array size as limit)
    int count = std::min({dates.size(), tempsMax.size(), tempsMin.size(), uvMax.size(), humidityMean.size()});

    for (int i = 0; i < count; i++) {
        DayForecast day;

        // Parse date and day name
        QString dateStr = dates[i].toString();
        QDate date = QDate::fromString(dateStr, Qt::ISODate);
        day.date = date;
        day.dayName = date.toString("dddd");

        // Convert and round weather metrics
        day.highTemp = qRound(tempsMax[i].toDouble());
        day.lowTemp = qRound(tempsMin[i].toDouble());
        day.UV = qRound(uvMax[i].toDouble());
        day.humidity = qRound(humidityMean[i].toDouble());

        forecastList.append(day);
    }

    // Signal completion
    emit forecastsReady(forecastList);
    qDebug() << "Emitting forecastsReady with" << forecastList.size() << "entries";
}



