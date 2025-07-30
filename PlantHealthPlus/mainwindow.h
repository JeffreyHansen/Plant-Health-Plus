#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QResizeEvent>
#include <QTimer>
#include <QEvent>
#include "plantcard.h"
#include "plantmanager.h"
#include "conditionchart.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTabButtonClicked(int id);
    void onAddPlantClicked();
    void onPlantAdded(int index);
    void onPlantRemoved(int index);
    void onPlantUpdated(int index);
    void onPlantCardEdit(PlantCard* card, int index);
    void onPlantCardDelete(PlantCard* card, int index);
    void onPlantCardExpanded(PlantCard* card, bool expanded);
    void refreshPlantGrid();
    void onThemeChanged(Qt::ColorScheme colorScheme);
    void onResizeTimeout();
    void onConditionClicked();

private:
    void setupMyPlantsPage();
    void setupConditionLayout();
    void updatePlantGrid();
    void clearPlantGrid();
    PlantCard* createPlantCard(const PlantData& plantData, int index);
    void updateScrollAreaTheme();
    void setupResponsiveLayout();
    void setupPlantUILayout();
    void updateAllPlantCardsEnvironmentalData();
    QPushButton* m_tempButton;
    QPushButton* m_uvButton;
    QPushButton* m_humidityButton;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* object, QEvent* event) override;

    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup;
    QStackedWidget *stackedWidget;

    ConditionChart *conditionChart;
    
    // Plant management
    PlantManager* m_plantManager;
    QScrollArea* m_plantsScrollArea;
    QWidget* m_plantsContainer;
    QGridLayout* m_plantsGridLayout;
    QPushButton* m_addPlantButton;
    QVector<PlantCard*> m_plantCards;
    
    // Environmental data
    double m_currentTemp = 0.0;
    double m_currentHumidity = 0.0;
    double m_currentUV = 0.0;
    
    // UI helpers
    QTimer* m_resizeTimer;
    
    // Constants
    static const int CARDS_PER_ROW = 2;
};
#endif // MAINWINDOW_H
