#include "qpocketbase.h"

QPocketBase::QPocketBase(QString& baseUrl, const QString& lang, AuthStore* authStore, QObject *parent) :
    QObject{parent},
    m_baseUrl(baseUrl),
    m_lang(lang),
    m_authStore((authStore ? authStore : new AuthStore())),
    m_admins(new AdminService()),
    m_collections(new CollectionService()),
    m_files(new FileService()),
    m_realtime(new RealtimeService()),
    m_settings(new SettingsService()),
    m_logs(new LogService()),
    m_health(new HealthService()),
    m_backups(new BackupService())
{

}

RecordService *QPocketBase::collection(const QString &collectionIdOrName) {
    RecordService* service = m_recordServices.value( collectionIdOrName );

    if ( !m_recordServices.contains( collectionIdOrName )) {
        // create and cache the service
        service = new RecordService( collectionIdOrName, this );
        m_recordServices[collectionIdOrName] = service;
    }

    return service;
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

QUrl QPocketBase::buildUrl(QString path, QMap<QString, QVariant> queryParameters) {
    QString url = m_baseUrl + (m_baseUrl.endsWith("/") ? "" : "/");

    if ( path.isEmpty() ) {
        url += path.startsWith("/") ? path.right( path.size()-1 ) : path;
    }

    // QString query = _normalizeQueryParameters(queryParameters);

    // return Uri.parse(url).replace(
    //     queryParameters: query.isNotEmpty ? query : null,
    //     );

    return QUrl();
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
