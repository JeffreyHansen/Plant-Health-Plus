#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QScrollArea>
#include <QScrollBar>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QPalette>
#include <QStyleHints>
#include <QDebug>
#include <QMap>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "addplantdialog.h"
#include "scraper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plantManager(nullptr)
    , m_plantsScrollArea(nullptr)
    , m_plantsContainer(nullptr)
    , m_plantsGridLayout(nullptr)
    , m_addPlantButton(nullptr)
{
    ui->setupUi(this);

    // Setup layout for condition buttons
    setupConditionLayout();
    
    // Set minimum window size to ensure at least 2 plants per row
    // Calculation: 2 cards (280px each) + 3 spacings (15px each) + scroll margins (40px) + sidebars (131px + 81px)
    int cardWidth = PlantCard::CARD_WIDTH; // 280px
    int spacing = 15;
    int scrollMargins = 40; // left + right margins for scroll area
    int leftSidebar = 131;   // Tab sidebar - fixed width
    int rightSidebar = 81;   // Conditions sidebar - fixed width
    
    int minWidthForPlants = (2 * cardWidth) + (3 * spacing) + scrollMargins; // 2 cards + spacings + margins = 645px
    int totalMinWidth = minWidthForPlants + leftSidebar + rightSidebar; // 645 + 131 + 81 = 857px
    int minHeight = PlantCard::CARD_EXPANDED_HEIGHT + 100; // Expanded card height + buffer = 480px
    
    setMinimumSize(totalMinWidth, minHeight);
    qDebug() << "Set minimum window size to:" << totalMinWidth << "x" << minHeight;
    
    // Setup responsive layout for central widget
    setupResponsiveLayout();

    // Initialize plant manager
    m_plantManager = new PlantManager(this);
    
    // Connect plant manager signals
    connect(m_plantManager, &PlantManager::plantAdded, this, &MainWindow::onPlantAdded);
    connect(m_plantManager, &PlantManager::plantRemoved, this, &MainWindow::onPlantRemoved);
    connect(m_plantManager, &PlantManager::plantUpdated, this, &MainWindow::onPlantUpdated);

    Scraper* scraper = new Scraper(this);
    connect(scraper, &Scraper::tempReady, this, [this](const QString& temp) {
        qDebug() << temp;
        ui->temp_label->setText(temp);
        
        // Parse temperature and update plant cards
        QString tempStr = temp;
        tempStr.remove("°").remove(" ");
        bool ok;
        double tempValue = tempStr.toDouble(&ok);
        if (ok) {
            m_currentTemp = tempValue;
            updateAllPlantCardsEnvironmentalData();
        }
    });

    connect(scraper, &Scraper::uvReady, this, [this](const QString& uv) {
        qDebug() << uv;
        ui->uv_label->setText(uv);
        
        // Parse UV index and update plant cards
        bool ok;
        double uvValue = uv.toDouble(&ok);
        if (ok) {
            m_currentUV = uvValue;
            updateAllPlantCardsEnvironmentalData();
        }
    });

    connect(scraper, &Scraper::humidReady, this, [this](const QString& humid) {
        qDebug() << humid;
        ui->humid_label->setText(humid);
        
        // Parse humidity and update plant cards
        QString humidStr = humid;
        humidStr.remove("%").remove(" ");
        bool ok;
        double humidValue = humidStr.toDouble(&ok);
        if (ok) {
            m_currentHumidity = humidValue;
            updateAllPlantCardsEnvironmentalData();
        }
    });

    // Periodically updates the temperature, humidity, and UV conditions to match current conditions
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scraper, &Scraper::getURL);
    timer->start(5000);
    
    // Get initial data immediately
    scraper->getURL();

    // Get the button group and stacked widget from the UI
    QButtonGroup* buttonGroup = ui->buttonGroup;
    QStackedWidget* stackedWidget = ui->stackedWidget;

    // Assign IDs to the buttons matching page indexes
    // These IDs correspond to the page indexes in the stacked widget
    buttonGroup->setId(ui->myplants_tab, 0);  // Tab 0 -> MyPlants (index 0)
    buttonGroup->setId(ui->logbook_tab, 1);  // Tab 1 -> LogBook (index 1)
    buttonGroup->setId(ui->plantpedia_tab, 2);  // Tab 2 -> PlantPedia (index 2)

    // Connect button clicks to custom slot using new-style syntax
    connect(buttonGroup, &QButtonGroup::idClicked,
            this, &MainWindow::onTabButtonClicked);

    // Also try connecting individual buttons as a backup
    connect(ui->myplants_tab, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->logbook_tab, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->plantpedia_tab, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(2); });

    // Set the initial state
    ui->myplants_tab->setChecked(true);
    stackedWidget->setCurrentIndex(0);
    
    // Setup My Plants page
    setupMyPlantsPage();
    
    // Apply initial theme
    updateScrollAreaTheme();
    
    // Connect to theme changes
    connect(QApplication::styleHints(), &QStyleHints::colorSchemeChanged,
            this, &MainWindow::onThemeChanged);
    
    // Setup resize timer
    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setSingleShot(true);
    m_resizeTimer->setInterval(100); // 100ms delay
    connect(m_resizeTimer, &QTimer::timeout, this, &MainWindow::onResizeTimeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMyPlantsPage()
{
    // Find the My Plants page widget
    QWidget* myPlantsPage = ui->stackedWidget->widget(0); // Assuming index 0 is My Plants
    if (!myPlantsPage) {
        return;
    }
    
    // Clear existing layout if any
    delete myPlantsPage->layout();
    
    // Create main layout for the page
    QVBoxLayout* pageLayout = new QVBoxLayout(myPlantsPage);
    pageLayout->setContentsMargins(20, 20, 20, 20);
    pageLayout->setSpacing(15);
    
    // Create header with title and add button
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    QLabel* titleLabel = new QLabel("My Plants");
    titleLabel->setFont(QFont("Arial", 18, QFont::Bold));
    titleLabel->setStyleSheet("color: #2E7D32;");
    
    m_addPlantButton = new QPushButton("+ Add Plant");
    m_addPlantButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );
    
    connect(m_addPlantButton, &QPushButton::clicked, this, &MainWindow::onAddPlantClicked);
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_addPlantButton);
    
    // Create scroll area for plant cards
    m_plantsScrollArea = new QScrollArea();
    m_plantsScrollArea->setWidgetResizable(true);
    m_plantsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_plantsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Install event filter to handle scroll area resize events
    m_plantsScrollArea->installEventFilter(this);
    
    // Apply theme-aware styling
    updateScrollAreaTheme();
    
    // Create container widget for the grid
    m_plantsContainer = new QWidget();
    m_plantsContainer->setStyleSheet("background-color: transparent;");
    
    // Create grid layout for plant cards
    m_plantsGridLayout = new QGridLayout(m_plantsContainer);
    m_plantsGridLayout->setSpacing(15);
    m_plantsGridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    m_plantsScrollArea->setWidget(m_plantsContainer);
    
    // Add everything to the page
    pageLayout->addLayout(headerLayout);
    pageLayout->addWidget(m_plantsScrollArea);
    
    // Initially populate the plant grid
    updatePlantGrid();
}

