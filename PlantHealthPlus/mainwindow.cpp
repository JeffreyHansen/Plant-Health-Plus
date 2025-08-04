#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scraper.h"
#include "usermanager.h"
#include "logbookmanager.h"
#include "logbookentrydialog.h"
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
#include <QSlider>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QResizeEvent>
#include <QShowEvent>
#include <QPalette>
#include <QStyleHints>
#include <QDebug>
#include <QMap>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "addplantdialog.h"
#include <QIcon>
#include "scraper.h"
#include "forecastclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plantManager(nullptr)
    , m_plantsScrollArea(nullptr)
    , m_plantsContainer(nullptr)
    , m_plantsGridLayout(nullptr)
    , m_addPlantButton(nullptr)
    , m_loginPage(nullptr)
    , m_usernameEdit(nullptr)
    , m_passwordEdit(nullptr)
    , m_confirmPasswordEdit(nullptr)
    , m_loginButton(nullptr)
    , m_registerButton(nullptr)
    , m_togglePasswordButton(nullptr)
    , m_statusLabel(nullptr)
    , m_confirmLabel(nullptr)
    , m_mainInterface(nullptr)
    , m_userManager(nullptr)
    , m_isRegisterMode(false)
    , m_notificationButton(nullptr)
    , m_notificationBadge(nullptr)
    , m_notificationSound(nullptr)
    , m_audioOutput(nullptr)
    , m_currentVolume(50)
    , m_logbookManager(nullptr)
    , m_logbookEntryList(nullptr)
    , m_logbookContentDisplay(nullptr)
    , m_logbookImagesArea(nullptr)
    , m_logbookImagesContainer(nullptr)
    , m_logbookImagesLayout(nullptr)
    , m_addLogbookEntryButton(nullptr)
    , m_editLogbookEntryButton(nullptr)
    , m_deleteLogbookEntryButton(nullptr)
    , m_logbookEntryTitle(nullptr)
    , m_logbookEntryDates(nullptr)
{
    // Initialize user manager
    m_userManager = new UserManager();
    
    // Initialize logbook manager
    m_logbookManager = new LogbookManager();
    
    // Initialize resize timer early to prevent segfaults
    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setSingleShot(true);
    m_resizeTimer->setInterval(100); // 100ms delay
    connect(m_resizeTimer, &QTimer::timeout, this, &MainWindow::onResizeTimeout);
    
    // Set window properties
    setWindowTitle("Plant Health Plus");
    
    // Set window icon for taskbar/dock
    QIcon appIcon(":/images/images/PHPlus_logo.png");
    setWindowIcon(appIcon);
    
    // For macOS, also set the application icon
    QApplication::setWindowIcon(appIcon);
    
    setMinimumSize(400, 500);
    resize(800, 600);
    
    // Setup UI first
    ui->setupUi(this);

    // Setup layout for condition buttons
    setupConditionLayout();
    
    // Store reference to the main interface and hide it initially
    m_mainInterface = ui->centralwidget;
    m_mainInterface->hide();
    
    // Setup login page as the initial view
    setupLoginPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLoginPage()
{
    // Create the login page widget as a child of the main window
    m_loginPage = new QWidget(this);
    m_loginPage->setGeometry(0, 0, width(), height());
    m_loginPage->raise(); // Bring to front
    m_loginPage->show();
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(m_loginPage);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
    // Title
    QLabel* titleLabel = new QLabel("Plant Health Plus", m_loginPage);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 28px; font-weight: bold; color: #2e7d32; margin-bottom: 20px; }");
    mainLayout->addWidget(titleLabel);
    
    // Subtitle
    QLabel* subtitleLabel = new QLabel("Track and monitor your plant's health", m_loginPage);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("QLabel { font-size: 14px; margin-bottom: 30px; }");
    mainLayout->addWidget(subtitleLabel);
    
    // Login form container
    QFrame* formFrame = new QFrame(m_loginPage);
    formFrame->setMaximumWidth(400);
    formFrame->setStyleSheet("QFrame { border: 1px solid palette(mid); border-radius: 8px; padding: 25px; }");
    
    QVBoxLayout* formLayout = new QVBoxLayout(formFrame);
    formLayout->setSpacing(12);
    
    // Form fields - using custom layout for better control
    QVBoxLayout* fieldsLayout = new QVBoxLayout();
    fieldsLayout->setSpacing(12);
    
    // Username field - horizontal layout
    QHBoxLayout* usernameLayout = new QHBoxLayout();
    QLabel* usernameLabel = new QLabel("Username:", formFrame);
    usernameLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: normal; padding: 0px; margin: 0px; }");
    usernameLabel->setFixedWidth(90); // Fixed width to keep labels compact
    usernameLabel->setFixedHeight(40); // Fixed height for consistency
    usernameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Center text vertically
    m_usernameEdit = new QLineEdit(formFrame);
    m_usernameEdit->setPlaceholderText("Enter username");
    m_usernameEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid palette(mid); border-radius: 3px; font-size: 14px; background-color: palette(base); color: palette(text); }");
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit);
    fieldsLayout->addLayout(usernameLayout);
    
    // Password field with visibility toggle - horizontal layout
    QHBoxLayout* passwordRowLayout = new QHBoxLayout();
    QLabel* passwordLabel = new QLabel("Password:", formFrame);
    passwordLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: normal; padding: 0px; margin: 0px; }");
    passwordLabel->setFixedWidth(90); // Same width as username label
    passwordLabel->setFixedHeight(40); // Fixed height for consistency
    passwordLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Center text vertically
    m_passwordEdit = new QLineEdit(formFrame);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter password");
    m_passwordEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid palette(mid); border-radius: 3px; font-size: 14px; background-color: palette(base); color: palette(text); }");
    
    m_togglePasswordButton = new QPushButton("👁", formFrame);
    m_togglePasswordButton->setMaximumWidth(30);
    m_togglePasswordButton->setStyleSheet("QPushButton { border: none; font-size: 16px; color: palette(text); }");
    connect(m_togglePasswordButton, &QPushButton::clicked, this, &MainWindow::togglePasswordVisibility);
    
    QHBoxLayout* passwordInputLayout = new QHBoxLayout();
    passwordInputLayout->addWidget(m_passwordEdit);
    passwordInputLayout->addWidget(m_togglePasswordButton);
    passwordInputLayout->setContentsMargins(0, 0, 0, 0);
    
    passwordRowLayout->addWidget(passwordLabel);
    passwordRowLayout->addLayout(passwordInputLayout);
    fieldsLayout->addLayout(passwordRowLayout);
    
    // Confirm password field (initially hidden) - horizontal layout
    QHBoxLayout* confirmPasswordLayout = new QHBoxLayout();
    m_confirmLabel = new QLabel("Confirm Password:", formFrame);
    m_confirmLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: normal; padding: 0px; margin: 0px; }");
    m_confirmLabel->setFixedWidth(120); // Same width as other labels
    m_confirmLabel->setFixedHeight(40); // Fixed height for consistency
    m_confirmLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Center text vertically
    m_confirmPasswordEdit = new QLineEdit(formFrame);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText("Confirm password");
    m_confirmPasswordEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid palette(mid); border-radius: 3px; font-size: 14px; background-color: palette(base); color: palette(text); }");
    confirmPasswordLayout->addWidget(m_confirmLabel);
    confirmPasswordLayout->addWidget(m_confirmPasswordEdit);
    fieldsLayout->addLayout(confirmPasswordLayout);
    
    // Hide confirm password initially
    m_confirmLabel->hide();
    m_confirmPasswordEdit->hide();
    
    formLayout->addLayout(fieldsLayout);
    
    // Status label - clean text only, no background
    m_statusLabel = new QLabel(formFrame);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setMinimumHeight(20); // Minimum height to reserve space
    m_statusLabel->setWordWrap(true); // Allow text wrapping
    m_statusLabel->setStyleSheet("QLabel { font-size: 12px; color: transparent; }"); // Initially transparent
    m_statusLabel->setText(""); // Start with empty text
    formLayout->addWidget(m_statusLabel);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_loginButton = new QPushButton("Login", formFrame);
    m_loginButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 12px 24px; border: none; border-radius: 4px; font-size: 14px; font-weight: bold; } QPushButton:hover { background-color: #218838; }");
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    
    m_registerButton = new QPushButton("Register", formFrame);
    m_registerButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; padding: 12px 24px; border: none; border-radius: 4px; font-size: 14px; font-weight: bold; } QPushButton:hover { background-color: #0056b3; }");
    connect(m_registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_registerButton);
    formLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(formFrame, 0, Qt::AlignCenter);
    
    // Connect Enter key to login
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
    connect(m_confirmPasswordEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
}

void MainWindow::onLoginClicked()
{
    if (!validateLoginInput()) {
        return;
    }
    
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (m_isRegisterMode) {
        // Register new user
        if (m_userManager->userExists(username)) {
            showErrorMessage("Username already exists!");
            return;
        }
        
        if (m_userManager->createUser(username, password)) {
            showSuccessMessage("Registration successful! Please login.");
            
            // Switch to login mode
            m_isRegisterMode = false;
            m_confirmLabel->hide();
            m_confirmPasswordEdit->hide();
            m_registerButton->setText("Register");
            m_loginButton->setText("Login");
            
            // Clear form
            m_passwordEdit->clear();
            m_confirmPasswordEdit->clear();
        } else {
            showErrorMessage("Registration failed!");
        }
    } else {
        // Login existing user
        if (m_userManager->authenticateUser(username, password)) {
            showSuccessMessage("Login successful!");
            
            // Setup main interface with authenticated user
            showMainInterface();
        } else {
            showErrorMessage("Invalid username or password!");
        }
    }
}

void MainWindow::onRegisterClicked()
{
    m_isRegisterMode = !m_isRegisterMode;
    
    if (m_isRegisterMode) {
        m_confirmLabel->show();
        m_confirmPasswordEdit->show();
        m_registerButton->setText("Back to Login");
        m_loginButton->setText("Register");
    } else {
        m_confirmLabel->hide();
        m_confirmPasswordEdit->hide();
        m_registerButton->setText("Register");
        m_loginButton->setText("Login");
    }
    
    m_statusLabel->setText(""); // Clear status message
    m_statusLabel->setStyleSheet("QLabel { font-size: 12px; color: transparent; }"); // Make transparent
    m_passwordEdit->clear();
    m_confirmPasswordEdit->clear();
}

void MainWindow::togglePasswordVisibility()
{
    if (m_passwordEdit->echoMode() == QLineEdit::Password) {
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
        m_confirmPasswordEdit->setEchoMode(QLineEdit::Normal);
        m_togglePasswordButton->setText("🙈");
    } else {
        m_passwordEdit->setEchoMode(QLineEdit::Password);
        m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
        m_togglePasswordButton->setText("👁");
    }
}

bool MainWindow::validateLoginInput()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();
    
    if (username.isEmpty()) {
        showErrorMessage("Username cannot be empty!");
        return false;
    }
    
    if (password.isEmpty()) {
        showErrorMessage("Password cannot be empty!");
        return false;
    }
    
    if (m_isRegisterMode) {
        if (password.length() < 6) {
            showErrorMessage("Password must be at least 6 characters long!");
            return false;
        }
        
        if (password != confirmPassword) {
            showErrorMessage("Passwords do not match!");
            return false;
        }
    }
    
    return true;
}

