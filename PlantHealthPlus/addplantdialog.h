#ifndef ADDPLANTDIALOG_H
#define ADDPLANTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include "plantcard.h"

class AddPlantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPlantDialog(QWidget *parent = nullptr);
    explicit AddPlantDialog(const PlantData& plantData, QWidget *parent = nullptr);
    
    PlantData getPlantData() const;

private slots:
    void selectImage();
    void onAccepted();
    void addNewArea();

private:
    void setupUI();
    void populateFields(const PlantData& plantData);
    PlantData createPlantData() const;

    // UI Elements
    QLineEdit* m_nameEdit;
    QComboBox* m_categoryCombo;
    QDoubleSpinBox* m_wateringIntervalSpin;
    QTextEdit* m_notesEdit;
    QComboBox* m_lightCombo;
    QLineEdit* m_soilEdit;
    QLineEdit* m_fertilizerEdit;
    QLabel* m_imageLabel;
    QPushButton* m_selectImageButton;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    
    // New UI Elements
    QComboBox* m_colorCombo;
    QComboBox* m_areaCombo;
    QLineEdit* m_newAreaEdit;  // For creating new areas
    QPushButton* m_addAreaButton;

    QString m_selectedImagePath;
    bool m_editMode;
};

#endif // ADDPLANTDIALOG_H
