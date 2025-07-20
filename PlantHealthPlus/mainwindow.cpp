#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include "scraper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Scraper* scraper = new Scraper(this);
    connect(scraper, &Scraper::tempReady, this, [this](const QString& temp) {
    qDebug() << temp;
        ui->temp_label->setText(temp);
    });

    connect(scraper, &Scraper::uvReady, this, [this](const QString& uv) {
        qDebug() << uv;
        ui->uv_label->setText(uv);
    });

    connect(scraper, &Scraper::humidReady, this, [this](const QString& humid) {
        qDebug() << humid;
        ui->humid_label->setText(humid);
    });

    //Dynamically update the temperature and ...
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scraper, &Scraper::getURL);
    timer->start(5000);

    // Get the button group and stacked widget from the UI
    QButtonGroup* buttonGroup = ui->buttonGroup;
    QStackedWidget* stackedWidget = ui->stackedWidget;

    // Assign IDs to the buttons matching page indexes
    // These IDs correspond to the page indexes in the stacked widget
    buttonGroup->setId(ui->myplants_tab, 0);  // Tab 0 -> page (index 0)
    buttonGroup->setId(ui->logbook_tab, 1);  // Tab 1 -> page_3 (index 1) 
    buttonGroup->setId(ui->plantpedia_tab, 2);  // Tab 2 -> page_2 (index 2)

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTabButtonClicked(int id)
{
    // This slot receives the button ID and changes the page
    ui->stackedWidget->setCurrentIndex(id);
}


