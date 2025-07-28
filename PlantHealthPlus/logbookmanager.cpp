#include "logbookmanager.h"

QJsonObject LogbookEntry::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["title"] = title;
    obj["content"] = content;
    obj["dateCreated"] = dateCreated.toString(Qt::ISODate);
    obj["dateModified"] = dateModified.toString(Qt::ISODate);
    
    QJsonArray imageArray;
    for (const QString& imagePath : imagePaths) {
        imageArray.append(imagePath);
    }
    obj["imagePaths"] = imageArray;
    
    return obj;
}

void LogbookEntry::fromJson(const QJsonObject& json)
{
    id = json["id"].toString();
    title = json["title"].toString();
    content = json["content"].toString();
    dateCreated = QDateTime::fromString(json["dateCreated"].toString(), Qt::ISODate);
    dateModified = QDateTime::fromString(json["dateModified"].toString(), Qt::ISODate);
    
    imagePaths.clear();
    QJsonArray imageArray = json["imagePaths"].toArray();
    for (const QJsonValue& value : imageArray) {
        imagePaths.append(value.toString());
    }
}

LogbookManager::LogbookManager()
{
}

void LogbookManager::setCurrentUser(const QString& username)
{
    if (m_currentUser != username) {
        // Save current user's data before switching
        if (!m_currentUser.isEmpty()) {
            saveToFile();
        }
        
        m_currentUser = username;
        m_entries.clear();
        
        // Load new user's data
        if (!username.isEmpty()) {
            loadFromFile();
        }
    }
}

bool LogbookManager::addEntry(const LogbookEntry& entry)
{
    // Check if entry with this ID already exists
    for (const LogbookEntry& existingEntry : m_entries) {
        if (existingEntry.id == entry.id) {
            qDebug() << "Entry with ID" << entry.id << "already exists";
            return false;
        }
    }
    
    m_entries.append(entry);
    
    // Sort entries by date created (newest first)
    std::sort(m_entries.begin(), m_entries.end(), [](const LogbookEntry& a, const LogbookEntry& b) {
        return a.dateCreated > b.dateCreated;
    });
    
    return saveToFile();
}

bool LogbookManager::updateEntry(const QString& entryId, const LogbookEntry& entry)
{
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].id == entryId) {
            LogbookEntry updatedEntry = entry;
            updatedEntry.id = entryId; // Preserve original ID
            updatedEntry.dateModified = QDateTime::currentDateTime();
            m_entries[i] = updatedEntry;
            
            // Sort entries by date created (newest first)
            std::sort(m_entries.begin(), m_entries.end(), [](const LogbookEntry& a, const LogbookEntry& b) {
                return a.dateCreated > b.dateCreated;
            });
            
            return saveToFile();
        }
    }
    
    qDebug() << "Entry with ID" << entryId << "not found for update";
    return false;
}

bool LogbookManager::deleteEntry(const QString& entryId)
{
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].id == entryId) {
            // Delete associated images
            const LogbookEntry& entry = m_entries[i];
            for (const QString& imagePath : entry.imagePaths) {
                deleteImage(imagePath);
            }
            
            m_entries.removeAt(i);
            return saveToFile();
        }
    }
    
    qDebug() << "Entry with ID" << entryId << "not found for deletion";
    return false;
}

LogbookEntry LogbookManager::getEntry(const QString& entryId) const
{
    for (const LogbookEntry& entry : m_entries) {
        if (entry.id == entryId) {
            return entry;
        }
    }
    
    qDebug() << "Entry with ID" << entryId << "not found";
    return LogbookEntry(); // Return empty entry
}

QList<LogbookEntry> LogbookManager::getAllEntries() const
{
    return m_entries;
}

QStringList LogbookManager::getEntryIds() const
{
    QStringList ids;
    for (const LogbookEntry& entry : m_entries) {
        ids.append(entry.id);
    }
    return ids;
}

