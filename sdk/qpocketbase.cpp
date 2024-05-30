#include "qpocketbase.h"
#include "BaseAuthStore.h"
#include "AdminService.h"
#include "BackupService.h"
#include "CollectionService.h"
#include "LogService.h"
#include "RealtimeService.h"
#include "RecordService.h"
#include "SettingsService.h"
#include "RecordService.h"

#include <QUrlQuery>
#include <QNetworkRequest>


QPocketBase::QPocketBase(const QString& baseUrl, const QString& lang, std::shared_ptr<BaseAuthStore> authStore, int timeout, QObject* parent)
    : QObject(parent),
    m_timeout(timeout),
    m_networkManager(new QNetworkAccessManager(this)),
    m_baseUrl(baseUrl),
    m_lang(lang),
    m_authStore(authStore ? authStore : std::make_shared<BaseAuthStore>()),
    m_admins(std::make_shared<AdminService>(this)),
    m_collections(std::make_shared<CollectionService>(this)),
    m_files(std::make_shared<FileService>(this)),
    m_realtime(std::make_shared<RealtimeService>(this)),
    m_settings(std::make_shared<SettingsService>(this)),
    m_logs(std::make_shared<LogService>(this)),
    m_health(std::make_shared<HealthService>(this)),
    m_backups(std::make_shared<BackupService>(this))
{
    // Initialize services
    // this->admins = std::make_shared<AdminService>(this);
}

std::shared_ptr<RecordService> QPocketBase::collection(const QString& idOrName) {
    if (!recordServices.contains(idOrName)) {
        recordServices[idOrName] = std::make_shared<RecordService>(this, idOrName);
    }
    return recordServices[idOrName];
}

QString QPocketBase::filter(const QString &expr, const QMap<QString, QVariant> &query) {
    if (query.isEmpty()) {
        return expr;
    }

    for( const auto key : query.keys()) {
        auto value = query.value( key );

        // if ( value.isNull() || value is num || value is bool) {
        //     value = value.toString();
        // } else if (value is DateTime) {
        //     value = "'${value.toUtc().toIso8601String().replaceFirst("T", " ")}'";
        // } else if (value is String) {
        //     value = "'${value.replaceAll("'", "\\'")}'";
        // } else {
        //     value = "'${jsonEncode(value).replaceAll("'", "\\'")}'";
        // }
        // expr = expr.replaceAll("{:$key}", value.toString());
    }

    return expr;
}

QUrl QPocketBase::buildUrl(const QString& path) {
    QUrl url(m_baseUrl);
    if (!url.path().endsWith("/")) {
        url.setPath(url.path() + "/");
    }

    url.setPath(url.path() + path);
    return url;
}

QNetworkReply* QPocketBase::send(const QString& path, const QJsonObject& reqConfig) {
    QUrl url = buildUrl(path);
    QNetworkRequest request(url);

    // Add headers and other request configuration
    if (m_authStore->token().isEmpty() && !request.hasRawHeader("Authorization")) {
        request.setRawHeader("Authorization", m_authStore->token().toUtf8());
    }

    QJsonDocument jsonDoc(reqConfig);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkReply* reply = m_networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        emit requestFinished(reply);
    });

    return reply;
}

QUrl QPocketBase::getFileUrl(const RecordService& record, const QString& filename, const QMap<QString, QString>& queryParams) {
    QStringList parts = { "api", "files", QUrl::toPercentEncoding(record.collectionId()), QUrl::toPercentEncoding(record.id()), QUrl::toPercentEncoding(filename) };
    QUrl url = buildUrl(parts.join("/"));
    if (!queryParams.isEmpty()) {
        QUrlQuery query;
        for (auto it = queryParams.begin(); it != queryParams.end(); ++it) {
            query.addQueryItem(it.key(), it.value());
        }
        url.setQuery(query);
    }
    return url;
}

QString QPocketBase::getFileToken() {
    QJsonObject reqConfig;
    reqConfig["method"] = "POST";
    QNetworkReply* reply = sendRequest("/api/files/token", reqConfig);

    auto data = reply->readAll().data();

    qDebug() << "Reply [File Token] " << data;

    // Handle reply and extract token

    return QString(); // Placeholder
}

QString QPocketBase::baseUrl() const
{
    return m_baseUrl;
}

void QPocketBase::setBaseUrl(const QString &newBaseUrl)
{
    if (m_baseUrl == newBaseUrl)
        return;
    m_baseUrl = newBaseUrl;
    emit baseUrlChanged();
}

QString QPocketBase::lang() const
{
    return m_lang;
}

void QPocketBase::setLang(const QString &newLang)
{
    if (m_lang == newLang)
        return;
    m_lang = newLang;
    emit langChanged();
}
