#include "qpocketbase.h"

QPocketBase::QPocketBase(const QString& baseUrl, const QString& lang, int timeout, QObject* parent)
    : QObject(parent),
    m_timeout(timeout),
    m_networkManager(new QNetworkAccessManager(this)),
    m_lang(lang),
    a(12),
    b(13)
{
    setBaseUrl(baseUrl);
    qDebug() << baseUrl;
}

// std::shared_ptr<RecordService> QPocketBase::collection(const QString& idOrName) {
//     qDebug() << "Collection init [" << idOrName << "]";
//     if (!recordServices.contains(idOrName)) {
//         qDebug() << "[RecordService] No record, creating new one ...";
//         recordServices[idOrName] = std::make_shared<RecordService>(this, idOrName);
//         qDebug() << "[RecordService] New instance created ...";
//     }

//     return recordServices[idOrName];
// }

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
    qDebug() << path;
    qDebug() << "--";
    // qDebug() << m_baseurl;
    qDebug() << "--";

    QUrl url("http://127.0.0.1:5740/"); // m_baseurl);
    if (!url.path().endsWith("/")) {
        url.setPath(url.path() + "/");
    }

    url.setPath(url.path() + path);
    return url;
}

QNetworkReply* QPocketBase::send(const QString& path, const QJsonObject params) {
    qDebug() << path;
    qDebug() << baseUrl();
    qDebug() << "Accessing the baseURL";
    setLang("something");
    qDebug() << "Lang set";
    a = 5;
    qDebug() << "Getting a";
    qDebug() << a;
    qDebug() << b;
    qDebug() << m_timeout;
    qDebug() << m_lang;
    qDebug() << QString(m_baseurl);

    QUrl url = buildUrl(path);

    // If there are query parameters, pass them into the URL
    if( params.contains("query") && !params.value("query").isNull() ) {
        // QUrlQuery q;
        /// TODO
    }


    QNetworkRequest request(url);

    // Add headers and other request configuration
    // if (m_authStore->token().isEmpty() && !request.hasRawHeader("Authorization")) {
    //     request.setRawHeader("Authorization", m_authStore->token().toUtf8());
    // }

    // QJsonDocument jsonDoc(reqConfig);
    // QByteArray jsonData = jsonDoc.toJson();

    QNetworkReply* reply;
    QJsonDocument doc(params.value("body").toObject());

    if(params.value("method").toString() == "GET") {
        reply = m_networkManager->get(request);
    } else if(params.value("method").toString() == "GET") {
        reply = m_networkManager->post(request, doc.toJson());
    } else {
        qDebug() << "Unhandled: " << params.value("method").toString();
    }

    return reply;
}

// QUrl QPocketBase::getFileUrl(const RecordService& record, const QString& filename, const QMap<QString, QString>& queryParams) {
//     // QStringList parts = { "api", "files", QUrl::toPercentEncoding(record.collectionId), QUrl::toPercentEncoding(record.getId()), QUrl::toPercentEncoding(filename) };
//     // QUrl url = buildUrl(parts.join("/"));
//     // if (!queryParams.isEmpty()) {
//     //     QUrlQuery query;
//     //     for (auto it = queryParams.begin(); it != queryParams.end(); ++it) {
//     //         query.addQueryItem(it.key(), it.value());
//     //     }
//     //     url.setQuery(query);
//     // }
//     return QUrl{};
// }

QString QPocketBase::getFileToken() {
    QJsonObject params;
    params.insert("method", "POST");

    QNetworkReply* reply = send("/api/files/token", params);

    auto data = reply->readAll().data();

    qDebug() << "Reply [File Token] " << data;

    // Handle reply and extract token

    return QString(); // Placeholder
}

QString QPocketBase::baseUrl() const
{
    return this->m_baseurl;
}

void QPocketBase::setBaseUrl(const QString &newBaseUrl)
{
    if (m_baseurl == newBaseUrl)
        return;
    m_baseurl = newBaseUrl;
    emit baseUrlChanged();

    qDebug() << m_baseurl;
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
