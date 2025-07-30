#include "plantmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>

PlantManager::PlantManager(QObject *parent)
    : QObject(parent)
{
    // Don't load data until user is set
}

PlantManager::~PlantManager()
{
    savePlantsToFile();
}

void PlantManager::setCurrentUser(const QString& username)
{
    // Save current user's data if there is one
    if (!m_currentUser.isEmpty()) {
        savePlantsToFile();
    }
    
    // Clear current plants
    m_plants.clear();
    
    // Set new user
    m_currentUser = username;
    m_userManager.setCurrentUser(username);
    
    // Load new user's data
    if (!loadPlantsFromFile()) {
        // If no data exists for this user, start with empty list
        m_plants.clear();
    }
    
    emit dataChanged();
}

void PlantManager::addPlant(const PlantData& plantData)
{
    m_plants.append(plantData);
    emit plantAdded(m_plants.size() - 1);
    emit dataChanged();
    savePlantsToFile();
}

void PlantManager::removePlant(int index)
{
    if (index >= 0 && index < m_plants.size()) {
        m_plants.removeAt(index);
        emit plantRemoved(index);
        emit dataChanged();
        savePlantsToFile();
    }
}

void PlantManager::updatePlant(int index, const PlantData& plantData)
{
    if (index >= 0 && index < m_plants.size()) {
        m_plants[index] = plantData;
        emit plantUpdated(index);
        emit dataChanged();
        savePlantsToFile();
    }
}

PlantData PlantManager::getPlant(int index) const
{
    if (index >= 0 && index < m_plants.size()) {
        return m_plants[index];
    }
    return PlantData(); // Return empty PlantData if index is invalid
}

QString PlantManager::getDataFilePath() const
{
    if (m_currentUser.isEmpty()) {
        return QString(); // No user set
    }
    return m_userManager.getUserPlantsFilePath(m_currentUser);
}

bool PlantManager::savePlantsToFile()
{
    QString filePath = getDataFilePath();
    if (filePath.isEmpty()) {
        qDebug() << "Cannot save plants: no user set or invalid file path";
        return false;
    }
    
    // Ensure the directory exists
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory for plants data:" << dir.path();
            return false;
        }
    }
    
    QJsonArray plantsArray;
    
    for (const PlantData& plant : m_plants) {
        plantsArray.append(plantDataToJson(plant));
    }
    
    QJsonObject rootObject;
    rootObject["plants"] = plantsArray;
    rootObject["version"] = "1.0";
    
    QJsonDocument doc(rootObject);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open plants data file for writing:" << filePath;
        return false;
    }
    
    qint64 bytesWritten = file.write(doc.toJson());
    bool success = (bytesWritten != -1);
    file.close();
    
    if (success) {
        qDebug() << "Successfully saved" << m_plants.size() << "plants for user" << m_currentUser << "to" << filePath;
    } else {
        qDebug() << "Failed to write plants data to file:" << filePath;
    }
    
    return success;
}

bool PlantManager::loadPlantsFromFile()
{
    QString filePath = getDataFilePath();
    if (filePath.isEmpty()) {
        qDebug() << "Cannot load plants: no user set or invalid file path";
        return false;
    }
    
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qDebug() << "Plants data file not found:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse plants data file:" << error.errorString();
        return false;
    }
    
    QJsonObject rootObject = doc.object();
    QJsonArray plantsArray = rootObject["plants"].toArray();
    
    m_plants.clear();
    for (const QJsonValue& value : plantsArray) {
        if (value.isObject()) {
            PlantData plant = plantDataFromJson(value.toObject());
            m_plants.append(plant);
        }
    }
    
    qDebug() << "Successfully loaded" << m_plants.size() << "plants for user" << m_currentUser << "from" << filePath;
    emit dataChanged();
    return true;
}

