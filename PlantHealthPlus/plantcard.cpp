#include "plantcard.h"
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QMap>

PlantCard::PlantCard(const PlantData& plantData, QWidget *parent)
    : QFrame(parent)
    , m_plantData(plantData)
    , m_expanded(false)
    , m_animation(nullptr)
{
    setupUI();
    updateWateringStatus();
    
    // Set up card styling
    setFrameStyle(QFrame::Box);
    setLineWidth(1);
    setFixedSize(CARD_WIDTH, CARD_BASIC_HEIGHT);
    
    // Add drop shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(2, 2);
    setGraphicsEffect(shadow);
    
    // Set cursor to pointer to indicate clickable
    setCursor(Qt::PointingHandCursor);
    
    // Style the card with CSS using dynamic color
    QString backgroundColor = getColorHexCode(m_plantData.cardColor);
    QString hoverColor = backgroundColor == "#FFFFFF" ? "#f8fff8" : backgroundColor;  // Keep same hover color if not white
    
    setStyleSheet(QString(
        "PlantCard {"
        "    background-color: %1;"
        "    border: 1px solid #e0e0e0;"
        "    border-radius: 8px;"
        "}"
        "PlantCard:hover {"
        "    border: 2px solid #4CAF50;"
        "    background-color: %2;"
        "}"
    ).arg(backgroundColor, hoverColor));
}

PlantCard::~PlantCard()
{
    if (m_animation) {
        m_animation->deleteLater();
    }
}

void PlantCard::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(12, 12, 12, 12);
    m_mainLayout->setSpacing(8);
    
    setupBasicInfo();
    setupExpandedInfo();
    
    // Initially hide expanded info
    m_expandedWidget->hide();
}

void PlantCard::setupBasicInfo()
{
    m_basicWidget = new QWidget(this);
    QVBoxLayout* basicLayout = new QVBoxLayout(m_basicWidget);
    basicLayout->setContentsMargins(0, 0, 0, 0);
    basicLayout->setSpacing(6);
    
    // Top row with image and basic info
    QHBoxLayout* topRow = new QHBoxLayout();
    
    // Plant image
    m_plantImage = new QLabel();
    m_plantImage->setFixedSize(60, 60);
    m_plantImage->setScaledContents(true);
    m_plantImage->setStyleSheet("border: 1px solid #ddd; border-radius: 30px;");
    
    // Load plant image or use default
    QPixmap plantPixmap;
    if (!m_plantData.imagePath.isEmpty() && plantPixmap.load(m_plantData.imagePath)) {
        m_plantImage->setPixmap(plantPixmap);
    } else {
        // Create a simple default plant icon
        m_plantImage->setText("P");
        m_plantImage->setAlignment(Qt::AlignCenter);
        m_plantImage->setStyleSheet(
            "background-color: #e8f5e8; "
            "border: 1px solid #4CAF50; "
            "border-radius: 30px; "
            "font-size: 24px; "
            "font-weight: bold; "
            "color: #4CAF50;"
        );
    }
    
    // Plant info column
    QVBoxLayout* infoColumn = new QVBoxLayout();
    
    m_plantName = new QLabel(m_plantData.name);
    m_plantName->setFont(QFont("Arial", 12, QFont::Bold));
    m_plantName->setStyleSheet("color: #2E7D32;");
    
    m_plantCategory = new QLabel(m_plantData.category);
    m_plantCategory->setFont(QFont("Arial", 9));
    m_plantCategory->setStyleSheet("color: #666;");
    
    infoColumn->addWidget(m_plantName);
    infoColumn->addWidget(m_plantCategory);
    infoColumn->addStretch();
    
    topRow->addWidget(m_plantImage);
    topRow->addLayout(infoColumn);
    topRow->addStretch();
    
    // Watering status and button row
    QHBoxLayout* waterRow = new QHBoxLayout();
    
    m_wateringStatus = new QLabel();
    m_wateringStatus->setFont(QFont("Arial", 9));
    
    m_waterButton = new QPushButton("Water");
    m_waterButton->setFixedSize(60, 30);
    m_waterButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    border: none;"
        "    border-radius: 15px;"
        "    color: white;"
        "    font-size: 10px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0D47A1;"
        "}"
    );
    
    connect(m_waterButton, &QPushButton::clicked, this, &PlantCard::markWatered);
    
    waterRow->addWidget(m_wateringStatus);
    waterRow->addStretch();
    waterRow->addWidget(m_waterButton);
    
    basicLayout->addLayout(topRow);
    basicLayout->addLayout(waterRow);
    
    m_mainLayout->addWidget(m_basicWidget);
}

