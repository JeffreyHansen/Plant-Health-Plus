#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include "plantcard.h"
#include "usermanager.h"

class PlantManager : public QObject
{
    Q_OBJECT

public:
    explicit PlantManager(QObject *parent = nullptr);
    ~PlantManager();

    // User management
    void setCurrentUser(const QString& username);
    QString getCurrentUser() const { return m_currentUser; }

    // Plant management
    void addPlant(const PlantData& plantData);
    void removePlant(int index);
    void updatePlant(int index, const PlantData& plantData);
    
    // Data access
    const QVector<PlantData>& getPlants() const { return m_plants; }
    PlantData getPlant(int index) const;
    int getPlantCount() const { return m_plants.size(); }
    
    // Data persistence
    bool savePlantsToFile();
    bool loadPlantsFromFile();
    
    // Sample data
    void createSamplePlants();

signals:
    void plantAdded(int index);
    void plantRemoved(int index);
    void plantUpdated(int index);
    void dataChanged();

private:
    QString getDataFilePath() const;
    QJsonObject plantDataToJson(const PlantData& plant) const;
    PlantData plantDataFromJson(const QJsonObject& json) const;

    QVector<PlantData> m_plants;
    QString m_currentUser;
    UserManager m_userManager;
    QString m_dataFilePath;
};

#endif // PLANTMANAGER_H