void MainWindow::showSuccessMessage(const QString& message)
{
    qDebug() << "Showing success message:" << message;
    m_statusLabel->setText(message);
    // Use system theme colors for success message - clean text only
    QPalette palette = this->palette();
    QColor successColor = QColor(40, 167, 69); // Green that works in both themes
    if (palette.color(QPalette::Base).lightness() < 128) {
        successColor = QColor(72, 187, 120); // Lighter green for dark mode
    }
    QString styleSheet = QString("QLabel { color: %1; font-size: 12px; font-weight: bold; }").arg(successColor.name());
    m_statusLabel->setStyleSheet(styleSheet);
    qDebug() << "Applied success style:" << styleSheet;
}

void MainWindow::showErrorMessage(const QString& message)
{
    qDebug() << "Showing error message:" << message;
    m_statusLabel->setText(message);
    // Use system theme colors for error message - clean text only
    QPalette palette = this->palette();
    QColor errorColor = QColor(220, 53, 69); // Red that works in both themes  
    if (palette.color(QPalette::Base).lightness() < 128) {
        errorColor = QColor(255, 107, 107); // Lighter red for dark mode
    }
    QString styleSheet = QString("QLabel { color: %1; font-size: 12px; font-weight: bold; }").arg(errorColor.name());
    m_statusLabel->setStyleSheet(styleSheet);
    qDebug() << "Applied error style:" << styleSheet;
}