void PlantCard::setupExpandedInfo()
{
    m_expandedWidget = new QWidget(this);
    QVBoxLayout* expandedLayout = new QVBoxLayout(m_expandedWidget);
    expandedLayout->setContentsMargins(0, 8, 0, 0);
    expandedLayout->setSpacing(6);
    
    // Separator line
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("background-color: #e0e0e0;");
    expandedLayout->addWidget(separator);
    
    // Detailed information
    m_detailedNotes = new QLabel(m_plantData.notes);
    m_detailedNotes->setWordWrap(true);
    m_detailedNotes->setFont(QFont("Arial", 9));
    m_detailedNotes->setStyleSheet("color: #444; padding: 4px;");
    
    m_lightRequirement = new QLabel(QString("Light: %1").arg(m_plantData.lightRequirement));
    m_lightRequirement->setFont(QFont("Arial", 9, QFont::Bold));
    m_lightRequirement->setStyleSheet("color: #444; margin: 2px; padding: 2px;");
    
    m_soilType = new QLabel(QString("Soil: %1").arg(m_plantData.soilType));
    m_soilType->setFont(QFont("Arial", 9, QFont::Bold));
    m_soilType->setStyleSheet("color: #444; margin: 2px; padding: 2px;");
    
    m_fertilizerInfo = new QLabel(QString("Fertilizer: %1").arg(m_plantData.fertilizer));
    m_fertilizerInfo->setFont(QFont("Arial", 9, QFont::Bold));
    m_fertilizerInfo->setStyleSheet("color: #444; margin: 2px; padding: 2px;");
    
    m_dateAdded = new QLabel(QString("Added: %1").arg(m_plantData.dateAdded.toString("MMM dd, yyyy")));
    m_dateAdded->setFont(QFont("Arial", 9));
    m_dateAdded->setStyleSheet("color: #666;");
    
    m_lastWateredLabel = new QLabel(QString("Last Watered: %1").arg(m_plantData.lastWatered.toString("MMM dd, yyyy hh:mm AP")));
    m_lastWateredLabel->setFont(QFont("Arial", 9));
    m_lastWateredLabel->setStyleSheet("color: #666;");
    
    // Format the watering interval to handle fractional days
    QString intervalText;
    if (m_plantData.wateringIntervalDays < 1.0) {
        double hoursInterval = m_plantData.wateringIntervalDays * 24.0;
        intervalText = QString("Watering Interval: Every %1 hours").arg(QString::number(hoursInterval, 'f', 1));
    } else if (m_plantData.wateringIntervalDays == 1.0) {
        intervalText = "Watering Interval: Daily";
    } else if (m_plantData.wateringIntervalDays == (int)m_plantData.wateringIntervalDays) {
        intervalText = QString("Watering Interval: Every %1 days").arg((int)m_plantData.wateringIntervalDays);
    } else {
        intervalText = QString("Watering Interval: Every %1 days").arg(QString::number(m_plantData.wateringIntervalDays, 'f', 1));
    }
    
    m_wateringIntervalLabel = new QLabel(intervalText);
    m_wateringIntervalLabel->setFont(QFont("Arial", 9));
    m_wateringIntervalLabel->setStyleSheet("color: #666;");
    
    // Action buttons
    QHBoxLayout* buttonRow = new QHBoxLayout();
    
    m_editButton = new QPushButton("Edit");
    m_editButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #FF9800;"
        "    border: none;"
        "    border-radius: 4px;"
        "    color: white;"
        "    padding: 6px 12px;"
        "    font-size: 10px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F57C00;"
        "}"
    );
    
    m_deleteButton = new QPushButton("Delete");
    m_deleteButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #F44336;"
        "    border: none;"
        "    border-radius: 4px;"
        "    color: white;"
        "    padding: 6px 12px;"
        "    font-size: 10px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #D32F2F;"
        "}"
    );
    
    connect(m_editButton, &QPushButton::clicked, this, &PlantCard::onEditClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &PlantCard::onDeleteClicked);
    
    buttonRow->addWidget(m_editButton);
    buttonRow->addStretch();
    buttonRow->addWidget(m_deleteButton);
    
    expandedLayout->addWidget(m_detailedNotes);
    expandedLayout->addWidget(m_lightRequirement);
    expandedLayout->addWidget(m_soilType);
    expandedLayout->addWidget(m_fertilizerInfo);
    expandedLayout->addWidget(m_dateAdded);
    expandedLayout->addWidget(m_lastWateredLabel);
    expandedLayout->addWidget(m_wateringIntervalLabel);
    expandedLayout->addLayout(buttonRow);
    
    m_mainLayout->addWidget(m_expandedWidget);
}

