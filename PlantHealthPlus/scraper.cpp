#include "scraper.h"
#include <QRegularExpression>
#include <QDebug>

Scraper::Scraper(QObject* parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
        this, &::Scraper::handleNetReply);
}

void Scraper::getURL() {
    qDebug() << "Scraper: Starting to fetch weather data...";
    QUrl url("https://weather.com/weather/today/l/58ce32964ce60c21632b79f2eb9dc596af190484e5639847e40fc43b45225dbe");
        QNetworkRequest request(url);
    manager->get(request);
}

void Scraper::handleNetReply(QNetworkReply* reply) {
    QString html = reply->readAll();
    reply->deleteLater();
    
    qDebug() << "Scraper: Received HTML response, length:" << html.length();

    QString temp = "N/A";
    QString humid = "N/A";
    QString uv = "N/A";

    QRegularExpression tempRegex(R"(<span[^>]*data-testid="TemperatureValue"[^>]*>(\d+))");
    QRegularExpressionMatch tempMatch = tempRegex.match(html);
    if (tempMatch.hasMatch()) {
        temp = tempMatch.captured(1);
        temp += "°";
        qDebug() << "Scraper: Found temperature:" << temp;
    } else {
        qDebug() << "Scraper: Temperature regex did not match";
    }
    emit tempReady(temp);

    QRegularExpression humidRegex(R"(Humidity<\/span>\s*<span[^>]*data-testid="PercentageValue"[^>]*>([^<]+)<\/span>)");
    QRegularExpressionMatch humidMatch = humidRegex.match(html);
    if (humidMatch.hasMatch()) {
        humid = humidMatch.captured(1);
        qDebug() << "Scraper: Found humidity:" << humid;
    } else {
        qDebug() << "Scraper: Humidity regex did not match";
    }
    emit humidReady(humid);

    QRegularExpression uvRegex(R"(<span[^>]*data-testid="UVIndexValue"[^>]*>(\d+))");
    QRegularExpressionMatch uvMatch = uvRegex.match(html);
    if (uvMatch.hasMatch()) {
        uv = uvMatch.captured(1);
        qDebug() << "Scraper: Found UV:" << uv;
    } else {
        qDebug() << "Scraper: UV regex did not match";
    }
    emit uvReady(uv);
}
