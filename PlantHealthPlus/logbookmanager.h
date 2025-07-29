#ifndef LOGBOOKMANAGER_H
#define LOGBOOKMANAGER_H

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

struct LogbookEntry {
    QString id;              // Unique identifier
    QString title;           // Entry title
    QString content;         // Text content
    QStringList imagePaths;  // Paths to associated images
    QDateTime dateCreated;   // When entry was created
    QDateTime dateModified;  // When entry was last modified
    
    LogbookEntry() : dateCreated(QDateTime::currentDateTime()), dateModified(QDateTime::currentDateTime()) {}
    
    LogbookEntry(const QString& entryTitle, const QString& entryContent)
        : title(entryTitle), content(entryContent), 
          dateCreated(QDateTime::currentDateTime()), 
          dateModified(QDateTime::currentDateTime()) {
        id = QString::number(QDateTime::currentMSecsSinceEpoch());
    }
    
    // Convert to/from JSON
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

class LogbookManager
{
public:
    LogbookManager();
    
    // User management
    void setCurrentUser(const QString& username);
    QString getCurrentUser() const { return m_currentUser; }
    
    // Entry management
    bool addEntry(const LogbookEntry& entry);
    bool updateEntry(const QString& entryId, const LogbookEntry& entry);
    bool deleteEntry(const QString& entryId);
    LogbookEntry getEntry(const QString& entryId) const;
    QList<LogbookEntry> getAllEntries() const;
    QStringList getEntryIds() const;
    
    // Data persistence
    bool saveToFile();
    bool loadFromFile();
    
    // Image management
    QString saveImage(const QString& sourceImagePath, const QString& entryId);
    bool deleteImage(const QString& imagePath);
    void cleanupOrphanedImages(const QString& entryId, const QStringList& oldImagePaths, const QStringList& newImagePaths);
    QString getUserImagesPath() const;
    
private:
    QString getUserLogbookFilePath() const;
    QString generateImageFileName(const QString& entryId, const QString& originalFileName) const;
    
    QString m_currentUser;
    QList<LogbookEntry> m_entries;
};

#endif // LOGBOOKMANAGER_H
