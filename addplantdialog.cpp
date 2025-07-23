#include "addplantdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QGroupBox>

AddPlantDialog::AddPlantDialog(QWidget *parent)
    : QDialog(parent)
    , m_editMode(false)
{
    setupUI();
    setWindowTitle("Add New Plant");
}

AddPlantDialog::AddPlantDialog(const PlantData& plantData, QWidget *parent)
    : QDialog(parent)
    , m_editMode(true)
{
    setupUI();
    populateFields(plantData);
    setWindowTitle("Edit Plant");
}

void AddPlantDialog::setupUI()
{
    setFixedSize(400, 600);
    setModal(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create form layout for input fields
    QFormLayout* formLayout = new QFormLayout();
    
    // Plant name
    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("Enter plant name...");
    formLayout->addRow("Plant Name:", m_nameEdit);
    
    // Category
    m_categoryCombo = new QComboBox();
    m_categoryCombo->addItems({
        "Tropical", "Succulent", "Vine", "Fern", "Flowering", 
        "Herb", "Tree", "Shrub", "Grass", "Other"
    });
    m_categoryCombo->setEditable(true);
    formLayout->addRow("Category:", m_categoryCombo);
    
    // Watering interval
    m_wateringIntervalSpin = new QDoubleSpinBox();
    m_wateringIntervalSpin->setRange(0.1, 365.0);  // Allow fractional days from 0.1 to 365
    m_wateringIntervalSpin->setValue(7.0);
    m_wateringIntervalSpin->setSingleStep(0.5);  // Allow 0.5 day increments
    m_wateringIntervalSpin->setDecimals(1);  // Show 1 decimal place
    m_wateringIntervalSpin->setSuffix(" days");
    formLayout->addRow("Watering Interval:", m_wateringIntervalSpin);
    
    // Light requirement
    m_lightCombo = new QComboBox();
    m_lightCombo->addItems({
        "Low light", "Medium light", "Bright, indirect light", 
        "Bright, direct light", "Full sun"
    });
    m_lightCombo->setEditable(true);
    formLayout->addRow("Light Requirement:", m_lightCombo);
    
    // Soil type
    m_soilEdit = new QLineEdit();
    m_soilEdit->setPlaceholderText("e.g., Well-draining potting mix");
    formLayout->addRow("Soil Type:", m_soilEdit);
    
    // Fertilizer
    m_fertilizerEdit = new QLineEdit();
    m_fertilizerEdit->setPlaceholderText("e.g., Monthly liquid fertilizer");
    formLayout->addRow("Fertilizer:", m_fertilizerEdit);
    
    // Card Color
    m_colorCombo = new QComboBox();
    m_colorCombo->addItems({
        "White", "Light Green", "Light Blue", "Light Yellow", "Light Pink", 
        "Light Purple", "Light Orange", "Light Gray", "Mint Green", "Lavender"
    });
    formLayout->addRow("Card Color:", m_colorCombo);
    
    // House Area
    m_areaCombo = new QComboBox();
    m_areaCombo->setEditable(false);
    m_areaCombo->addItems({
        "Living Room", "Bedroom", "Kitchen", "Bathroom", "Balcony", 
        "Backyard", "Garden", "Office", "Dining Room", "Other"
    });
    
    // Area selection with add new area functionality
    QWidget* areaWidget = new QWidget();
    QHBoxLayout* areaLayout = new QHBoxLayout(areaWidget);
    areaLayout->setContentsMargins(0, 0, 0, 0);
    
    m_newAreaEdit = new QLineEdit();
    m_newAreaEdit->setPlaceholderText("Or enter new area...");
    m_newAreaEdit->setVisible(false);
    
    m_addAreaButton = new QPushButton("Add New");
    m_addAreaButton->setMaximumWidth(80);
    
    areaLayout->addWidget(m_areaCombo);
    areaLayout->addWidget(m_addAreaButton);
    
    formLayout->addRow("House Area:", areaWidget);
    formLayout->addRow("", m_newAreaEdit);  // New area input (initially hidden)
    
    connect(m_addAreaButton, &QPushButton::clicked, this, &AddPlantDialog::addNewArea);
    
    // Image selection
    QGroupBox* imageGroup = new QGroupBox("Plant Image");
    QVBoxLayout* imageLayout = new QVBoxLayout(imageGroup);
    
    m_imageLabel = new QLabel();
    m_imageLabel->setFixedSize(120, 120);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet(
        "QLabel {"
        "    border: 2px dashed #ccc;"
        "    border-radius: 8px;"
        "    background-color: #f9f9f9;"
        "}"
    );
    m_imageLabel->setText("No image selected");
    
    m_selectImageButton = new QPushButton("Select Image");
    m_selectImageButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
    );
    
    connect(m_selectImageButton, &QPushButton::clicked, this, &AddPlantDialog::selectImage);
    
    QHBoxLayout* imageButtonLayout = new QHBoxLayout();
    imageButtonLayout->addWidget(m_imageLabel);
    imageButtonLayout->addWidget(m_selectImageButton);
    
    imageLayout->addLayout(imageButtonLayout);
    
    // Notes
    m_notesEdit = new QTextEdit();
    m_notesEdit->setPlaceholderText("Enter any additional notes about this plant...");
    m_notesEdit->setMaximumHeight(100);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_okButton = new QPushButton(m_editMode ? "Update Plant" : "Add Plant");
    m_okButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
    );
    
    m_cancelButton = new QPushButton("Cancel");
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #757575;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #616161;"
        "}"
    );
    
    connect(m_okButton, &QPushButton::clicked, this, &AddPlantDialog::onAccepted);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_okButton);
    
    // Add everything to main layout
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(imageGroup);
    mainLayout->addWidget(new QLabel("Notes:"));
    mainLayout->addWidget(m_notesEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Set tab order
    setTabOrder(m_nameEdit, m_categoryCombo);
    setTabOrder(m_categoryCombo, m_wateringIntervalSpin);
    setTabOrder(m_wateringIntervalSpin, m_lightCombo);
    setTabOrder(m_lightCombo, m_soilEdit);
    setTabOrder(m_soilEdit, m_fertilizerEdit);
    setTabOrder(m_fertilizerEdit, m_selectImageButton);
    setTabOrder(m_selectImageButton, m_notesEdit);
    setTabOrder(m_notesEdit, m_okButton);
    setTabOrder(m_okButton, m_cancelButton);
    
    // Set default values
    m_soilEdit->setText("Well-draining potting mix");
    m_fertilizerEdit->setText("Monthly liquid fertilizer");
    m_lightCombo->setCurrentText("Bright, indirect light");
}

