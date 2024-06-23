#ifndef QPOCKETBASE_H
#define QPOCKETBASE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QUrl>
#include <QDebug>
#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QHttpMultiPart>
#include <QMap>
#include <QString>
#include <memory>

class QPocketBase : public QObject
{
    Q_OBJECT
public:
    QPocketBase(const QString& baseUrl = "/", const QString& lang = "en-US", int timeout = 120, QObject* parent = nullptr);

    /// The PocketBase backend base url address (eg. 'http://127.0.0.1:8090').
    // Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)

    /// Optional language code (default to `en-US`) that will be sent
    /// with the requests to the server as `Accept-Language` header.
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged FINAL)

    /// Returns the RecordService associated to the specified collection.
    // RecordService* collection(const QString& idOrName);

    QString filter(const QString& expr, const QMap<QString, QVariant>& query);

    /// Legacy alias of `pb.files.getUrl()`.
    // QString getFileUrl( RecordModel record, QString filename, QString thumb="", QString token="", QMap<QString, QVariant> query = {} ) {
    //     return m_files.getUrl( record, filename, thumb, token, query );
    // }

    /// Builds and returns a full request url by safely concatenating
    /// the provided path to the base url.
    // QUrl buildUrl(QString path, QMap<QString, QVariant> queryParameters );

    // QUrl getFileUrl(const RecordService& record, const QString& filename, const QMap<QString, QString>& queryParams);

    QString getFileToken();

    QString baseUrl() const;
    void setBaseUrl(const QString &newBaseUrl);

    QString lang() const;
    void setLang(const QString &newLang);

    // BaseAuthStore* authStore() { return m_authStore; }

    QNetworkReply* send(const QString& path, const QJsonObject params);

signals:
    void baseUrlChanged();

    void langChanged();

    void requestFinished(QNetworkReply* reply);

private:
    QUrl buildUrl(const QString& path);

    int a = 100, b = 12331332;
    QString m_baseurl, m_lang;
    int m_timeout;
    QNetworkAccessManager* m_networkManager;
};

#endif // QPOCKETBASE_H
