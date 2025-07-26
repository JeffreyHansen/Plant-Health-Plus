#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Scraper : public QObject {
	Q_OBJECT

public:
	explicit Scraper(QObject* parent = nullptr);
    void getURL();

signals:
	void tempReady(const QString& temp);
    void humidReady(const QString& humid);
    void uvReady(const QString& uv);

private slots:
	void handleNetReply(QNetworkReply* reply);

private:
	QNetworkAccessManager* manager;
};