void AddPlantDialog::populateFields(const PlantData& plantData)
{
    m_nameEdit->setText(plantData.name);
    m_categoryCombo->setCurrentText(plantData.category);
    m_wateringIntervalSpin->setValue(plantData.wateringIntervalDays);
    m_lightCombo->setCurrentText(plantData.lightRequirement);
    m_soilEdit->setText(plantData.soilType);
    m_fertilizerEdit->setText(plantData.fertilizer);
    m_notesEdit->setText(plantData.notes);
    
    // Set the selected color
    if (!plantData.cardColor.isEmpty()) {
        m_colorCombo->setCurrentText(plantData.cardColor);
    }
    
    // Set the selected area
    if (!plantData.houseArea.isEmpty()) {
        m_areaCombo->setCurrentText(plantData.houseArea);
    }
    
    if (!plantData.imagePath.isEmpty()) {
        m_selectedImagePath = plantData.imagePath;
        QPixmap pixmap(plantData.imagePath);
        if (!pixmap.isNull()) {
            m_imageLabel->setPixmap(pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void AddPlantDialog::selectImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Plant Image",
        "",
        "Image Files (*.png *.jpg *.jpeg *.bmp *.gif)"
    );
    
    if (!fileName.isEmpty()) {
        m_selectedImagePath = fileName;
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            m_imageLabel->setPixmap(pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QMessageBox::warning(this, "Invalid Image", "The selected file is not a valid image.");
        }
    }
}

void AddPlantDialog::onAccepted()
{
    // Validate input
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a plant name.");
        m_nameEdit->setFocus();
        return;
    }
    
    if (m_categoryCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please select or enter a category.");
        m_categoryCombo->setFocus();
        return;
    }
    
    accept();
}

PlantData AddPlantDialog::getPlantData() const
{
    return createPlantData();
}

PlantData AddPlantDialog::createPlantData() const
{
    PlantData plant;
    plant.name = m_nameEdit->text().trimmed();
    plant.category = m_categoryCombo->currentText().trimmed();
    plant.imagePath = m_selectedImagePath;
    plant.lastWatered = QDateTime::currentDateTime(); // Default to now for new plants
    plant.wateringIntervalDays = m_wateringIntervalSpin->value();
    plant.notes = m_notesEdit->toPlainText().trimmed();
    plant.lightRequirement = m_lightCombo->currentText().trimmed();
    plant.soilType = m_soilEdit->text().trimmed();
    plant.fertilizer = m_fertilizerEdit->text().trimmed();
    plant.dateAdded = m_editMode ? QDateTime() : QDateTime::currentDateTime(); // Don't change dateAdded when editing
    
    // New fields
    plant.cardColor = m_colorCombo->currentText();
    plant.houseArea = m_areaCombo->currentText();
    
    return plant;
}

void AddPlantDialog::addNewArea()
{
    if (m_newAreaEdit->isVisible()) {
        // Add the new area to the combo box
        QString newArea = m_newAreaEdit->text().trimmed();
        if (!newArea.isEmpty()) {
            // Check if area already exists
            bool areaExists = false;
            for (int i = 0; i < m_areaCombo->count(); ++i) {
                if (m_areaCombo->itemText(i).compare(newArea, Qt::CaseInsensitive) == 0) {
                    areaExists = true;
                    break;
                }
            }
            
            if (!areaExists) {
                m_areaCombo->addItem(newArea);
                m_areaCombo->setCurrentText(newArea);
            } else {
                m_areaCombo->setCurrentText(newArea);
            }
            
            m_newAreaEdit->clear();
        }
        
        // Hide the input field and change button text back
        m_newAreaEdit->setVisible(false);
        m_addAreaButton->setText("Add New");
    } else {
        // Show the input field and change button text
        m_newAreaEdit->setVisible(true);
        m_newAreaEdit->setFocus();
        m_addAreaButton->setText("Add");
    }
}