void MainWindow::showMainInterface()
{
    QString username = m_usernameEdit->text().trimmed();
    qDebug() << "showMainInterface called for user:" << username;
    
    // Hide login page and show main interface
    m_loginPage->hide();
    m_mainInterface->show();
    qDebug() << "Login page hidden, main interface shown";
    
    // Setup responsive layout for central widget
    setupResponsiveLayout();
    qDebug() << "Responsive layout setup complete";
    
    // Set minimum window size to ensure at least 2 plants per row
    // Calculation: 2 cards (280px each) + 3 spacings (15px each) + scroll margins (40px) + sidebars (131px + 81px)
    int cardWidth = PlantCard::CARD_WIDTH; // 280px
    int spacing = 15;
    int scrollMargins = 40; // left + right margins for scroll area
    int leftSidebar = 131;   // Tab sidebar - fixed width
    int rightSidebar = 81;   // Conditions sidebar - fixed width
    
    int minWidthForPlants = (2 * cardWidth) + (3 * spacing) + scrollMargins; // 2 cards + spacings + margins = 645px
    int totalMinWidth = minWidthForPlants + leftSidebar + rightSidebar; // 645 + 131 + 81 = 857px
    
    // Calculate minimum height to ensure tab buttons and bottom buttons are visible
    // Tab buttons: 3 buttons at ~50px each = 150px
    // Spacer and bottom buttons: settings + notification buttons = 80px (40px each)
    // Additional margin/spacing = 30px (reduced to allow more contraction)
    // Total sidebar minimum = 260px
    int minSidebarHeight = 260; // Minimum height for tab sidebar
    int minHeight = qMax(PlantCard::CARD_EXPANDED_HEIGHT + 100, minSidebarHeight); // Use the larger of the two
    
    setMinimumSize(totalMinWidth, minHeight);
    qDebug() << "Set minimum window size to:" << totalMinWidth << "x" << minHeight;

    // Initialize plant manager with the authenticated user
    m_plantManager = new PlantManager(this);
    m_plantManager->setCurrentUser(username);
    
    // Initialize logbook manager with the authenticated user
    m_logbookManager->setCurrentUser(username);
    
    // Load user's volume setting
    m_currentVolume = m_userManager->getUserVolume();
    
    // Connect plant manager signals
    connect(m_plantManager, &PlantManager::plantAdded, this, &MainWindow::onPlantAdded);
    connect(m_plantManager, &PlantManager::plantRemoved, this, &MainWindow::onPlantRemoved);
    connect(m_plantManager, &PlantManager::plantUpdated, this, &MainWindow::onPlantUpdated);
    connect(m_plantManager, &PlantManager::dataChanged, this, &MainWindow::updatePlantGrid);

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
            checkPlantConditions(); // Check for condition violations
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
            checkPlantConditions(); // Check for condition violations
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
            checkPlantConditions(); // Check for condition violations
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
    
    // Connect settings button (disconnect first to prevent duplicates)
    disconnect(ui->settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(ui->settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    
    // Setup notification system
    setupNotificationButton();
    
    // Load user's volume setting
    m_currentVolume = m_userManager->getUserVolume();

    // Set the initial state
    ui->myplants_tab->setChecked(true);
    stackedWidget->setCurrentIndex(0);
    
    // Setup My Plants page
    setupMyPlantsPage();
    qDebug() << "My Plants page setup complete";
    
    // Setup Logbook page
    setupLogbookPage();
    qDebug() << "Logbook page setup complete";
    
    // Apply initial theme
    updateScrollAreaTheme();
    
    // Connect to theme changes
    connect(QApplication::styleHints(), &QStyleHints::colorSchemeChanged,
            this, &MainWindow::onThemeChanged);

    // Connect forecast and check plants
    forecastClient = new ForecastClient(this);
    connect(forecastClient, &ForecastClient::forecastsReady,
            this, &MainWindow::handleForecastNotification);

    // Forecast-check timer
    forecastTimer = new QTimer(this);
    connect(forecastTimer, &QTimer::timeout, this, &MainWindow::checkForecastTemperature);
    forecastTimer->start(24 * 60 * 60 * 1000);

    QTimer::singleShot(0, this, &MainWindow::checkForecastTemperature);
}

void MainWindow::setupMyPlantsPage()
{
    qDebug() << "setupMyPlantsPage called";
    
    // Find the My Plants page widget
    QWidget* myPlantsPage = ui->stackedWidget->widget(0); // Assuming index 0 is My Plants
    if (!myPlantsPage) {
        qDebug() << "Error: My Plants page widget not found!";
        return;
    }
    
    qDebug() << "My Plants page widget found, clearing existing layout";
    
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
    qDebug() << "setupMyPlantsPage completed, grid updated";
}

void MainWindow::setupLogbookPage()
{
    qDebug() << "setupLogbookPage called";
    
    // Find the Logbook page widget (assuming index 1 is Logbook)
    QWidget* logbookPage = ui->stackedWidget->widget(1);
    if (!logbookPage) {
        qDebug() << "Error: Logbook page widget not found!";
        return;
    }
    
    // Check if logbook is already set up to prevent duplicates
    if (m_logbookEntryList != nullptr) {
        qDebug() << "Logbook page already set up, skipping...";
        return;
    }
    
    qDebug() << "Logbook page widget found, clearing existing layout";
    
    // Clear existing layout if any and ensure all child widgets are properly cleaned up
    QLayout* existingLayout = logbookPage->layout();
    if (existingLayout) {
        // Clear all widgets from the layout first
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->setParent(nullptr);
                widget->deleteLater();
            }
            if (QLayout* childLayout = item->layout()) {
                delete childLayout;
            }
            delete item;
        }
        delete existingLayout;
    }
    
    // Reset all logbook UI pointers to ensure clean state
    m_logbookEntryList = nullptr;
    m_logbookContentDisplay = nullptr;
    m_logbookImagesArea = nullptr;
    m_logbookImagesContainer = nullptr;
    m_logbookImagesLayout = nullptr;
    m_addLogbookEntryButton = nullptr;
    m_editLogbookEntryButton = nullptr;
    m_deleteLogbookEntryButton = nullptr;
    m_logbookEntryTitle = nullptr;
    m_logbookEntryDates = nullptr;
    
    // Create main layout for the page
    QHBoxLayout* pageLayout = new QHBoxLayout(logbookPage);
    pageLayout->setContentsMargins(20, 20, 20, 20);
    pageLayout->setSpacing(15);
    
    // Left side - Entry list
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    // Header for entry list
    QHBoxLayout* leftHeaderLayout = new QHBoxLayout();
    QLabel* entriesLabel = new QLabel("Journal Entries");
    entriesLabel->setFont(QFont("Arial", 16, QFont::Bold));
    entriesLabel->setStyleSheet("color: #2E7D32;");
    
    m_addLogbookEntryButton = new QPushButton("+ New Entry");
    m_addLogbookEntryButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );
    connect(m_addLogbookEntryButton, &QPushButton::clicked, this, &MainWindow::onAddLogbookEntryClicked);
    
    leftHeaderLayout->addWidget(entriesLabel);
    leftHeaderLayout->addStretch();
    leftHeaderLayout->addWidget(m_addLogbookEntryButton);
    
    // Entry list widget
    m_logbookEntryList = new QListWidget();
    m_logbookEntryList->setStyleSheet(
        "QListWidget {"
        "    border: 1px solid palette(mid);"
        "    border-radius: 6px;"
        "    background-color: palette(base);"
        "    selection-background-color: #E8F5E8;"
        "}"
        "QListWidget::item {"
        "    padding: 10px;"
        "    border-bottom: 1px solid palette(mid);"
        "}"
        "QListWidget::item:hover {"
        "    background-color: palette(alternate-base);"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #E8F5E8;"
        "    color: #2E7D32;"
        "}"
    );
    
    // Enable rich text rendering for list items
    m_logbookEntryList->setTextElideMode(Qt::ElideNone);
    m_logbookEntryList->setWordWrap(true);
    
    connect(m_logbookEntryList, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        if (item && item->data(Qt::UserRole).isValid()) {
            QString entryId = item->data(Qt::UserRole).toString();
            onLogbookEntrySelected(entryId);
        }
    });
    
    leftLayout->addLayout(leftHeaderLayout);
    leftLayout->addWidget(m_logbookEntryList);
    
    // Right side - Entry display and editing
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    // Header for entry display
    QHBoxLayout* rightHeaderLayout = new QHBoxLayout();
    
    m_logbookEntryTitle = new QLabel("Select an entry to view");
    m_logbookEntryTitle->setFont(QFont("Arial", 18, QFont::Bold));
    m_logbookEntryTitle->setStyleSheet("color: #2E7D32;");
    
    m_logbookEntryDates = new QLabel("");
    m_logbookEntryDates->setStyleSheet("color: palette(dark); font-size: 12px;");
    m_logbookEntryDates->setAlignment(Qt::AlignRight);
    
    rightHeaderLayout->addWidget(m_logbookEntryTitle);
    rightHeaderLayout->addStretch();
    rightHeaderLayout->addWidget(m_logbookEntryDates);
    
    // Action buttons
    QHBoxLayout* actionButtonsLayout = new QHBoxLayout();
    
    m_editLogbookEntryButton = new QPushButton("Edit Entry");
    m_editLogbookEntryButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #007bff;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0056b3;"
        "}"
        "QPushButton:disabled {"
        "    background-color: palette(mid);"
        "    color: palette(dark);"
        "}"
    );
    m_editLogbookEntryButton->setEnabled(false);
    connect(m_editLogbookEntryButton, &QPushButton::clicked, this, &MainWindow::onEditLogbookEntryClicked);
    
    m_deleteLogbookEntryButton = new QPushButton("Delete Entry");
    m_deleteLogbookEntryButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c82333;"
        "}"
        "QPushButton:disabled {"
        "    background-color: palette(mid);"
        "    color: palette(dark);"
        "}"
    );
    m_deleteLogbookEntryButton->setEnabled(false);
    connect(m_deleteLogbookEntryButton, &QPushButton::clicked, this, &MainWindow::onDeleteLogbookEntryClicked);
    
    actionButtonsLayout->addStretch();
    actionButtonsLayout->addWidget(m_editLogbookEntryButton);
    actionButtonsLayout->addWidget(m_deleteLogbookEntryButton);
    
    // Content display area
    m_logbookContentDisplay = new QTextEdit();
    m_logbookContentDisplay->setReadOnly(true);
    m_logbookContentDisplay->setStyleSheet(
        "QTextEdit {"
        "    border: 1px solid palette(mid);"
        "    border-radius: 6px;"
        "    background-color: palette(base);"
        "    padding: 10px;"
        "    font-size: 14px;"
        "    line-height: 1.4;"
        "}"
    );
    m_logbookContentDisplay->setPlaceholderText("Select an entry to view its content...");
    
    // Images display area
    QLabel* imagesLabel = new QLabel("Images");
    imagesLabel->setFont(QFont("Arial", 14, QFont::Bold));
    imagesLabel->setStyleSheet("color: #2E7D32; margin-top: 10px;");
    
    m_logbookImagesArea = new QScrollArea();
    m_logbookImagesArea->setWidgetResizable(true);
    m_logbookImagesArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_logbookImagesArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_logbookImagesArea->setMaximumHeight(200);
    m_logbookImagesArea->setStyleSheet(
        "QScrollArea {"
        "    border: 1px solid palette(mid);"
        "    border-radius: 6px;"
        "    background-color: palette(base);"
        "}"
    );
    
    // Install event filter to handle scroll area resize events
    m_logbookImagesArea->installEventFilter(this);
    
    m_logbookImagesContainer = new QWidget();
    m_logbookImagesContainer->setStyleSheet("background-color: transparent;");
    
    m_logbookImagesLayout = new QGridLayout(m_logbookImagesContainer);
    m_logbookImagesLayout->setSpacing(10);
    m_logbookImagesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    m_logbookImagesArea->setWidget(m_logbookImagesContainer);
    
    rightLayout->addLayout(rightHeaderLayout);
    rightLayout->addLayout(actionButtonsLayout);
    rightLayout->addWidget(m_logbookContentDisplay, 2); // Give content display more space
    rightLayout->addWidget(imagesLabel);
    rightLayout->addWidget(m_logbookImagesArea, 1);
    
    // Add left and right layouts to main layout
    pageLayout->addLayout(leftLayout, 1);   // Entry list takes 1/3
    pageLayout->addLayout(rightLayout, 2);  // Entry display takes 2/3
    
    // Load initial data
    updateLogbookEntryList();
    
    qDebug() << "setupLogbookPage completed";
}

