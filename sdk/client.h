#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QUrl>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDebug>

class PocketBase : public QObject
{
    Q_OBJECT
public:
    PocketBase(const QString& baseUrl = "/", const QString& lang = "en-US", int timeout = 120, QObject* parent = nullptr);

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged FINAL)

    // RecordService* collection(const QString& idOrName);
    // BaseAuthStore* authStore() { return m_authStore; }
    QNetworkReply* send(const QString& path, const QJsonObject params);

    QString filter(const QString& expr, const QMap<QString, QVariant>& query);
    QString getFileToken();
    QUrl buildUrl(const QString& path);

    QString baseUrl() const;
    void setBaseUrl(const QString &newBaseUrl);

    QString lang() const;
    void setLang(const QString &newLang);

signals:
    void baseUrlChanged();

    void langChanged();

    void requestFinished(QNetworkReply* reply);

private:
    int a = 100, b = 12331332;
    QString m_baseurl, m_lang;
    int m_timeout;
    QNetworkAccessManager* m_networkManager;
};

typedef PocketBase Client;

#endif // CLIENT_H
