#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Get the button group and stacked widget from the UI
    QButtonGroup* buttonGroup = ui->buttonGroup;
    QStackedWidget* stackedWidget = ui->stackedWidget;

    // Assign IDs to the buttons matching page indexes
    // These IDs correspond to the page indexes in the stacked widget
    buttonGroup->setId(ui->tab1, 0);  // Tab 0 -> page (index 0)
    buttonGroup->setId(ui->tab2, 1);  // Tab 1 -> page_3 (index 1) 
    buttonGroup->setId(ui->tab3, 2);  // Tab 2 -> page_2 (index 2)

    // Connect button clicks to custom slot using old-style syntax
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(onTabButtonClicked(int)));

    // Also try connecting individual buttons as a backup
    connect(ui->tab1, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->tab2, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->tab3, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(2); });

    // Set the initial state
    ui->tab1->setChecked(true);
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