void MainWindow::setupConditionLayout()
{
    if(!ui->Conditions->layout()) {
        ui->Conditions->setLayout(new QVBoxLayout);
    }

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->Conditions->layout());
    layout->setContentsMargins(1, 10, 1, 1);
    layout->setSpacing(10);

    auto makeShadowEffect = []() {
        QGraphicsDropShadowEffect* m_buttonEffect = new QGraphicsDropShadowEffect;
        m_buttonEffect->setBlurRadius(10);
        m_buttonEffect->setOffset(2, 2);
        m_buttonEffect->setColor(QColor(0, 0, 0, 100));
        return m_buttonEffect;
    };

    auto createConditionButton = [this]() -> QPushButton* {
        QPushButton* btn = new QPushButton(this);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn->setFixedHeight(100);
        btn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(102, 248, 135, 0.2);"
            "    color: #4CAF50;"
            "    border: 2px solid #4CAF50;"
            "    padding: 6px 12px;"
            "    border-radius: 15px;"
            "    font-weight: bold;"
            "    max-width: 60px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(102, 248, 135, 0.4);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(102, 248, 135, 0.6);"
            "}"
            );
        return btn;
    };

    QPushButton* m_tempButton = createConditionButton();
    m_tempButton->setObjectName("temp");
    m_tempButton->setIcon(QIcon(":/images/images/thermometer_temp.png"));
    m_tempButton->setIconSize(QSize(60, 60));
    m_tempButton->setGraphicsEffect(makeShadowEffect());
    QPushButton* m_uvButton = createConditionButton();
    m_uvButton->setObjectName("uv");
    m_uvButton->setIcon(QIcon(":/images/images/uv.png"));
    m_uvButton->setIconSize(QSize(60, 60));
    m_uvButton->setGraphicsEffect(makeShadowEffect());
    QPushButton* m_humidityButton = createConditionButton();
    m_humidityButton->setObjectName("humidity");
    m_humidityButton->setIcon(QIcon(":/images/images/humidity.png"));
    m_humidityButton->setIconSize(QSize(80, 80));
    m_humidityButton->setGraphicsEffect(makeShadowEffect());

    connect(m_tempButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);
    connect(m_uvButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);
    connect(m_humidityButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);

    QLabel* humidityLabel = new QLabel("Humidity");
    humidityLabel->setFont(QFont("Calbiri", 12, QFont::Bold));
    humidityLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");

    QLabel* tempLabel = new QLabel("Temp");
    tempLabel->setFont(QFont("Calbiri", 12, QFont::Bold));
    tempLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");

    QLabel* uvLabel = new QLabel("UV");
    uvLabel->setFont(QFont("Calbiri", 12, QFont::Bold));
    uvLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");

    layout->addWidget(tempLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_tempButton);
    layout->addWidget(uvLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_uvButton);
    layout->addWidget(humidityLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_humidityButton);

    // Add a stretch at the bottom to push buttons to top
    layout->addStretch();
}