void PlantCard::updateWateringStatus()
{
    QDateTime now = QDateTime::currentDateTime();
    
    // Calculate hours since last watered for more precise timing
    qint64 hoursSinceWatered = m_plantData.lastWatered.secsTo(now) / 3600;
    double hoursUntilNextWatering = (m_plantData.wateringIntervalDays * 24.0) - hoursSinceWatered;
    
    QString statusText;
    QString statusColor;
    
    if (hoursUntilNextWatering <= 0) {
        double hoursOverdue = -hoursUntilNextWatering;
        if (hoursOverdue < 24) {
            statusText = QString("Needs water! (%1 hrs overdue)").arg(QString::number(hoursOverdue, 'f', 1));
        } else {
            double daysOverdue = hoursOverdue / 24.0;
            statusText = QString("Needs water! (%1 days overdue)").arg(QString::number(daysOverdue, 'f', 1));
        }
        statusColor = "#F44336"; // Red
    } else if (hoursUntilNextWatering <= 2) {
        statusText = QString("Water soon (%1 hrs)").arg(QString::number(hoursUntilNextWatering, 'f', 1));
        statusColor = "#FF9800"; // Orange
    } else if (hoursUntilNextWatering <= 24) {
        statusText = QString("Water in %1 hrs").arg(QString::number(hoursUntilNextWatering, 'f', 1));
        statusColor = "#FFC107"; // Amber
    } else {
        double daysUntilWatering = hoursUntilNextWatering / 24.0;
        statusText = QString("Water in %1 days").arg(QString::number(daysUntilWatering, 'f', 1));
        statusColor = "#4CAF50"; // Green
    }
    
    m_wateringStatus->setText(statusText);
    m_wateringStatus->setStyleSheet(QString("color: %1; font-weight: bold;").arg(statusColor));
}

void PlantCard::updatePlantData(const PlantData& data)
{
    m_plantData = data;
    
    // Update UI elements
    m_plantName->setText(m_plantData.name);
    m_plantCategory->setText(m_plantData.category);
    m_detailedNotes->setText(m_plantData.notes);
    m_lightRequirement->setText(QString("Light: %1").arg(m_plantData.lightRequirement));
    m_soilType->setText(QString("Soil: %1").arg(m_plantData.soilType));
    m_fertilizerInfo->setText(QString("Fertilizer: %1").arg(m_plantData.fertilizer));
    m_dateAdded->setText(QString("Added: %1").arg(m_plantData.dateAdded.toString("MMM dd, yyyy")));
    m_lastWateredLabel->setText(QString("Last Watered: %1").arg(m_plantData.lastWatered.toString("MMM dd, yyyy hh:mm AP")));
    
    // Format the watering interval to handle fractional days
    QString intervalText;
    if (m_plantData.wateringIntervalDays < 1.0) {
        double hoursInterval = m_plantData.wateringIntervalDays * 24.0;
        intervalText = QString("Watering Interval: Every %1 hours").arg(QString::number(hoursInterval, 'f', 1));
    } else if (m_plantData.wateringIntervalDays == 1.0) {
        intervalText = "Watering Interval: Daily";
    } else if (m_plantData.wateringIntervalDays == (int)m_plantData.wateringIntervalDays) {
        intervalText = QString("Watering Interval: Every %1 days").arg((int)m_plantData.wateringIntervalDays);
    } else {
        intervalText = QString("Watering Interval: Every %1 days").arg(QString::number(m_plantData.wateringIntervalDays, 'f', 1));
    }
    m_wateringIntervalLabel->setText(intervalText);
    
    // Update plant image
    QPixmap plantPixmap;
    if (!m_plantData.imagePath.isEmpty() && plantPixmap.load(m_plantData.imagePath)) {
        m_plantImage->setPixmap(plantPixmap);
    }
    
    // Update card color if it has changed
    QString backgroundColor = getColorHexCode(m_plantData.cardColor);
    QString hoverColor = backgroundColor == "#FFFFFF" ? "#f8fff8" : backgroundColor;
    
    setStyleSheet(QString(
        "PlantCard {"
        "    background-color: %1;"
        "    border: 1px solid #e0e0e0;"
        "    border-radius: 8px;"
        "}"
        "PlantCard:hover {"
        "    border: 2px solid #4CAF50;"
        "    background-color: %2;"
        "}"
    ).arg(backgroundColor, hoverColor));
    
    updateWateringStatus();
}

