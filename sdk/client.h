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

class AdminService;
class RecordService;
class BaseAuthStore;
class CollectionService;
class HealthService;

class PocketBase : public QObject
{
    Q_OBJECT
public:
    PocketBase(const QString& baseUrl = "/", const QString& lang = "en-US", int timeout = 120, QObject* parent = nullptr);

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged FINAL)

    AdminService* admins() const;
    RecordService* collection(const QString& idOrName);
    CollectionService* collections() const;
    BaseAuthStore* authStore() const;
    HealthService* health() const;


    QJsonObject send(const QString& path, const QJsonObject params);

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
    QString m_baseurl, m_lang;
    int m_timeout;

    QNetworkAccessManager* m_networkManager;
    BaseAuthStore* m_authStore;
    CollectionService* m_collectionService;
    AdminService* m_adminService;
    HealthService* m_healthService;

    QMap<QString, RecordService*> m_recordServices;
};

typedef PocketBase Client;

#endif // CLIENT_H
