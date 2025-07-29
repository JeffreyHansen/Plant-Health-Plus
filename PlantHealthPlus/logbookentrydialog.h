#ifndef LOGBOOKENTRY_DIALOG_H
#define LOGBOOKENTRY_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImageReader>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include "logbookmanager.h"

class LogbookEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogbookEntryDialog(LogbookManager* manager, QWidget *parent = nullptr);
    explicit LogbookEntryDialog(const LogbookEntry& entry, LogbookManager* manager, QWidget *parent = nullptr);
    
    LogbookEntry getEntry() const;
    void setEntry(const LogbookEntry& entry);

private slots:
    void onAddImageClicked();
    void onRemoveImageClicked();

private:
    void setupUI();
    void updateImagesDisplay();
    void addImageToDisplay(const QString& imagePath);
    void clearImagesDisplay();
    bool isValidImageFile(const QString& filePath) const;
    
    QLineEdit* m_titleEdit;
    QTextEdit* m_contentEdit;
    QPushButton* m_addImageButton;
    QScrollArea* m_imagesScrollArea;
    QWidget* m_imagesContainer;
    QGridLayout* m_imagesLayout;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
    QLabel* m_dateCreatedLabel;
    QLabel* m_dateModifiedLabel;
    
    QStringList m_imagePaths;
    LogbookEntry m_originalEntry;
    bool m_isEditing;
    LogbookManager* m_logbookManager;
    
    static const int IMAGE_THUMBNAIL_SIZE = 150;
    static const int IMAGES_PER_ROW = 4;
};

#endif // LOGBOOKENTRY_DIALOG_H