void PlantCard::setExpanded(bool expanded)
{
    if (m_expanded != expanded) {
        m_expanded = expanded;
        animateExpansion();
        emit cardExpanded(this, m_expanded);  // Emit signal when expansion state changes
    }
}

void PlantCard::toggleExpanded()
{
    setExpanded(!m_expanded);
    emit cardClicked();
}

void PlantCard::markWatered()
{
    // Show confirmation dialog
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Confirm Watering", 
        QString("Mark '%1' as watered today?").arg(m_plantData.name),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes
    );
    
    if (reply == QMessageBox::Yes) {
        QDateTime oldWatered = m_plantData.lastWatered;
        m_plantData.lastWatered = QDateTime::currentDateTime();
        
        qDebug() << "Plant watered - Old date:" << oldWatered.toString() 
                 << "New date:" << m_plantData.lastWatered.toString();
        
        updateWateringStatus();
        
        // Update the last watered label in expanded view if it exists
        if (m_lastWateredLabel) {
            m_lastWateredLabel->setText(QString("Last Watered: %1").arg(m_plantData.lastWatered.toString("MMM dd, yyyy hh:mm AP")));
        }
        
        emit wateringUpdated();
    }
}

void PlantCard::animateExpansion()
{
    if (m_animation) {
        m_animation->stop();
        m_animation->deleteLater();
    }
    
    m_animation = new QPropertyAnimation(this, "maximumHeight");
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutQuart);
    
    if (m_expanded) {
        m_expandedWidget->show();
        m_animation->setStartValue(CARD_BASIC_HEIGHT);
        m_animation->setEndValue(CARD_EXPANDED_HEIGHT);
        setFixedHeight(CARD_EXPANDED_HEIGHT);
    } else {
        m_animation->setStartValue(CARD_EXPANDED_HEIGHT);
        m_animation->setEndValue(CARD_BASIC_HEIGHT);
        connect(m_animation, &QPropertyAnimation::finished, [this]() {
            m_expandedWidget->hide();
            setFixedHeight(CARD_BASIC_HEIGHT);
        });
    }
    
    m_animation->start();
}

void PlantCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // Don't toggle if clicking on buttons
        QWidget* clickedWidget = childAt(event->pos());
        if (clickedWidget != m_waterButton && clickedWidget != m_editButton && clickedWidget != m_deleteButton) {
            toggleExpanded();
        }
    }
    QFrame::mousePressEvent(event);
}

void PlantCard::enterEvent(QEnterEvent* event)
{
    // Add hover effect is handled by CSS
    QFrame::enterEvent(event);
}

void PlantCard::leaveEvent(QEvent* event)
{
    // Remove hover effect is handled by CSS
    QFrame::leaveEvent(event);
}

void PlantCard::onEditClicked()
{
    emit editRequested();
}

void PlantCard::onDeleteClicked()
{
    emit deleteRequested();
}

QString PlantCard::getColorHexCode(const QString& colorName) const
{
    // Map color names to hex codes
    static const QMap<QString, QString> colorMap = {
        {"White", "#FFFFFF"},
        {"Light Gray", "#F5F5F5"},
        {"Light Blue", "#E3F2FD"},
        {"Light Green", "#E8F5E8"},
        {"Light Pink", "#FCE4EC"},
        {"Light Yellow", "#FFFDE7"},
        {"Light Purple", "#F3E5F5"},
        {"Light Orange", "#FFF3E0"},
        {"Light Red", "#FFEBEE"},
        {"Light Cyan", "#E0F7FA"}
    };
    
    return colorMap.value(colorName, "#FFFFFF");  // Default to white if color not found
}