void MainWindow::setupConditionLayout()
{
    // Initialize layout for Conditions container if it doesn't exist
    if(!ui->Conditions->layout()) {
        ui->Conditions->setLayout(new QVBoxLayout);
    }

    // Configure the vertical layout properties
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->Conditions->layout());
    layout->setContentsMargins(1, 10, 1, 1);
    layout->setSpacing(10);

    // Helper function to create consistent drop shadow effects
    auto makeShadowEffect = []() {
        QGraphicsDropShadowEffect* m_buttonEffect = new QGraphicsDropShadowEffect;
        m_buttonEffect->setBlurRadius(10);
        m_buttonEffect->setOffset(2, 2);
        m_buttonEffect->setColor(QColor(0, 0, 0, 100));
        return m_buttonEffect;
    };

    // Helper function to create standardized condition buttons
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

    // Create temperature button with icon and shadow
    QPushButton* m_tempButton = createConditionButton();
    m_tempButton->setObjectName("temp");
    m_tempButton->setIcon(QIcon(":/images/images/thermometer_temp.png"));
    m_tempButton->setIconSize(QSize(60, 60));
    m_tempButton->setGraphicsEffect(makeShadowEffect());

    // Create UV button with icon and shadow
    QPushButton* m_uvButton = createConditionButton();
    m_uvButton->setObjectName("uv");
    m_uvButton->setIcon(QIcon(":/images/images/uv.png"));
    m_uvButton->setIconSize(QSize(60, 60));
    m_uvButton->setGraphicsEffect(makeShadowEffect());

    // Create humidity button with icon and shadow
    QPushButton* m_humidityButton = createConditionButton();
    m_humidityButton->setObjectName("humidity");
    m_humidityButton->setIcon(QIcon(":/images/images/humidity.png"));
    m_humidityButton->setIconSize(QSize(80, 80));
    m_humidityButton->setGraphicsEffect(makeShadowEffect());

     // Connect all buttons to the same slot
    connect(m_tempButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);
    connect(m_uvButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);
    connect(m_humidityButton, &QPushButton::clicked, this, &MainWindow::onConditionClicked);

     // Create descriptive labels for each button
    QLabel* humidityLabel = new QLabel("Humidity");
    humidityLabel->setFont(QFont("Calbiri", 12, QFont::Bold));
    humidityLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");
    QLabel* tempLabel = new QLabel("Temp");
    tempLabel->setFont(QFont("Calbiri", 12, QFont::Bold));
    tempLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");
    QLabel* uvLabel = new QLabel("UV");
    uvLabel->setFont(QFont("Calibri", 12, QFont::Bold));
    uvLabel->setStyleSheet("color: rgba(47, 148, 41, 0.9);");

    // Add widgets to layout in pairs: label then button
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

    // Get the button that triggered this slot
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    // Determine condition type from button's object name
    QString name = clickedButton->objectName();
    ConditionType type;

    if (name == "temp") {
        type = ConditionType::Temperature;
    } else if (name == "uv") {
        type = ConditionType::UV;
    } else if (name == "humidity") {
        type = ConditionType::Humidity;
    } else {
        qDebug() << "Unknown condition, returning ungracefully.";
        return;
    }

    // Check if chart for this condition already exists
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

    // Create new chart window
    QWidget *chartWindow = new QWidget();
    chartWindow->resize(600, 400);
    chartWindow->setAttribute(Qt::WA_DeleteOnClose);

    // Create chart widget and forecast client
    ConditionChart *chartWidget = new ConditionChart(type, chartWindow);
    ForecastClient *client = new ForecastClient(chartWindow);

    // Connect data ready signal to chart update slot, and request forecast data
    connect(client, &ForecastClient::forecastsReady,
            chartWidget, &ConditionChart::updateChart);

    client->getURL();

    // Provide layout
    QVBoxLayout *layout = new QVBoxLayout(chartWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartWidget);
    chartWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartWindow->setLayout(layout);

    // Track the new window, or clear window when it is destroyed
    openCharts[type] = chartWindow;

    connect(chartWindow, &QWidget::destroyed, [type]() {
        openCharts.remove(type);
    });

    // Set window title based on condition type
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

    // Show and focus the new window
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

// Logbook methods implementation
void MainWindow::onAddLogbookEntryClicked()
{
    LogbookEntryDialog dialog(m_logbookManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        LogbookEntry newEntry = dialog.getEntry();
        if (m_logbookManager->addEntry(newEntry)) {
            qDebug() << "Logbook entry added successfully";
            updateLogbookEntryList();
            // Select the newly added entry
            onLogbookEntrySelected(newEntry.id);
        } else {
            QMessageBox::warning(this, "Error", "Failed to add logbook entry.");
        }
    }
}

void MainWindow::onEditLogbookEntryClicked()
{
    if (m_selectedLogbookEntryId.isEmpty()) {
        return;
    }
    
    LogbookEntry currentEntry = m_logbookManager->getEntry(m_selectedLogbookEntryId);
    LogbookEntryDialog dialog(currentEntry, m_logbookManager, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        LogbookEntry updatedEntry = dialog.getEntry();
        // Preserve the original ID and creation date
        updatedEntry.id = currentEntry.id;
        updatedEntry.dateCreated = currentEntry.dateCreated;
        updatedEntry.dateModified = QDateTime::currentDateTime();
        
        if (m_logbookManager->updateEntry(m_selectedLogbookEntryId, updatedEntry)) {
            qDebug() << "Logbook entry updated successfully";
            updateLogbookEntryList();
            // Refresh the display
            onLogbookEntrySelected(m_selectedLogbookEntryId);
        } else {
            QMessageBox::warning(this, "Error", "Failed to update logbook entry.");
        }
    }
}

