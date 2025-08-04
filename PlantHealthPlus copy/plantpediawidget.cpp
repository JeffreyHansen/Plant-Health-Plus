#include "plantpediawidget.h"
#include <QVBoxLayout>
#include <QTextBrowser>

PlantPediaWidget::PlantPediaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Enter plant name, e.g., 'Fern' or 'Snake Plant'");
    m_searchButton = new QPushButton("Search", this);
    m_resultLabel = new QLabel("Plant information will appear here.", this);
    m_resultLabel->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_searchBar);
    layout->addWidget(m_searchButton);
    layout->addWidget(m_resultLabel);

    m_manager = new PlantPediaManager(this);

    connect(m_searchButton, &QPushButton::clicked, this, &PlantPediaWidget::onSearch);
    connect(m_manager, &PlantPediaManager::entryFound, this, &PlantPediaWidget::onEntryFound);
    connect(m_manager, &PlantPediaManager::errorOccurred, this, &PlantPediaWidget::onError);
}

void PlantPediaWidget::onSearch()
{
    QString query = m_searchBar->text().trimmed();
    if (!query.isEmpty())
        m_manager->searchPlant(query);
}

void PlantPediaWidget::onEntryFound(const PlantPediaEntry &entry)
{
    QString html = "<h2>" + entry.title + "</h2>";
    if (!entry.description.isEmpty())
        html += "<b>" + entry.description + "</b><br>";
    if (!entry.imageUrl.isEmpty())
        html += QString("<img src=\"%1\" width=\"200\"><br>").arg(entry.imageUrl);
    html += "<p>" + entry.extract + "</p>";
    html += QString("<br><a href=\"https://en.wikipedia.org/wiki/%1\">Read more on Wikipedia</a>").arg(QString(QUrl::toPercentEncoding(entry.title.replace(' ', '_'))));
    m_resultLabel->setText(html);
    m_resultLabel->setTextFormat(Qt::RichText);
    m_resultLabel->setOpenExternalLinks(true);
}

void PlantPediaWidget::onError(const QString &error)
{
    m_resultLabel->setText("<span style='color:red'>" + error + "</span>");
}