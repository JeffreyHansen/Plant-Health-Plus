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
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QFrame>
#include <QFormLayout>
#include <QDialog>
#include <QSlider>
#include <QTabWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QSet>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QMessageBox>
#include <algorithm>
#include "plantcard.h"
#include "plantmanager.h"
#include "addplantdialog.h"
#include "usermanager.h"
#include "logbookmanager.h"
#include "logbookentrydialog.h"

// Notification structure
struct NotificationItem {
    QString message;
    QDateTime timestamp;
    bool isRead;
    
    NotificationItem(const QString& msg) 
        : message(msg), timestamp(QDateTime::currentDateTime()), isRead(false) {}
};

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
    void onLoginClicked();
    void onRegisterClicked();
    void togglePasswordVisibility();
    void onSettingsClicked();
    void onLogoutClicked();
    void onNotificationsClicked();
    void checkPlantConditions();
    void addNotification(const QString& message);
    
    // Logbook slots
    void onAddLogbookEntryClicked();
    void onEditLogbookEntryClicked();
    void onDeleteLogbookEntryClicked();
    void onLogbookEntrySelected(const QString& entryId);

private:
    void setupLoginPage();
    void setupMainInterface();
    void showMainInterface();
    bool validateLoginInput();
    void authenticateUser();
    void showSuccessMessage(const QString& message);
    void showErrorMessage(const QString& message);
    void setupMyPlantsPage();
    void updatePlantGrid();
    void clearPlantGrid();
    PlantCard* createPlantCard(const PlantData& plantData, int index);
    void updateScrollAreaTheme();
    void setupResponsiveLayout();
    void setupPlantUILayout();
    void updateAllPlantCardsEnvironmentalData();
    void showSettingsDialog();
    void setupNotificationButton();
    void updateNotificationBadge();
    void showNotificationsDialog();
    void playNotificationSound();
    
    // Logbook methods
    void setupLogbookPage();
    void updateLogbookEntryList();
    void displayLogbookEntry(const QString& entryId);
    void clearLogbookDisplay();
    void clearLogbookImagesDisplay();
    void adjustTabLayout();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* object, QEvent* event) override;

    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup;
    QStackedWidget *stackedWidget;
    
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
    
    // Login page components
    QWidget* m_loginPage;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_registerButton;
    QPushButton* m_togglePasswordButton;
    QLabel* m_statusLabel;
    QLabel* m_confirmLabel;
    QWidget* m_mainInterface;
    UserManager* m_userManager;
    bool m_isRegisterMode;
    
    // UI helpers
    QTimer* m_resizeTimer;
    
    // Notification system
    QPushButton* m_notificationButton;
    QLabel* m_notificationBadge;
    QList<NotificationItem> m_notifications;
    QSet<QString> m_sentNotifications; // Track sent notifications to prevent duplicates
    QMediaPlayer* m_notificationSound;
    QAudioOutput* m_audioOutput;
    int m_currentVolume;
    
    // Logbook system
    LogbookManager* m_logbookManager;
    QListWidget* m_logbookEntryList;
    QTextEdit* m_logbookContentDisplay;
    QScrollArea* m_logbookImagesArea;
    QWidget* m_logbookImagesContainer;
    QGridLayout* m_logbookImagesLayout;
    QPushButton* m_addLogbookEntryButton;
    QPushButton* m_editLogbookEntryButton;
    QPushButton* m_deleteLogbookEntryButton;
    QLabel* m_logbookEntryTitle;
    QLabel* m_logbookEntryDates;
    QString m_selectedLogbookEntryId;
    
    // Constants
    static const int CARDS_PER_ROW = 2;
};
#endif // MAINWINDOW_H
