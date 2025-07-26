#ifndef PLANTCARD_H
#define PLANTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QFrame>
#include "rangeindicator.h"

struct PlantData {
    QString name;
    QString category;
    QString imagePath;
    QDateTime lastWatered;
    double wateringIntervalDays;  // Changed to double to support fractional days
    QString notes;
    QString lightRequirement;
    QString soilType;
    QString fertilizer;
    QDateTime dateAdded;
    QString cardColor;  // New: Color for the plant card
    QString houseArea;  // New: House area/location (e.g., "Living Room", "Bedroom")
    
    // Environmental ranges
    double tempRangeLow = 60.0;    // Minimum safe temperature (°F)
    double tempRangeHigh = 85.0;   // Maximum safe temperature (°F)
    double humidityRangeLow = 40.0;  // Minimum safe humidity (%)
    double humidityRangeHigh = 70.0; // Maximum safe humidity (%)
    double uvRangeLow = 0.0;       // Minimum safe UV index
    double uvRangeHigh = 6.0;      // Maximum safe UV index
};

class PlantCard : public QFrame
{
    Q_OBJECT

public:
    explicit PlantCard(const PlantData& plantData, QWidget *parent = nullptr);
    ~PlantCard();

    // Getters
    const PlantData& getPlantData() const { return m_plantData; }
    bool isExpanded() const { return m_expanded; }

    // Setters
    void updatePlantData(const PlantData& data);
    void setExpanded(bool expanded);
    void updateEnvironmentalData(double temp, double humidity, double uv);

public slots:
    void toggleExpanded();
    void markWatered();

signals:
    void cardClicked();
    void wateringUpdated();
    void editRequested();
    void deleteRequested();
    void cardExpanded(PlantCard* card, bool expanded);  // Signal when card expands/collapses

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private slots:
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void setupBasicInfo();
    void setupExpandedInfo();
    void updateWateringStatus();
    void animateExpansion();
    QString getColorHexCode(const QString& colorName) const;  // Helper method to get hex code from color name

    // Data
    PlantData m_plantData;
    bool m_expanded;

    // UI Elements - Basic Info
    QVBoxLayout* m_mainLayout;
    QWidget* m_basicWidget;
    QWidget* m_expandedWidget;
    
    QLabel* m_plantImage;
    QLabel* m_plantName;
    QLabel* m_plantCategory;
    QLabel* m_wateringStatus;
    QPushButton* m_waterButton;
    
    // UI Elements - Expanded Info
    QLabel* m_detailedNotes;
    QLabel* m_lightRequirement;
    QLabel* m_soilType;
    QLabel* m_fertilizerInfo;
    QLabel* m_dateAdded;
    QLabel* m_lastWateredLabel;
    QLabel* m_wateringIntervalLabel;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    
    // Environmental range indicators
    RangeIndicator* m_tempIndicator;
    RangeIndicator* m_humidityIndicator;
    RangeIndicator* m_uvIndicator;
    
    // Current environmental values
    double m_currentTemp;
    double m_currentHumidity;
    double m_currentUV;

    // Animation
    QPropertyAnimation* m_animation;
    
public:
    // Constants
    static const int CARD_BASIC_HEIGHT = 150;
    static const int CARD_EXPANDED_HEIGHT = 420; // Increased to accommodate environmental indicators
    static const int CARD_WIDTH = 280;
};

#endif // PLANTCARD_H
