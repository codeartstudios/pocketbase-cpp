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
#include <QJsonDocument>
#include <QHttpMultiPart>
#include <QMap>
#include <QString>
#include <memory>

class BaseAuthStore;
class AdminService;
class BackupService;
class CollectionService;
class LogService;
class RealtimeService;
class RecordService;
class SettingsService;
class FileService;
class HealthService;


class QPocketBase : public QObject
{
    Q_OBJECT
public:
    QPocketBase(const QString& baseUrl = "/", const QString& lang = "en-US", std::shared_ptr<BaseAuthStore> authStore = nullptr, int timeout = 120, QObject* parent = nullptr);

    /// The PocketBase backend base url address (eg. 'http://127.0.0.1:8090').
    // Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)

    /// Optional language code (default to `en-US`) that will be sent
    /// with the requests to the server as `Accept-Language` header.
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged FINAL)

    /// Returns the RecordService associated to the specified collection.
    std::shared_ptr<RecordService> collection(const QString& idOrName);

    /// Constructs a filter expression with placeholders populated from a map.
    ///
    /// Placeholder parameters are defined with the `{:paramName}` notation.
    ///
    /// The following parameter values are supported:
    /// - `String` (_single quotes are autoescaped_)
    /// - `num`
    /// - `bool`
    /// - `DateTime`
    /// - `null`
    /// - everything else is converted to a string using `jsonEncode()`
    ///
    /// Example:
    ///
    /// ```dart
    /// pb.collection("example").getList(filter: pb.filter(
    ///   "title ~ {:title} && created >= {:created}",
    ///   { "title": "example", "created": DateTime.now() },
    /// ));
    /// ```
    QString filter(const QString& expr, const QMap<QString, QVariant>& query);

    /// Legacy alias of `pb.files.getUrl()`.
    // QString getFileUrl( RecordModel record, QString filename, QString thumb="", QString token="", QMap<QString, QVariant> query = {} ) {
    //     return m_files.getUrl( record, filename, thumb, token, query );
    // }

    /// Builds and returns a full request url by safely concatenating
    /// the provided path to the base url.
    QUrl buildUrl(QString path, QMap<QString, QVariant> queryParameters );

    QUrl getFileUrl(const RecordService& record, const QString& filename, const QMap<QString, QString>& queryParams);

    QString getFileToken();

    QString baseUrl() const;
    void setBaseUrl(const QString &newBaseUrl);

    QString lang() const;
    void setLang(const QString &newLang);

    std::shared_ptr<BaseAuthStore> authStore() { return m_authStore; }


    QNetworkReply* send(const QString& path, const QJsonObject& reqConfig);

signals:

    void baseUrlChanged();

    void langChanged();signals:

    void requestFinished(QNetworkReply* reply);

private:
    QUrl buildUrl(const QString& path);

    /// An instance of the local [AuthStore] service.
    std::shared_ptr<BaseAuthStore> m_authStore;

    /// An instance of the service that handles the **Admin APIs**.
    std::shared_ptr<AdminService> m_admins;

    /// An instance of the service that handles the **Collection APIs**.
    std::shared_ptr<CollectionService> m_collections;

    /// An instance of the service that handles the **File APIs**.
    std::shared_ptr<FileService> m_files;

    /// An instance of the service that handles the **Realtime APIs**.
    ///
    /// This service is usually used with custom realtime actions.
    /// For records realtime subscriptions you can use the subscribe/unsubscribe
    /// methods available in the `collection()` RecordService.
    std::shared_ptr<RealtimeService> m_realtime;

    /// An instance of the service that handles the **Settings APIs**.
    std::shared_ptr<SettingsService> m_settings;

    /// An instance of the service that handles the **Log APIs**.
    std::shared_ptr<LogService> m_logs;

    /// An instance of the service that handles the **Health APIs**.
    std::shared_ptr<HealthService> m_health;

    /// The service that handles the **Backup and restore APIs**.
    std::shared_ptr<BackupService> m_backups;

    /// Cache of all created RecordService instances.
    QMap<QString, RecordService*> m_recordServices;

    QMap<QString, std::shared_ptr<RecordService>> recordServices;

    QString m_baseUrl;
    QString m_lang;
    int m_timeout;


    std::shared_ptr<QNetworkAccessManager> m_networkManager;
};

#endif // QPOCKETBASE_H