void MainWindow::onDeleteLogbookEntryClicked()
{
    if (m_selectedLogbookEntryId.isEmpty()) {
        return;
    }
    
    LogbookEntry entry = m_logbookManager->getEntry(m_selectedLogbookEntryId);
    
    // Create confirmation dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Delete Entry");
    dialog.setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QLabel* label = new QLabel(QString("Are you sure you want to delete the entry '%1'?").arg(entry.title));
    label->setWordWrap(true);
    layout->addWidget(label);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* yesButton = new QPushButton("Yes");
    QPushButton* noButton = new QPushButton("No");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(noButton);
    buttonLayout->addWidget(yesButton);
    layout->addLayout(buttonLayout);
    
    connect(yesButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(noButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    noButton->setDefault(true);
    noButton->setFocus();
    
    if (dialog.exec() == QDialog::Accepted) {
        if (m_logbookManager->deleteEntry(m_selectedLogbookEntryId)) {
            qDebug() << "Logbook entry deleted successfully";
            updateLogbookEntryList();
            clearLogbookDisplay();
            m_selectedLogbookEntryId.clear();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete logbook entry.");
        }
    }
}

void MainWindow::onLogbookEntrySelected(const QString& entryId)
{
    m_selectedLogbookEntryId = entryId;
    displayLogbookEntry(entryId);
    
    // Enable action buttons
    m_editLogbookEntryButton->setEnabled(true);
    m_deleteLogbookEntryButton->setEnabled(true);
}

void MainWindow::updateLogbookEntryList()
{
    if (!m_logbookEntryList || !m_logbookManager) {
        return;
    }
    
    m_logbookEntryList->clear();
    
    QList<LogbookEntry> entries = m_logbookManager->getAllEntries();
    
    // Sort entries by creation date (newest first)
    std::sort(entries.begin(), entries.end(), [](const LogbookEntry& a, const LogbookEntry& b) {
        return a.dateCreated > b.dateCreated;
    });
    
    for (const LogbookEntry& entry : entries) {
        QListWidgetItem* item = new QListWidgetItem();
        
        // Create a custom widget for the list item
        QWidget* itemWidget = new QWidget();
        itemWidget->setStyleSheet("QWidget { background-color: transparent; }");
        
        QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
        itemLayout->setContentsMargins(10, 8, 10, 8);
        itemLayout->setSpacing(2);
        
        // Title label
        QLabel* titleLabel = new QLabel(entry.title.isEmpty() ? "Untitled Entry" : entry.title);
        titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; color: palette(text); }");
        titleLabel->setWordWrap(true);
        
        // Date label
        QLabel* dateLabel = new QLabel(entry.dateCreated.toString("MMM dd, yyyy - hh:mm"));
        dateLabel->setStyleSheet("QLabel { font-size: 11px; color: palette(dark); }");
        
        itemLayout->addWidget(titleLabel);
        itemLayout->addWidget(dateLabel);
        
        // Set the item size and data
        item->setSizeHint(QSize(0, 60));
        item->setData(Qt::UserRole, entry.id);
        
        // Add item to list and set custom widget
        m_logbookEntryList->addItem(item);
        m_logbookEntryList->setItemWidget(item, itemWidget);
    }
    
    qDebug() << "Updated logbook entry list with" << entries.size() << "entries";
}

void MainWindow::displayLogbookEntry(const QString& entryId)
{
    if (entryId.isEmpty() || !m_logbookManager) {
        clearLogbookDisplay();
        return;
    }
    
    LogbookEntry entry = m_logbookManager->getEntry(entryId);
    if (entry.id.isEmpty()) {
        qDebug() << "Entry not found for ID:" << entryId;
        clearLogbookDisplay();
        return;
    }
    
    // Update title and dates
    m_logbookEntryTitle->setText(entry.title.isEmpty() ? "Untitled Entry" : entry.title);
    
    QString dateText = QString("Created: %1").arg(entry.dateCreated.toString("MMM dd, yyyy hh:mm"));
    if (entry.dateModified != entry.dateCreated) {
        dateText += QString("\nModified: %1").arg(entry.dateModified.toString("MMM dd, yyyy hh:mm"));
    }
    m_logbookEntryDates->setText(dateText);
    
    // Update content
    m_logbookContentDisplay->setPlainText(entry.content);
    
    // Clear and update images
    clearLogbookImagesDisplay();
    
    int imageIndex = 0;
    const int imagesPerRow = 3;
    
    for (const QString& imagePath : entry.imagePaths) {
        if (QFile::exists(imagePath)) {
            QLabel* imageLabel = new QLabel();
            imageLabel->setStyleSheet("border: 1px solid palette(mid); border-radius: 4px;");
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setFixedSize(120, 90);
            imageLabel->setScaledContents(true);
            
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
                imageLabel->setPixmap(pixmap);
            } else {
                imageLabel->setText("Image\nNot Found");
                imageLabel->setStyleSheet("border: 1px solid red; color: red; text-align: center;");
            }
            
            int row = imageIndex / imagesPerRow;
            int col = imageIndex % imagesPerRow;
            
            m_logbookImagesLayout->addWidget(imageLabel, row, col);
            imageIndex++;
        }
    }
    
    qDebug() << "Displayed logbook entry:" << entry.title;
}

void MainWindow::clearLogbookDisplay()
{
    if (m_logbookEntryTitle) {
        m_logbookEntryTitle->setText("Select an entry to view");
    }
    if (m_logbookEntryDates) {
        m_logbookEntryDates->setText("");
    }
    if (m_logbookContentDisplay) {
        m_logbookContentDisplay->clear();
        m_logbookContentDisplay->setPlaceholderText("Select an entry to view its content...");
    }
    
    clearLogbookImagesDisplay();
    
    // Disable action buttons
    if (m_editLogbookEntryButton) {
        m_editLogbookEntryButton->setEnabled(false);
    }
    if (m_deleteLogbookEntryButton) {
        m_deleteLogbookEntryButton->setEnabled(false);
    }
}

