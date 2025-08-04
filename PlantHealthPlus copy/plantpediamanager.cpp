#include "plantpediamanager.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

PlantPediaManager::PlantPediaManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_network, &QNetworkAccessManager::finished, this, &PlantPediaManager::onWikiReply);
}

void PlantPediaManager::searchPlant(const QString &query)
{
    // Wikipedia REST API for summary:
    // https://en.wikipedia.org/api/rest_v1/page/summary/{title}
    QString title = query.trimmed();
    title.replace(' ', '_');
    QUrl url("https://en.wikipedia.org/api/rest_v1/page/summary/" + QUrl::toPercentEncoding(title));
    QNetworkRequest req(url);
    m_network.get(req);
}

void PlantPediaManager::onWikiReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        emit errorOccurred("Invalid Wikipedia response");
        return;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("title") && obj.contains("extract")) {
        PlantPediaEntry entry;
        entry.title = obj["title"].toString();
        entry.description = obj["description"].toString();
        entry.extract = obj["extract"].toString();
        // Get image (if available)
        if (obj.contains("thumbnail")) {
            entry.imageUrl = obj["thumbnail"].toObject()["source"].toString();
        } else {
            entry.imageUrl.clear();
        }
        emit entryFound(entry);
    } else {
        emit errorOccurred("No summary found for this plant.");
    }
}