void PlantManager::createSamplePlants()
{
    // Create some sample plants
    PlantData monstera;
    monstera.name = "Monstera Deliciosa";
    monstera.category = "Tropical";
    monstera.imagePath = ""; // Will use default emoji
    monstera.lastWatered = QDateTime::currentDateTime().addDays(-5);
    monstera.wateringIntervalDays = 7.0;
    monstera.notes = "Large, beautiful leaves with natural holes. Loves bright, indirect light.";
    monstera.lightRequirement = "Bright, indirect light";
    monstera.soilType = "Well-draining potting mix";
    monstera.fertilizer = "Monthly liquid fertilizer";
    monstera.dateAdded = QDateTime::currentDateTime().addDays(-30);
    
    PlantData succulent;
    succulent.name = "Jade Plant";
    succulent.category = "Succulent";
    succulent.imagePath = "";
    succulent.lastWatered = QDateTime::currentDateTime().addDays(-10);
    succulent.wateringIntervalDays = 14.0;
    succulent.notes = "Easy-care succulent with thick, glossy leaves. Perfect for beginners!";
    succulent.lightRequirement = "Bright, direct light";
    succulent.soilType = "Cactus/succulent mix";
    succulent.fertilizer = "Quarterly diluted fertilizer";
    succulent.dateAdded = QDateTime::currentDateTime().addDays(-45);
    
    PlantData pothos;
    pothos.name = "Golden Pothos";
    pothos.category = "Vine";
    pothos.imagePath = "";
    pothos.lastWatered = QDateTime::currentDateTime().addDays(-3);
    pothos.wateringIntervalDays = 5.0;
    pothos.notes = "Fast-growing vine with heart-shaped leaves. Great for hanging baskets or climbing.";
    pothos.lightRequirement = "Low to medium light";
    pothos.soilType = "Regular potting soil";
    pothos.fertilizer = "Monthly liquid fertilizer";
    pothos.dateAdded = QDateTime::currentDateTime().addDays(-15);
    
    PlantData snake;
    snake.name = "Snake Plant";
    snake.category = "Succulent";
    snake.imagePath = "";
    snake.lastWatered = QDateTime::currentDateTime().addDays(-18);
    snake.wateringIntervalDays = 21.0;
    snake.notes = "Very low maintenance with tall, upright leaves. Excellent air purifier.";
    snake.lightRequirement = "Low to bright indirect light";
    snake.soilType = "Well-draining potting mix";
    snake.fertilizer = "2-3 times per year";
    snake.dateAdded = QDateTime::currentDateTime().addDays(-60);
    
    PlantData fern;
    fern.name = "Boston Fern";
    fern.category = "Fern";
    fern.imagePath = "";
    fern.lastWatered = QDateTime::currentDateTime().addDays(-2);
    fern.wateringIntervalDays = 3.0;
    fern.notes = "Loves humidity and consistent moisture. Beautiful feathery fronds.";
    fern.lightRequirement = "Bright, indirect light";
    fern.soilType = "Peat-based potting mix";
    fern.fertilizer = "Weekly diluted fertilizer";
    fern.dateAdded = QDateTime::currentDateTime().addDays(-20);
    
    // Add a plant that needs daily watering
    PlantData dailyPlant;
    dailyPlant.name = "Peace Lily";
    dailyPlant.category = "Flowering";
    dailyPlant.imagePath = "";
    dailyPlant.lastWatered = QDateTime::currentDateTime().addSecs(-18 * 3600);  // 18 hours ago
    dailyPlant.wateringIntervalDays = 1.0;  // Daily watering
    dailyPlant.notes = "Beautiful white flowers, needs consistent moisture but not soggy soil.";
    dailyPlant.lightRequirement = "Medium to bright indirect light";
    dailyPlant.soilType = "Well-draining potting mix";
    dailyPlant.fertilizer = "Monthly liquid fertilizer";
    dailyPlant.dateAdded = QDateTime::currentDateTime().addDays(-10);
    
    // Add a plant that needs twice-daily watering (0.5 days = 12 hours)
    PlantData highMaintenance;
    highMaintenance.name = "Baby Tears";
    highMaintenance.category = "Ground Cover";
    highMaintenance.imagePath = "";
    highMaintenance.lastWatered = QDateTime::currentDateTime().addSecs(-8 * 3600);  // 8 hours ago
    highMaintenance.wateringIntervalDays = 0.5;  // Twice daily (every 12 hours)
    highMaintenance.notes = "Delicate ground cover that needs constant moisture. Very high maintenance!";
    highMaintenance.lightRequirement = "Bright, indirect light";
    highMaintenance.soilType = "Moist, well-draining soil";
    highMaintenance.fertilizer = "Bi-weekly diluted fertilizer";
    highMaintenance.dateAdded = QDateTime::currentDateTime().addDays(-5);
    
    m_plants << monstera << succulent << pothos << snake << fern << dailyPlant << highMaintenance;
    emit dataChanged();
}

QJsonObject PlantManager::plantDataToJson(const PlantData& plant) const
{
    QJsonObject obj;
    obj["name"] = plant.name;
    obj["category"] = plant.category;
    obj["imagePath"] = plant.imagePath;
    obj["lastWatered"] = plant.lastWatered.toString(Qt::ISODate);
    obj["wateringIntervalDays"] = plant.wateringIntervalDays;
    obj["notes"] = plant.notes;
    obj["lightRequirement"] = plant.lightRequirement;
    obj["soilType"] = plant.soilType;
    obj["fertilizer"] = plant.fertilizer;
    obj["dateAdded"] = plant.dateAdded.toString(Qt::ISODate);
    obj["cardColor"] = plant.cardColor;
    obj["houseArea"] = plant.houseArea;
    obj["tempRangeLow"] = plant.tempRangeLow;
    obj["tempRangeHigh"] = plant.tempRangeHigh;
    obj["humidityRangeLow"] = plant.humidityRangeLow;
    obj["humidityRangeHigh"] = plant.humidityRangeHigh;
    obj["uvRangeLow"] = plant.uvRangeLow;
    obj["uvRangeHigh"] = plant.uvRangeHigh;
    return obj;
}

PlantData PlantManager::plantDataFromJson(const QJsonObject& json) const
{
    PlantData plant;
    plant.name = json["name"].toString();
    plant.category = json["category"].toString();
    plant.imagePath = json["imagePath"].toString();
    plant.lastWatered = QDateTime::fromString(json["lastWatered"].toString(), Qt::ISODate);
    plant.wateringIntervalDays = json["wateringIntervalDays"].toDouble();
    plant.notes = json["notes"].toString();
    plant.lightRequirement = json["lightRequirement"].toString();
    plant.soilType = json["soilType"].toString();
    plant.fertilizer = json["fertilizer"].toString();
    plant.dateAdded = QDateTime::fromString(json["dateAdded"].toString(), Qt::ISODate);
    plant.cardColor = json["cardColor"].toString();
    plant.houseArea = json["houseArea"].toString();
    
    // Environmental ranges with defaults for backwards compatibility
    plant.tempRangeLow = json["tempRangeLow"].toDouble(60.0);
    plant.tempRangeHigh = json["tempRangeHigh"].toDouble(85.0);
    plant.humidityRangeLow = json["humidityRangeLow"].toDouble(40.0);
    plant.humidityRangeHigh = json["humidityRangeHigh"].toDouble(70.0);
    plant.uvRangeLow = json["uvRangeLow"].toDouble(0.0);
    plant.uvRangeHigh = json["uvRangeHigh"].toDouble(6.0);
    
    return plant;
}
