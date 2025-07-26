#ifndef PLANTPEDIAWIDGET_H
#define PLANTPEDIAWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "plantpediamanager.h"

class PlantPediaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlantPediaWidget(QWidget *parent = nullptr);

private slots:
    void onSearch();
    void onEntryFound(const PlantPediaEntry &entry);
    void onError(const QString &error);

private:
    QLineEdit *m_searchBar;
    QPushButton *m_searchButton;
    QLabel *m_resultLabel;
    PlantPediaManager *m_manager;
};

#endif // PLANTPEDIAWIDGET_H