#include "logbookentrydialog.h"

LogbookEntryDialog::LogbookEntryDialog(LogbookManager* manager, QWidget *parent)
    : QDialog(parent), m_isEditing(false), m_logbookManager(manager)
{
    setupUI();
    setWindowTitle("New Logbook Entry");
    
    // Set current date for new entry
    QDateTime now = QDateTime::currentDateTime();
    m_dateCreatedLabel->setText(QString("Created: %1").arg(now.toString("MMM dd, yyyy hh:mm")));
    m_dateModifiedLabel->setText(QString("Modified: %1").arg(now.toString("MMM dd, yyyy hh:mm")));
}

LogbookEntryDialog::LogbookEntryDialog(const LogbookEntry& entry, LogbookManager* manager, QWidget *parent)
    : QDialog(parent), m_originalEntry(entry), m_isEditing(true), m_logbookManager(manager)
{
    setupUI();
    setEntry(entry);
    setWindowTitle("Edit Logbook Entry");
}

void LogbookEntryDialog::setupUI()
{
    setModal(true);
    setFixedSize(700, 600);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title section
    QLabel* titleLabel = new QLabel("Title:", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setPlaceholderText("Enter entry title...");
    m_titleEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid palette(mid); border-radius: 4px; font-size: 14px; }");
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_titleEdit);
    
    // Content section
    QLabel* contentLabel = new QLabel("Content:", this);
    contentLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    m_contentEdit = new QTextEdit(this);
    m_contentEdit->setPlaceholderText("Write your entry here...");
    m_contentEdit->setStyleSheet("QTextEdit { padding: 8px; border: 1px solid palette(mid); border-radius: 4px; font-size: 14px; }");
    m_contentEdit->setMinimumHeight(200);
    
    mainLayout->addWidget(contentLabel);
    mainLayout->addWidget(m_contentEdit);
    
    // Images section
    QHBoxLayout* imagesHeaderLayout = new QHBoxLayout();
    QLabel* imagesLabel = new QLabel("Images:", this);
    imagesLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    
    m_addImageButton = new QPushButton("Add Image", this);
    m_addImageButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #28a745;"
        "    color: white;"
        "    padding: 8px 16px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #218838;"
        "}"
    );
    connect(m_addImageButton, &QPushButton::clicked, this, &LogbookEntryDialog::onAddImageClicked);
    
    imagesHeaderLayout->addWidget(imagesLabel);
    imagesHeaderLayout->addStretch();
    imagesHeaderLayout->addWidget(m_addImageButton);
    
    mainLayout->addLayout(imagesHeaderLayout);
    
    // Images display area
    m_imagesScrollArea = new QScrollArea(this);
    m_imagesScrollArea->setMinimumHeight(120);
    m_imagesScrollArea->setMaximumHeight(200);
    m_imagesScrollArea->setWidgetResizable(true);
    m_imagesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_imagesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_imagesScrollArea->setStyleSheet("QScrollArea { border: 1px solid palette(mid); border-radius: 4px; background-color: palette(base); }");
    
    m_imagesContainer = new QWidget();
    m_imagesLayout = new QGridLayout(m_imagesContainer);
    m_imagesLayout->setSpacing(10);
    m_imagesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    m_imagesScrollArea->setWidget(m_imagesContainer);
    mainLayout->addWidget(m_imagesScrollArea);
    
    // Date information
    QFrame* dateFrame = new QFrame(this);
    dateFrame->setStyleSheet("QFrame { background-color: palette(alternate-base); border-radius: 4px; padding: 8px; }");
    QVBoxLayout* dateLayout = new QVBoxLayout(dateFrame);
    dateLayout->setSpacing(4);
    
    m_dateCreatedLabel = new QLabel(this);
    m_dateCreatedLabel->setStyleSheet("QLabel { font-size: 12px; color: palette(dark); }");
    
    m_dateModifiedLabel = new QLabel(this);
    m_dateModifiedLabel->setStyleSheet("QLabel { font-size: 12px; color: palette(dark); }");
    
    dateLayout->addWidget(m_dateCreatedLabel);
    dateLayout->addWidget(m_dateModifiedLabel);
    
    mainLayout->addWidget(dateFrame);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border: 1px solid palette(mid);"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: palette(light);"
        "}"
    );
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    m_saveButton = new QPushButton("Save", this);
    m_saveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #007bff;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0056b3;"
        "}"
    );
    connect(m_saveButton, &QPushButton::clicked, this, &QDialog::accept);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Initially hide images area if no images
    updateImagesDisplay();
}

LogbookEntry LogbookEntryDialog::getEntry() const
{
    LogbookEntry entry;
    
    if (m_isEditing) {
        entry = m_originalEntry;
        entry.dateModified = QDateTime::currentDateTime();
    } else {
        entry.id = QString::number(QDateTime::currentMSecsSinceEpoch());
        entry.dateCreated = QDateTime::currentDateTime();
        entry.dateModified = entry.dateCreated;
    }
    
    entry.title = m_titleEdit->text().trimmed();
    entry.content = m_contentEdit->toPlainText();
    entry.imagePaths = m_imagePaths;
    
    return entry;
}