void MainWindow::clearLogbookImagesDisplay()
{
    if (!m_logbookImagesLayout) {
        return;
    }
    
    // Clear all widgets from the images layout
    while (QLayoutItem* item = m_logbookImagesLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    
    // Keep login page sized properly if it's visible
    if (m_loginPage && m_loginPage->isVisible()) {
        m_loginPage->setGeometry(0, 0, width(), height());
    }
    
    // DEBUG: Track layout elements during resize
    if (ui->settings_button && ui->notification_button) {
        QSpacerItem* verticalSpacer = nullptr;
        
        // Find the spacer by searching through the Tab layout
        if (ui->Tab) {
            QVBoxLayout* tabLayout = qobject_cast<QVBoxLayout*>(ui->Tab->layout());
            if (tabLayout) {
                for (int i = 0; i < tabLayout->count(); ++i) {
                    QLayoutItem* item = tabLayout->itemAt(i);
                    if (item && item->spacerItem()) {
                        verticalSpacer = item->spacerItem();
                        break;
                    }
                }
            }
        }
        
        qDebug() << "=== RESIZE EVENT DEBUG ===";
        qDebug() << "Window size:" << event->size().width() << "x" << event->size().height();
        qDebug() << "Settings button position:" << ui->settings_button->pos() << "size:" << ui->settings_button->size();
        qDebug() << "Settings button geometry:" << ui->settings_button->geometry();
        qDebug() << "Notification button position:" << ui->notification_button->pos() << "size:" << ui->notification_button->size();
        qDebug() << "Notification button geometry:" << ui->notification_button->geometry();
        
        if (verticalSpacer) {
            qDebug() << "Vertical spacer size hint:" << verticalSpacer->sizeHint();
            qDebug() << "Vertical spacer minimum size:" << verticalSpacer->minimumSize();
            qDebug() << "Vertical spacer maximum size:" << verticalSpacer->maximumSize();
            qDebug() << "Vertical spacer geometry:" << verticalSpacer->geometry();
        } else {
            qDebug() << "Vertical spacer not found!";
        }
        
        // Check Tab frame geometry
        if (ui->Tab) {
            qDebug() << "Tab frame geometry:" << ui->Tab->geometry();
        }
        
        // Check if settings button is cut off
        int settingsBottom = ui->settings_button->pos().y() + ui->settings_button->height();
        int windowHeight = height();
        bool isCutOff = settingsBottom > windowHeight;
        qDebug() << "Settings bottom Y:" << settingsBottom << "Window height:" << windowHeight << "Cut off:" << isCutOff;
        qDebug() << "=========================";
    }
    
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
    
    // Update logbook scroll areas when window resize is complete
    if (m_logbookEntryList) {
        if (m_logbookImagesArea) {
            m_logbookImagesArea->updateGeometry();
        }
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
    // Don't recreate layout if it already exists
    if (ui->centralwidget->layout()) {
        qDebug() << "Layout already exists, skipping setup";
        return;
    }
    
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
    // Handle scroll area resize events for both plants and logbook
    if (event->type() == QEvent::Resize) {
        if (object == m_plantsScrollArea || object == m_logbookImagesArea) {
            // Start the resize timer when any monitored scroll area is resized
            if (m_resizeTimer) {
                m_resizeTimer->start();
            }
        }
    }

    return QMainWindow::eventFilter(object, event);
}

void MainWindow::adjustTabLayout()
{
    // No longer needed - spacer behavior is controlled through UI configuration
}

void MainWindow::onSettingsClicked()
{
    showSettingsDialog();
}

void MainWindow::onLogoutClicked()
{
    // Save current user's data
    if (m_plantManager) {
        m_plantManager->savePlantsToFile();
    }
    
    // Save logbook data
    if (m_logbookManager) {
        m_logbookManager->saveToFile();
    }
    
    // Clear plant cards and grid
    clearPlantGrid();
    
    // Clear current user data
    if (m_plantManager) {
        delete m_plantManager;
        m_plantManager = nullptr;
    }
    
    // Clear plant UI components - they will be recreated on next login
    m_plantCards.clear();
    
    // Clear logbook UI components
    clearLogbookDisplay();
    m_selectedLogbookEntryId.clear();
    
    // Reset logbook UI pointers to prevent stale references
    m_logbookEntryList = nullptr;
    m_logbookContentDisplay = nullptr;
    m_logbookImagesArea = nullptr;
    m_logbookImagesContainer = nullptr;
    m_logbookImagesLayout = nullptr;
    m_addLogbookEntryButton = nullptr;
    m_editLogbookEntryButton = nullptr;
    m_deleteLogbookEntryButton = nullptr;
    m_logbookEntryTitle = nullptr;
    m_logbookEntryDates = nullptr;
    
    // Clear notifications
    m_notifications.clear();
    if (m_notificationBadge) {
        m_notificationBadge->hide();
    }
    
    // Reset environmental data
    m_currentTemp = 0.0;
    m_currentHumidity = 0.0;
    m_currentUV = 0.0;
    
    // Important: Clear the existing layout from PlantUI to ensure clean state
    QWidget* myPlantsPage = ui->stackedWidget->widget(0);
    if (myPlantsPage && myPlantsPage->layout()) {
        delete myPlantsPage->layout();
    }
    
    // Clear logbook page layout
    QWidget* logbookPage = ui->stackedWidget->widget(1);
    if (logbookPage && logbookPage->layout()) {
        delete logbookPage->layout();
    }
    
    // Clear the central widget layout to ensure clean state for next login
    if (ui->centralwidget->layout()) {
        delete ui->centralwidget->layout();
        
        // Reset the widgets to their original parent
        ui->Tab->setParent(ui->centralwidget);
        ui->PlantUI->setParent(ui->centralwidget);
        ui->Conditions->setParent(ui->centralwidget);
    }
    
    // Hide main interface and show login page
    m_mainInterface->hide();
    
    // Reset login form
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_confirmPasswordEdit->clear();
    m_statusLabel->setText("");
    m_statusLabel->setStyleSheet("QLabel { font-size: 12px; color: transparent; }");
    m_isRegisterMode = false;
    m_confirmLabel->hide();
    m_confirmPasswordEdit->hide();
    m_registerButton->setText("Register");
    m_loginButton->setText("Login");
    
    // Show login page
    m_loginPage->setGeometry(0, 0, width(), height());
    m_loginPage->show();
    m_loginPage->raise();
}

void MainWindow::showSettingsDialog()
{
    QDialog* settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("Settings");
    settingsDialog->setModal(true);
    settingsDialog->setFixedSize(300, 200);
    
    QVBoxLayout* layout = new QVBoxLayout(settingsDialog);
    layout->setSpacing(20);
    layout->setContentsMargins(20, 20, 20, 20);
    
    // Volume control section
    QLabel* volumeLabel = new QLabel("Volume", settingsDialog);
    volumeLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    layout->addWidget(volumeLabel);
    
    QSlider* volumeSlider = new QSlider(Qt::Horizontal, settingsDialog);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(m_currentVolume); // Use current user's volume
    volumeSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999999;"
        "    height: 8px;"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
        "    margin: 2px 0;"
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
        "    border: 1px solid #5c5c5c;"
        "    width: 18px;"
        "    margin: -2px 0;"
        "    border-radius: 9px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #d4d4d4, stop:1 #afafaf);"
        "}"
    );
    layout->addWidget(volumeSlider);
    
    // Volume value label
    QLabel* volumeValueLabel = new QLabel(QString("%1%").arg(m_currentVolume), settingsDialog);
    volumeValueLabel->setAlignment(Qt::AlignCenter);
    volumeValueLabel->setStyleSheet("QLabel { font-size: 12px; color: #666666; }");
    layout->addWidget(volumeValueLabel);
    
    // Connect volume slider to update the label and save volume
    connect(volumeSlider, &QSlider::valueChanged, [this, volumeValueLabel](int value) {
        volumeValueLabel->setText(QString("%1%").arg(value));
        m_currentVolume = value;
        if (m_userManager) {
            m_userManager->setUserVolume(value);
        }
    });
    
    // Add spacer
    layout->addStretch();
    
    // Logout button
    QPushButton* logoutButton = new QPushButton("Logout", settingsDialog);
    logoutButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c82333;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #bd2130;"
        "}"
    );
    
    connect(logoutButton, &QPushButton::clicked, [this, settingsDialog]() {
        settingsDialog->accept();
        onLogoutClicked();
    });
    
    layout->addWidget(logoutButton);
    
    // Show the dialog
    settingsDialog->exec();
    settingsDialog->deleteLater();
}

void MainWindow::setupNotificationButton()
{
    // Check if already set up to prevent duplicate connections
    if (m_notificationBadge) {
        return;
    }
    
    // Create notification badge label
    m_notificationBadge = new QLabel(ui->notification_button);
    // Position badge in top-right corner of 40px button: x=22 (40-18), y=2, width=18, height=18
    m_notificationBadge->setGeometry(22, 2, 18, 18);
    m_notificationBadge->setStyleSheet(
        "QLabel {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    border-radius: 9px;"
        "    font-size: 9px;"
        "    font-weight: bold;"
        "    text-align: center;"
        "}"
    );
    m_notificationBadge->setAlignment(Qt::AlignCenter);
    m_notificationBadge->hide(); // Initially hidden
    
    // Initialize audio for notifications
    if (!m_audioOutput) {
        qDebug() << "Initializing notification audio system";
        m_audioOutput = new QAudioOutput(this);
        m_notificationSound = new QMediaPlayer(this);
        m_notificationSound->setAudioOutput(m_audioOutput);
        
        // Use the correct resource path based on our resources.qrc structure
        QUrl audioUrl("qrc:/notification.wav");
        qDebug() << "Setting audio source to:" << audioUrl.toString();
        
        // Check if the resource exists using different formats
        QFile resourceFile1(":/notification.wav");
        QFile resourceFile2("qrc:/notification.wav");
        qDebug() << "Audio resource exists (:/notification.wav):" << resourceFile1.exists();
        qDebug() << "Audio resource exists (qrc:/notification.wav):" << resourceFile2.exists();
        
        // Connect to error and status signals for debugging
        connect(m_notificationSound, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
            qDebug() << "QMediaPlayer error:" << error << errorString;
        });
        
        connect(m_notificationSound, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
            qDebug() << "QMediaPlayer status changed:" << status;
            if (status == QMediaPlayer::LoadedMedia) {
                qDebug() << "Media successfully loaded, duration:" << m_notificationSound->duration();
            } else if (status == QMediaPlayer::InvalidMedia) {
                qDebug() << "Invalid media - checking resource path";
                qDebug() << "Current source:" << m_notificationSound->source().toString();
            }
        });
        
        // Set the source and try to load it
        m_notificationSound->setSource(audioUrl);
        qDebug() << "Audio source set, initial status:" << m_notificationSound->mediaStatus();
    }
    
    // Connect notification button
    connect(ui->notification_button, &QPushButton::clicked, this, &MainWindow::onNotificationsClicked);
}

void MainWindow::onNotificationsClicked()
{
    showNotificationsDialog();
    // Mark all notifications as read
    for (auto& notification : m_notifications) {
        notification.isRead = true;
    }
    updateNotificationBadge();
}

void MainWindow::addNotification(const QString& message)
{
    qDebug() << "Adding notification:" << message;
    m_notifications.append(NotificationItem(message));
    updateNotificationBadge();
}

void MainWindow::updateNotificationBadge()
{
    if (!m_notificationBadge) {
        qWarning() << "updateNotificationBadge called, but m_notificationBadge is nullptr!";
        return;
    }

    // Count unread notifications
    int unreadCount = 0;
    for (const auto& notification : std::as_const(m_notifications)) {
        if (!notification.isRead) {
            unreadCount++;
        }
    }

    if (unreadCount > 0) {
        m_notificationBadge->setText(QString::number(unreadCount));
        m_notificationBadge->show();
    } else {
        m_notificationBadge->hide();
    }
}