bool LogbookManager::saveToFile()
{
    if (m_currentUser.isEmpty()) {
        qDebug() << "Cannot save logbook: no current user set";
        return false;
    }
    
    QString filePath = getUserLogbookFilePath();
    
    // Ensure directory exists
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory:" << dir.absolutePath();
            return false;
        }
    }
    
    QJsonArray entriesArray;
    for (const LogbookEntry& entry : m_entries) {
        entriesArray.append(entry.toJson());
    }
    
    QJsonObject rootObject;
    rootObject["entries"] = entriesArray;
    rootObject["user"] = m_currentUser;
    rootObject["lastModified"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(rootObject);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open logbook file for writing:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "Logbook saved successfully to:" << filePath;
    return true;
}

bool LogbookManager::loadFromFile()
{
    if (m_currentUser.isEmpty()) {
        qDebug() << "Cannot load logbook: no current user set";
        return false;
    }
    
    QString filePath = getUserLogbookFilePath();
    
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "Logbook file does not exist, starting with empty logbook:" << filePath;
        m_entries.clear();
        return true; // Not an error, just means new user
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open logbook file for reading:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse logbook JSON:" << error.errorString();
        return false;
    }
    
    QJsonObject rootObject = doc.object();
    QJsonArray entriesArray = rootObject["entries"].toArray();
    
    m_entries.clear();
    for (const QJsonValue& value : entriesArray) {
        LogbookEntry entry;
        entry.fromJson(value.toObject());
        m_entries.append(entry);
    }
    
    // Sort entries by date created (newest first)
    std::sort(m_entries.begin(), m_entries.end(), [](const LogbookEntry& a, const LogbookEntry& b) {
        return a.dateCreated > b.dateCreated;
    });
    
    qDebug() << "Logbook loaded successfully from:" << filePath << "(" << m_entries.size() << "entries)";
    return true;
}

QString LogbookManager::saveImage(const QString& sourceImagePath, const QString& entryId)
{
    if (m_currentUser.isEmpty()) {
        qDebug() << "Cannot save image: no current user set";
        return QString();
    }
    
    QFileInfo sourceInfo(sourceImagePath);
    if (!sourceInfo.exists() || !sourceInfo.isFile()) {
        qDebug() << "Source image does not exist:" << sourceImagePath;
        return QString();
    }
    
    // Create user images directory
    QString userImagesPath = getUserImagesPath();
    QDir dir(userImagesPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create user images directory:" << userImagesPath;
            return QString();
        }
    }
    
    // Generate unique filename
    QString fileName = generateImageFileName(entryId, sourceInfo.fileName());
    QString destPath = dir.absoluteFilePath(fileName);
    
    // Copy the image
    if (QFile::copy(sourceImagePath, destPath)) {
        qDebug() << "Image saved successfully:" << destPath;
        return destPath;
    } else {
        qDebug() << "Failed to copy image from" << sourceImagePath << "to" << destPath;
        return QString();
    }
}

bool LogbookManager::deleteImage(const QString& imagePath)
{
    QFile file(imagePath);
    if (file.exists()) {
        bool success = file.remove();
        if (success) {
            qDebug() << "Image deleted successfully:" << imagePath;
        } else {
            qDebug() << "Failed to delete image:" << imagePath;
        }
        return success;
    }
    
    qDebug() << "Image file does not exist:" << imagePath;
    return true; // Not an error if file doesn't exist
}

QString LogbookManager::getUserImagesPath() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dataPath).absoluteFilePath(QString("users/%1/logbook_images").arg(m_currentUser));
}

QString LogbookManager::getUserLogbookFilePath() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dataPath).absoluteFilePath(QString("users/%1/logbook.json").arg(m_currentUser));
}

QString LogbookManager::generateImageFileName(const QString& entryId, const QString& originalFileName) const
{
    QFileInfo fileInfo(originalFileName);
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    return QString("%1_%2_%3.%4")
           .arg(entryId)
           .arg(timestamp)
           .arg(fileInfo.baseName())
           .arg(fileInfo.completeSuffix());
}