void LogbookEntryDialog::setEntry(const LogbookEntry& entry)
{
    m_originalEntry = entry;
    m_titleEdit->setText(entry.title);
    m_contentEdit->setPlainText(entry.content);
    m_imagePaths = entry.imagePaths;
    
    m_dateCreatedLabel->setText(QString("Created: %1").arg(entry.dateCreated.toString("MMM dd, yyyy hh:mm")));
    m_dateModifiedLabel->setText(QString("Modified: %1").arg(entry.dateModified.toString("MMM dd, yyyy hh:mm")));
    
    updateImagesDisplay();
}

void LogbookEntryDialog::onAddImageClicked()
{
    QStringList supportedFormats;
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    for (const QByteArray& format : formats) {
        supportedFormats << QString("*.%1").arg(QString::fromLatin1(format).toLower());
    }
    
    QString filter = QString("Image Files (%1)").arg(supportedFormats.join(" "));
    
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "Select Images",
        QString(),
        filter
    );
    
    for (const QString& filePath : filePaths) {
        if (isValidImageFile(filePath) && !m_imagePaths.contains(filePath)) {
            // Generate a temporary entry ID for new entries, or use the existing ID for edits
            QString entryId = m_isEditing ? m_originalEntry.id : QString("temp_%1").arg(QDateTime::currentMSecsSinceEpoch());
            
            // Copy the image to the app's data directory to prevent accidental deletion of originals
            if (m_logbookManager) {
                QString copiedImagePath = m_logbookManager->saveImage(filePath, entryId);
                if (!copiedImagePath.isEmpty()) {
                    m_imagePaths.append(copiedImagePath);
                } else {
                    qDebug() << "Failed to copy image:" << filePath;
                }
            } else {
                // Fallback: store original path if manager is not available
                m_imagePaths.append(filePath);
            }
        }
    }
    
    updateImagesDisplay();
}

void LogbookEntryDialog::onRemoveImageClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString imagePath = button->property("imagePath").toString();
    m_imagePaths.removeAll(imagePath);
    updateImagesDisplay();
}

void LogbookEntryDialog::updateImagesDisplay()
{
    clearImagesDisplay();
    
    for (int i = 0; i < m_imagePaths.size(); ++i) {
        addImageToDisplay(m_imagePaths[i]);
    }
    
    // Show/hide the images area based on whether there are images
    m_imagesScrollArea->setVisible(!m_imagePaths.isEmpty());
}

void LogbookEntryDialog::addImageToDisplay(const QString& imagePath)
{
    QFrame* imageFrame = new QFrame(m_imagesContainer);
    imageFrame->setFixedSize(IMAGE_THUMBNAIL_SIZE + 20, IMAGE_THUMBNAIL_SIZE + 50);
    imageFrame->setStyleSheet("QFrame { border: 1px solid palette(mid); border-radius: 8px; background-color: palette(base); }");
    
    QVBoxLayout* frameLayout = new QVBoxLayout(imageFrame);
    frameLayout->setSpacing(5);
    frameLayout->setContentsMargins(10, 10, 10, 10);
    
    // Image thumbnail
    QLabel* imageLabel = new QLabel(imageFrame);
    imageLabel->setFixedSize(IMAGE_THUMBNAIL_SIZE, IMAGE_THUMBNAIL_SIZE);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("QLabel { border: none; background-color: palette(alternate-base); border-radius: 4px; }");
    
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        QPixmap scaledPixmap = pixmap.scaled(IMAGE_THUMBNAIL_SIZE, IMAGE_THUMBNAIL_SIZE, 
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
    } else {
        imageLabel->setText("Invalid\nImage");
        imageLabel->setStyleSheet("QLabel { color: palette(dark); font-size: 10px; }");
    }
    
    // Remove button
    QPushButton* removeButton = new QPushButton("×", imageFrame);
    removeButton->setFixedSize(20, 20);
    removeButton->setProperty("imagePath", imagePath);
    removeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c82333;"
        "}"
    );
    connect(removeButton, &QPushButton::clicked, this, &LogbookEntryDialog::onRemoveImageClicked);
    
    frameLayout->addWidget(imageLabel);
    frameLayout->addWidget(removeButton, 0, Qt::AlignCenter);
    
    // Calculate grid position
    int row = m_imagePaths.indexOf(imagePath) / IMAGES_PER_ROW;
    int col = m_imagePaths.indexOf(imagePath) % IMAGES_PER_ROW;
    
    m_imagesLayout->addWidget(imageFrame, row, col);
}

void LogbookEntryDialog::clearImagesDisplay()
{
    while (QLayoutItem* item = m_imagesLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

bool LogbookEntryDialog::isValidImageFile(const QString& filePath) const
{
    QImageReader reader(filePath);
    return reader.canRead();
}
