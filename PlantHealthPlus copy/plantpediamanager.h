#ifndef PLANTPEDIAMANAGER_H
#define PLANTPEDIAMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "plantpediaentry.h"

class PlantPediaManager : public QObject
{
    Q_OBJECT
public:
    explicit PlantPediaManager(QObject *parent = nullptr);

    // Search Wikipedia for a plant
    void searchPlant(const QString &query);

signals:
    void entryFound(const PlantPediaEntry &entry);
    void errorOccurred(const QString &error);

private slots:
    void onWikiReply(QNetworkReply *reply);

private:
    QNetworkAccessManager m_network;
};

#endif // PLANTPEDIAMANAGER_H