void MainWindow::onTabButtonClicked(int id)
{
    // This slot receives the button ID and changes the page
    ui->stackedWidget->setCurrentIndex(id);
}

void MainWindow::onConditionClicked() {
    static QMap<ConditionType, QWidget*> openCharts;

    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString name = clickedButton->objectName();
    ConditionType type;

    if (name == "temp") {
        type = ConditionType::Temperature;
    } else if (name == "uv") {
        type = ConditionType::UV;
    } else if (name == "humidity") {
        type = ConditionType::Humidity;
    } else {
        qDebug() <<"Unknown condition, returning ungracefully.";
        return;
    }

    if (openCharts.contains(type)) {
        QWidget *existing = openCharts[type];
        if (existing && existing->isVisible()) {
            existing->raise();
            existing->activateWindow();
            return;
        } else {
            openCharts.remove(type); // clean stale pointer
        }
    }

    QWidget *chartWindow = new QWidget();
    chartWindow->resize(600, 400);
    chartWindow->setAttribute(Qt::WA_DeleteOnClose);

    // Create chart based on the type declared.
    ConditionChart *chartWidget = new ConditionChart(type, chartWindow);
    ForecastClient *client = new ForecastClient(chartWindow);

    connect(client, &ForecastClient::forecastsReady,
            chartWidget, &ConditionChart::updateChart);

    client->getURL();

    // Provide layout.
    QVBoxLayout *layout = new QVBoxLayout(chartWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartWidget);
    chartWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartWindow->setLayout(layout);

    openCharts[type] = chartWindow;

    connect(chartWindow, &QWidget::destroyed, [type]() {
        openCharts.remove(type);
    });

    QString title;
    switch(type) {
        case ConditionType::Temperature:
            title = "Temperature Chart";
            break;
        case ConditionType::UV:
            title = "UV Index Chart";
            break;
        case ConditionType::Humidity:
            title = "Humidity Index Chart";
            break;
        default:
            break;
    }

    chartWindow->setWindowTitle(title);
    chartWindow->show();
    chartWindow->raise();
    chartWindow->activateWindow();

    // Then, raise all other open charts (except this one)
    for (auto i = openCharts.begin(); i != openCharts.end(); ++i) {
        QWidget* w = i.value();
        if (w && w->isVisible()) {
            w->raise();
            w->activateWindow();
        }
    }
}

void MainWindow::onAddPlantClicked()
{
    AddPlantDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        PlantData newPlant = dialog.getPlantData();
        m_plantManager->addPlant(newPlant);
    }
}

void MainWindow::onPlantAdded(int index)
{
    updatePlantGrid();
}

void MainWindow::onPlantRemoved(int index)
{
    updatePlantGrid();
}

void MainWindow::onPlantUpdated(int index)
{
    // For plant updates, we need to refresh the entire grid because:
    // 1. Area changes might require regrouping plants
    // 2. Color changes require recreating the cards with new styles
    // 3. The index mapping might change due to grouping
    updatePlantGrid();
}