void MainWindow::showNotificationsDialog()
{
    QDialog* notificationDialog = new QDialog(this);
    notificationDialog->setWindowTitle("Plant Notifications");
    notificationDialog->setModal(true);
    notificationDialog->setFixedSize(500, 600);
    
    QVBoxLayout* layout = new QVBoxLayout(notificationDialog);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel* titleLabel = new QLabel("Plant Notifications", notificationDialog);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; }");
    layout->addWidget(titleLabel);
    
    // Always create tab widget for unread/read sections
    QTabWidget* tabWidget = new QTabWidget(notificationDialog);
    
    // Separate notifications into unread and read
    QList<NotificationItem> unreadNotifications;
    QList<NotificationItem> readNotifications;
    
    for (const auto& notification : m_notifications) {
        if (notification.isRead) {
            readNotifications.append(notification);
        } else {
            unreadNotifications.append(notification);
        }
    }
        
        // Create unread tab
        QWidget* unreadWidget = new QWidget();
        QVBoxLayout* unreadLayout = new QVBoxLayout(unreadWidget);
        QScrollArea* unreadScrollArea = new QScrollArea();
        QWidget* unreadContentWidget = new QWidget();
        QVBoxLayout* unreadContentLayout = new QVBoxLayout(unreadContentWidget);
        
        if (unreadNotifications.isEmpty()) {
            QLabel* noUnreadLabel = new QLabel("No unread notifications");
            noUnreadLabel->setAlignment(Qt::AlignCenter);
            noUnreadLabel->setStyleSheet("QLabel { font-style: italic; padding: 20px; }");
            unreadContentLayout->addWidget(noUnreadLabel);
        } else {
            for (const auto& notification : unreadNotifications) {
                QWidget* notificationWidget = new QWidget();
                notificationWidget->setMinimumHeight(40); // Set minimum height, allow expansion
                QHBoxLayout* notificationLayout = new QHBoxLayout(notificationWidget);
                
                QLabel* messageLabel = new QLabel(notification.message);
                messageLabel->setWordWrap(true);
                messageLabel->setStyleSheet("QLabel { font-weight: bold; }");
                messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                
                QLabel* timestampLabel = new QLabel(notification.timestamp.toString("MMM dd hh:mm"));
                timestampLabel->setStyleSheet("QLabel { font-size: 10px; color: palette(dark); }");
                timestampLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
                timestampLabel->setAlignment(Qt::AlignTop); // Align timestamp to top for multi-line messages
                
                notificationLayout->addWidget(messageLabel);
                notificationLayout->addWidget(timestampLabel);
                notificationLayout->setContentsMargins(8, 4, 8, 4);
                notificationLayout->setSpacing(10);
                
                notificationWidget->setStyleSheet(
                    "QWidget {"
                    "    padding: 4px 8px;"
                    "    border: 1px solid palette(mid);"
                    "    border-radius: 3px;"
                    "    background-color: palette(alternate-base);"
                    "    margin-bottom: 2px;"
                    "}"
                );
                
                unreadContentLayout->addWidget(notificationWidget);
            }
        }
        
        // Add stretch to push notifications to top
        unreadContentLayout->addStretch();
        
        unreadScrollArea->setWidget(unreadContentWidget);
        unreadScrollArea->setWidgetResizable(true);
        unreadLayout->addWidget(unreadScrollArea);
        
        // Create read tab
        QWidget* readWidget = new QWidget();
        QVBoxLayout* readLayout = new QVBoxLayout(readWidget);
        QScrollArea* readScrollArea = new QScrollArea();
        QWidget* readContentWidget = new QWidget();
        QVBoxLayout* readContentLayout = new QVBoxLayout(readContentWidget);
        
        if (readNotifications.isEmpty()) {
            QLabel* noReadLabel = new QLabel("No read notifications");
            noReadLabel->setAlignment(Qt::AlignCenter);
            noReadLabel->setStyleSheet("QLabel { font-style: italic; padding: 20px; }");
            readContentLayout->addWidget(noReadLabel);
        } else {
            for (const auto& notification : readNotifications) {
                QWidget* notificationWidget = new QWidget();
                notificationWidget->setMinimumHeight(40); // Set minimum height, allow expansion
                QHBoxLayout* notificationLayout = new QHBoxLayout(notificationWidget);
                
                QLabel* messageLabel = new QLabel(notification.message);
                messageLabel->setWordWrap(true);
                messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                
                QLabel* timestampLabel = new QLabel(notification.timestamp.toString("MMM dd hh:mm"));
                timestampLabel->setStyleSheet("QLabel { font-size: 10px; color: palette(dark); }");
                timestampLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
                timestampLabel->setAlignment(Qt::AlignTop); // Align timestamp to top for multi-line messages
                
                notificationLayout->addWidget(messageLabel);
                notificationLayout->addWidget(timestampLabel);
                notificationLayout->setContentsMargins(8, 4, 8, 4);
                notificationLayout->setSpacing(10);
                
                notificationWidget->setStyleSheet(
                    "QWidget {"
                    "    padding: 4px 8px;"
                    "    border: 1px solid palette(mid);"
                    "    border-radius: 3px;"
                    "    background-color: palette(base);"
                    "    margin-bottom: 2px;"
                    "}"
                );
                
                readContentLayout->addWidget(notificationWidget);
            }
        }
        
        // Add stretch to push notifications to top
        readContentLayout->addStretch();
        
        readScrollArea->setWidget(readContentWidget);
        readScrollArea->setWidgetResizable(true);
        readLayout->addWidget(readScrollArea);
        
        // Add tabs to tab widget
        tabWidget->addTab(unreadWidget, QString("Unread (%1)").arg(unreadNotifications.size()));
        tabWidget->addTab(readWidget, QString("Read (%1)").arg(readNotifications.size()));
        
        layout->addWidget(tabWidget);
    
    // Buttons layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    QPushButton* clearAllButton = new QPushButton("Clear All", notificationDialog);
    clearAllButton->setStyleSheet(
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border: 1px solid palette(mid);"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: palette(highlight);"
        "    color: palette(highlighted-text);"
        "}"
    );
    
    QPushButton* closeButton = new QPushButton("Close", notificationDialog);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border: 1px solid palette(mid);"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: palette(highlight);"
        "    color: palette(highlighted-text);"
        "}"
    );
    
    connect(clearAllButton, &QPushButton::clicked, [this, notificationDialog]() {
        m_notifications.clear();
        updateNotificationBadge();
        notificationDialog->accept();
    });
    
    connect(closeButton, &QPushButton::clicked, notificationDialog, &QDialog::accept);
    
    buttonLayout->addWidget(clearAllButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    layout->addLayout(buttonLayout);
    
    notificationDialog->exec();
    notificationDialog->deleteLater();
}