void MainWindow::onPlantCardEdit(PlantCard* card, int index)
{
    if (index >= 0 && index < m_plantManager->getPlantCount()) {
        PlantData currentData = m_plantManager->getPlant(index);
        AddPlantDialog dialog(currentData, this);
        
        if (dialog.exec() == QDialog::Accepted) {
            PlantData updatedData = dialog.getPlantData();
            // Preserve the dateAdded from the original plant
            updatedData.dateAdded = currentData.dateAdded;
            m_plantManager->updatePlant(index, updatedData);
        }
    }
}

void MainWindow::onPlantCardDelete(PlantCard* card, int index)
{
    // Create a custom dialog to avoid QMessageBox system icon loading issues
    QDialog dialog(this);
    dialog.setWindowTitle("Delete Plant");
    dialog.setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QLabel* label = new QLabel(QString("Are you sure you want to delete '%1'?").arg(m_plantManager->getPlant(index).name));
    label->setWordWrap(true);
    layout->addWidget(label);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* yesButton = new QPushButton("Yes");
    QPushButton* noButton = new QPushButton("No");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(noButton);
    buttonLayout->addWidget(yesButton);
    layout->addLayout(buttonLayout);
    
    // Connect buttons
    connect(yesButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(noButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    // Set No as default (focused button)
    noButton->setDefault(true);
    noButton->setFocus();
    
    if (dialog.exec() == QDialog::Accepted) {
        m_plantManager->removePlant(index);
    }
}

void MainWindow::onPlantCardExpanded(PlantCard* card, bool expanded)
{
    if (expanded && m_plantsScrollArea && card) {
        // Use a timer to allow the expansion animation to complete
        QTimer::singleShot(150, this, [this, card]() {
            // Get the card's position relative to the scroll area content
            QPoint cardPos = card->mapTo(m_plantsContainer, QPoint(0, 0));
            
            // Get the scroll area's viewport size
            QRect viewportRect = m_plantsScrollArea->viewport()->rect();
            int viewportHeight = viewportRect.height();
            int cardHeight = card->height();
            
            // Calculate the position to center the card in the viewport
            int targetY = cardPos.y() + (cardHeight / 2) - (viewportHeight / 2);
            
            // Ensure we don't scroll beyond the content bounds
            targetY = qMax(0, targetY);
            int maxScroll = m_plantsContainer->height() - viewportHeight;
            if (maxScroll > 0) {
                targetY = qMin(targetY, maxScroll);
            }
            
            // Set the scroll position to center the card
            m_plantsScrollArea->verticalScrollBar()->setValue(targetY);
            
            qDebug() << "Centering expanded card:"
                     << "Card position:" << cardPos.y()
                     << "Card height:" << cardHeight
                     << "Viewport height:" << viewportHeight
                     << "Target scroll:" << targetY;
        });
    }
}

void MainWindow::refreshPlantGrid()
{
    updatePlantGrid();
}

void MainWindow::updatePlantGrid()
{
    clearPlantGrid();
    
    int plantCount = m_plantManager->getPlantCount();
    if (plantCount == 0) {
        return;
    }
    
    // Group plants by house area
    QMap<QString, QList<QPair<PlantData, int>>> plantsByArea;  // area -> list of (plantData, originalIndex)
    
    for (int i = 0; i < plantCount; ++i) {
        PlantData plantData = m_plantManager->getPlant(i);
        QString area = plantData.houseArea.isEmpty() ? "Unassigned" : plantData.houseArea;
        plantsByArea[area].append(qMakePair(plantData, i));
    }

    // Calculate responsive columns based on available width
    int availableWidth = m_plantsScrollArea->viewport()->width();
    int cardWidth = PlantCard::CARD_WIDTH;
    int spacing = m_plantsGridLayout->spacing();
    
    // Ensure we always have at least 2 columns (minimum requirement)
    int maxPossibleColumns = (availableWidth - spacing) / (cardWidth + spacing);
    int columns = qMax(2, maxPossibleColumns);
    
    // If we can't fit 2 columns properly, still try with 2 (let them slightly overlap if needed)
    if (columns < 2 && availableWidth > cardWidth) {
        columns = 2;
    }
    
    int currentRow = 0;
    
    // Sort areas for consistent display
    QStringList sortedAreas = plantsByArea.keys();
    sortedAreas.sort();
    
    // Create plants grouped by area
    for (const QString& area : sortedAreas) {
        const QList<QPair<PlantData, int>>& plants = plantsByArea[area];
        
        // Add area header if there are multiple areas
        if (plantsByArea.size() > 1) {
            QLabel* areaHeader = new QLabel(area, m_plantsContainer);
            areaHeader->setStyleSheet(
                "QLabel {"
                "    font-size: 18px;"
                "    font-weight: bold;"
                "    color: #2E7D32;"
                "    background-color: #E8F5E8;"
                "    padding: 10px;"
                "    border-radius: 5px;"
                "    margin: 5px 0px;"
                "}"
            );
            areaHeader->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            
            // Add header spanning all columns
            m_plantsGridLayout->addWidget(areaHeader, currentRow, 0, 1, columns);
            currentRow++;
        }
        
        // Add plants in this area
        int plantsInArea = plants.size();
        for (int i = 0; i < plantsInArea; ++i) {
            const QPair<PlantData, int>& plantPair = plants[i];
            const PlantData& plantData = plantPair.first;
            int originalIndex = plantPair.second;
            
            PlantCard* card = createPlantCard(plantData, originalIndex);
            
            int row = currentRow + (i / columns);
            int col = i % columns;
            
            m_plantsGridLayout->addWidget(card, row, col);
            m_plantCards.append(card);
        }
        
        // Update current row for next area
        currentRow += (plantsInArea + columns - 1) / columns;  // Ceiling division
        
        // Add some space between areas
        if (area != sortedAreas.last()) {
            currentRow++;
        }
    }
    
    // Add stretch to push cards to top-left
    m_plantsGridLayout->setRowStretch(m_plantsGridLayout->rowCount(), 1);
    m_plantsGridLayout->setColumnStretch(columns, 1);
}

void MainWindow::clearPlantGrid()
{
    // Clear all widgets from the grid layout (both cards and area headers)
    while (QLayoutItem* item = m_plantsGridLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            // Check if it's a PlantCard and remove from our tracking list
            if (PlantCard* card = qobject_cast<PlantCard*>(widget)) {
                m_plantCards.removeAll(card);
            }
            widget->deleteLater();
        }
        delete item;
    }
    m_plantCards.clear();
}

PlantCard* MainWindow::createPlantCard(const PlantData& plantData, int index)
{
    PlantCard* card = new PlantCard(plantData, m_plantsContainer);
    
    // Connect card signals
    connect(card, &PlantCard::editRequested, [this, card, index]() {
        onPlantCardEdit(card, index);
    });
    
    connect(card, &PlantCard::deleteRequested, [this, card, index]() {
        onPlantCardDelete(card, index);
    });
    
    connect(card, &PlantCard::wateringUpdated, [this, card, index]() {
        // Update the plant data in the manager when watering is updated
        PlantData updatedData = card->getPlantData();
        m_plantManager->updatePlant(index, updatedData);
    });
    
    connect(card, &PlantCard::cardExpanded, [this](PlantCard* expandedCard, bool expanded) {
        onPlantCardExpanded(expandedCard, expanded);
    });
    
    // Initialize with current environmental data
    card->updateEnvironmentalData(m_currentTemp, m_currentHumidity, m_currentUV);
    
    return card;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    
    // Start/restart the resize timer to avoid too frequent updates
    if (m_resizeTimer) {
        m_resizeTimer->start();
    }
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    
    // Set the initial window size when the window is first shown
    // This ensures the UI layout is fully established
    static bool firstShow = true;
    if (firstShow) {
        firstShow = false;
        
        // Calculate the required size
        int cardWidth = PlantCard::CARD_WIDTH; // 280px
        int spacing = 15;
        int scrollMargins = 40; // left + right margins for scroll area
        int leftSidebar = 131;   // Tab sidebar - fixed width
        int rightSidebar = 81;   // Conditions sidebar - fixed width
        
        int minWidthForPlants = (2 * cardWidth) + (3 * spacing) + scrollMargins; // 645px
        int totalMinWidth = minWidthForPlants + leftSidebar + rightSidebar; // 857px
        int minHeight = PlantCard::CARD_EXPANDED_HEIGHT + 100; // 480px
        
        // Set size with buffer
        resize(totalMinWidth + 10, minHeight + 50);
        qDebug() << "Set initial window size in showEvent to:" << (totalMinWidth + 100) << "x" << (minHeight + 50);
        qDebug() << "Breakdown - Plants area:" << minWidthForPlants << "px, Left sidebar:" << leftSidebar << "px, Right sidebar:" << rightSidebar << "px";
    }
}

void MainWindow::onResizeTimeout()
{
    // Update plant grid when window resize is complete
    if (m_plantsGridLayout && m_plantManager && m_plantManager->getPlantCount() > 0) {
        // Force the scroll area to update its viewport size
        if (m_plantsScrollArea) {
            m_plantsScrollArea->updateGeometry();
        }
        updatePlantGrid();
    }
}

void MainWindow::onThemeChanged(Qt::ColorScheme colorScheme)
{
    Q_UNUSED(colorScheme)
    updateScrollAreaTheme();
}

void MainWindow::updateScrollAreaTheme()
{
    if (!m_plantsScrollArea) {
        return;
    }
    
    // Detect if we're in dark mode
    QPalette palette = QApplication::palette();
    QColor backgroundColor = palette.color(QPalette::Base);
    QColor borderColor = palette.color(QPalette::Mid);
    
    // Check if background is dark
    bool isDarkMode = backgroundColor.lightness() < 128;
    
    QString styleSheet;
    if (isDarkMode) {
        styleSheet = QString(
            "QScrollArea {"
            "    border: 1px solid %1;"
            "    border-radius: 8px;"
            "    background-color: %2;"
            "}"
        ).arg(borderColor.name()).arg(backgroundColor.name());
    } else {
        styleSheet = QString(
            "QScrollArea {"
            "    border: 1px solid %1;"
            "    border-radius: 8px;"
            "    background-color: %2;"
            "}"
        ).arg(borderColor.name()).arg(backgroundColor.name());
    }
    
    m_plantsScrollArea->setStyleSheet(styleSheet);
}

void MainWindow::setupResponsiveLayout()
{
    // Create a main horizontal layout for the central widget
    QHBoxLayout* mainLayout = new QHBoxLayout(ui->centralwidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Remove the widgets from their fixed positions and add to layout
    ui->Tab->setParent(nullptr);
    ui->PlantUI->setParent(nullptr);
    ui->Conditions->setParent(nullptr);
    
    // Set size policies for proper scaling
    ui->Tab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->PlantUI->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->Conditions->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    
    // Set minimum and maximum sizes for the sidebars to prevent them from being cut off
    ui->Tab->setMinimumWidth(131);
    ui->Tab->setMaximumWidth(131);
    ui->Conditions->setMinimumWidth(81);
    ui->Conditions->setMaximumWidth(81);
    
    // Add widgets to the layout
    mainLayout->addWidget(ui->Tab);
    mainLayout->addWidget(ui->PlantUI);
    mainLayout->addWidget(ui->Conditions);
    
    // Set stretch factors - PlantUI gets all extra space
    mainLayout->setStretchFactor(ui->Tab, 0);
    mainLayout->setStretchFactor(ui->PlantUI, 1);
    mainLayout->setStretchFactor(ui->Conditions, 0);
    
    // Now fix the PlantUI internal layout - make stackedWidget fill the entire PlantUI
    setupPlantUILayout();
}

void MainWindow::setupPlantUILayout()
{
    // Remove any existing layout from PlantUI
    delete ui->PlantUI->layout();
    
    // Create a layout that will make the stacked widget fill the entire PlantUI
    QVBoxLayout* plantUILayout = new QVBoxLayout(ui->PlantUI);
    plantUILayout->setContentsMargins(0, 0, 0, 0);
    plantUILayout->setSpacing(0);
    
    // Remove stackedWidget from its fixed position and add to layout
    ui->stackedWidget->setParent(nullptr);
    ui->stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Add stackedWidget to fill the entire PlantUI
    plantUILayout->addWidget(ui->stackedWidget);
}

void MainWindow::updateAllPlantCardsEnvironmentalData()
{
    // Update all plant cards with current environmental data
    for (PlantCard* card : m_plantCards) {
        if (card) {
            card->updateEnvironmentalData(m_currentTemp, m_currentHumidity, m_currentUV);
        }
    }
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    // Handle scroll area resize events
    if (object == m_plantsScrollArea && event->type() == QEvent::Resize) {
        // Start the resize timer when scroll area is resized
        if (m_resizeTimer) {
            m_resizeTimer->start();
        }
    }

    return QMainWindow::eventFilter(object, event);
}