void MainWindow::playNotificationSound()
{
    qDebug() << "playNotificationSound called - Volume:" << m_currentVolume 
             << "Sound object:" << (m_notificationSound != nullptr)
             << "Audio object:" << (m_audioOutput != nullptr);
             
    if (m_currentVolume == 0) {
        qDebug() << "Volume is 0, not playing sound";
        return;
    }
             
    if (m_notificationSound && m_audioOutput) {
        // Set volume based on user preference (0-100 to 0.0-1.0)
        qreal volume = m_currentVolume / 100.0;
        m_audioOutput->setVolume(volume);
        qDebug() << "Playing notification sound with volume:" << volume;
        qDebug() << "QMediaPlayer state:" << m_notificationSound->playbackState();
        qDebug() << "QMediaPlayer status:" << m_notificationSound->mediaStatus();
        
        // Check if media is loaded, if not try to load it
        if (m_notificationSound->mediaStatus() == QMediaPlayer::InvalidMedia || 
            m_notificationSound->mediaStatus() == QMediaPlayer::NoMedia) {
            qDebug() << "Media not loaded, trying to reload with correct resource path...";
            // Use the correct resource path that matches our resources.qrc
            QUrl audioUrl("qrc:/notification.wav");
            qDebug() << "Reloading audio source:" << audioUrl.toString();
            m_notificationSound->setSource(audioUrl);
            
            // Wait a moment for loading and then try to play
            QTimer::singleShot(300, this, [this]() {
                qDebug() << "After reload - QMediaPlayer status:" << m_notificationSound->mediaStatus();
                qDebug() << "After reload - QMediaPlayer source:" << m_notificationSound->source().toString();
                if (m_notificationSound->mediaStatus() == QMediaPlayer::LoadedMedia) {
                    qDebug() << "Media loaded successfully, playing...";
                    m_notificationSound->play();
                } else {
                    qDebug() << "Media still not loaded after reload, using system beep";
                    QApplication::beep();
                }
            });
        } else if (m_notificationSound->mediaStatus() == QMediaPlayer::LoadedMedia) {
            qDebug() << "Media already loaded, playing directly...";
            m_notificationSound->play();
        } else {
            qDebug() << "Media status:" << m_notificationSound->mediaStatus() << ", trying to play anyway";
            m_notificationSound->play();
            
            // If it doesn't start playing within 200ms, fall back to system beep
            QTimer::singleShot(200, this, [this]() {
                if (m_notificationSound->playbackState() != QMediaPlayer::PlayingState) {
                    qDebug() << "QMediaPlayer failed to play, falling back to system beep";
                    QApplication::beep();
                }
            });
        }
    } else {
        qDebug() << "Audio objects not available, using system beep";
        QApplication::beep();
    }
}

void MainWindow::checkPlantConditions()
{
    if (!m_plantManager) return;
    
    int plantCount = m_plantManager->getPlantCount();
    for (int i = 0; i < plantCount; ++i) {
        PlantData plant = m_plantManager->getPlant(i);
        
        // Check temperature
        if (m_currentTemp > 0) {
            QString tempKey = QString("%1_temp").arg(plant.name);
            if (m_currentTemp < plant.tempRangeLow) {
                QString tempLowKey = tempKey + "_low";
                if (!m_sentNotifications.contains(tempLowKey)) {
                    addNotification(QString("%1 temperature too low (%2°F < %3°F)")
                        .arg(plant.name).arg(m_currentTemp, 0, 'f', 1).arg(plant.tempRangeLow, 0, 'f', 1));
                    m_sentNotifications.insert(tempLowKey);
                }
            } else if (m_currentTemp > plant.tempRangeHigh) {
                QString tempHighKey = tempKey + "_high";
                if (!m_sentNotifications.contains(tempHighKey)) {
                    addNotification(QString("%1 temperature too high (%2°F > %3°F)")
                        .arg(plant.name).arg(m_currentTemp, 0, 'f', 1).arg(plant.tempRangeHigh, 0, 'f', 1));
                    m_sentNotifications.insert(tempHighKey);
                }
            } else {
                // Temperature is in range, clear notifications for this plant
                m_sentNotifications.remove(tempKey + "_low");
                m_sentNotifications.remove(tempKey + "_high");
            }
        }
        
        // Check humidity
        if (m_currentHumidity > 0) {
            QString humidityKey = QString("%1_humidity").arg(plant.name);
            if (m_currentHumidity < plant.humidityRangeLow) {
                QString humidityLowKey = humidityKey + "_low";
                if (!m_sentNotifications.contains(humidityLowKey)) {
                    addNotification(QString("%1 humidity too low (%2% < %3%)")
                        .arg(plant.name).arg(m_currentHumidity, 0, 'f', 1).arg(plant.humidityRangeLow, 0, 'f', 1));
                    m_sentNotifications.insert(humidityLowKey);
                }
            } else if (m_currentHumidity > plant.humidityRangeHigh) {
                QString humidityHighKey = humidityKey + "_high";
                if (!m_sentNotifications.contains(humidityHighKey)) {
                    addNotification(QString("%1 humidity too high (%2% > %3%)")
                        .arg(plant.name).arg(m_currentHumidity, 0, 'f', 1).arg(plant.humidityRangeHigh, 0, 'f', 1));
                    m_sentNotifications.insert(humidityHighKey);
                }
            } else {
                // Humidity is in range, clear notifications for this plant
                m_sentNotifications.remove(humidityKey + "_low");
                m_sentNotifications.remove(humidityKey + "_high");
            }
        }
        
        // Check UV
        if (m_currentUV > 0) {
            QString uvKey = QString("%1_uv_low").arg(plant.name);
            if (m_currentUV < plant.uvRangeLow) {
                if (!m_sentNotifications.contains(uvKey)) {
                    addNotification(QString("%1 UV level too low (%2 < %3)")
                        .arg(plant.name).arg(m_currentUV, 0, 'f', 1).arg(plant.uvRangeLow, 0, 'f', 1));
                    m_sentNotifications.insert(uvKey);
                }
            } else {
                // UV is in range, clear notification for this plant
                m_sentNotifications.remove(uvKey);
            }
        }
    }
}

void MainWindow::checkForecastTemperature() {
    forecastClient->getURL();
}

void MainWindow::handleForecastNotification(const QList<DayForecast> &forecasts) {
    // Get tomorrow's data and check plant manager availability
    QDate tomorrow = QDate::currentDate().addDays(1);
    if (!m_plantManager) return;

    // Find tomorrow's forecast from the list
    const DayForecast* tomorrowForecast = nullptr;
    for (const DayForecast &day : forecasts) {
        if (day.date == tomorrow) {
            tomorrowForecast = &day;
            break;
        }
    }
    if (!tomorrowForecast) return;

    // Extract forecast data for easier access
    QString dateStr = tomorrow.toString(Qt::ISODate);
    int highTemp = tomorrowForecast->highTemp;
    int humidity = tomorrowForecast->humidity;
    int uvIndex = tomorrowForecast->UV;


    // Process each plant's ranges against tomorrow's forecast
    int plantCount = m_plantManager->getPlantCount();
    for (int i = 0; i < plantCount; ++i) {
        PlantData plant = m_plantManager->getPlant(i);

        // Temperature high
        QString tempHighKey = QString("%1_forecast_temp_high_%2").arg(plant.name, dateStr);
        if (highTemp >= plant.tempRangeHigh && !m_sentNotifications.contains(tempHighKey)) {
            addNotification(QString("Warning! %1's temperature is %2°F, which is going to be too hot for %3! Consider moving it to a shaded spot.")
                                .arg(tomorrowForecast->dayName).arg(highTemp).arg(plant.name));
            m_sentNotifications.insert(tempHighKey);
        }

        // Humidity low
        QString humidityLowKey = QString("%1_forecast_humidity_low_%2").arg(plant.name, dateStr);
        if (humidity <= plant.humidityRangeLow && !m_sentNotifications.contains(humidityLowKey)) {
            addNotification(QString("Warning! %1's humidity is %2%, which is below your %3's ideal humidity range. Consider misting it or moving it to a more humid spot.")
                                .arg(tomorrowForecast->dayName).arg(humidity).arg(plant.name));
            m_sentNotifications.insert(humidityLowKey);
        }

        // Humidity high
        QString humidityHighKey = QString("%1_forecast_humidity_high_%2").arg(plant.name, dateStr);
        if (humidity >= plant.humidityRangeHigh && !m_sentNotifications.contains(humidityHighKey)) {
            addNotification(QString("Warning! %1's humidity is %2%, which is above your %3's ideal humidity range. Watch out for fungal growth and provide good air circulation.")
                                .arg(tomorrowForecast->dayName).arg(humidity).arg(plant.name));
            m_sentNotifications.insert(humidityHighKey);
        }

        // UV high
        QString uvHighKey = QString("%1_forecast_uv_high_%2").arg(plant.name, dateStr);
        if (uvIndex >= plant.uvRangeHigh && !m_sentNotifications.contains(uvHighKey)) {
            addNotification(QString("Warning! %1's UV Index is %2, which might damage your %3! Consider shading or moving indoors.")
                                .arg(tomorrowForecast->dayName).arg(uvIndex).arg(plant.name));
            m_sentNotifications.insert(uvHighKey);
        }
    }
}

