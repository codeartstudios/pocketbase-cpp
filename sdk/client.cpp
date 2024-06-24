#include "client.h"
#include "services/recordservice.h"
#include "services/baseauthstore.h"

PocketBase::PocketBase(const QString& baseUrl, const QString& lang, int timeout, QObject* parent)
    : QObject(parent),
    m_baseurl(baseUrl),
    m_lang(lang),
    m_timeout(timeout),
    m_networkManager(new QNetworkAccessManager(this)),
    m_authStore(new BaseAuthStore("", nullptr, this)) {}

RecordService* PocketBase::collection(const QString& idOrName) {
    if (!m_recordServices.contains(idOrName)) {
        m_recordServices[idOrName] = new RecordService(this, idOrName);
    }

    return m_recordServices[idOrName];
}

QString PocketBase::filter(const QString &expr, const QMap<QString, QVariant> &query) {
    if (query.isEmpty()) {
        return expr;
    }

    for( const auto &key : query.keys()) {
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

QUrl PocketBase::buildUrl(const QString& path) {
    QUrl url(m_baseurl);

    if (!url.path().endsWith("/")) {
        url.setPath(url.path() + "/");
    }

    if(path.startsWith("/"))
        url.setPath(url.path() + path.mid(1));
    else
        url.setPath(url.path() + path);

    return url;
}

BaseAuthStore *PocketBase::authStore() const { return m_authStore; }

QJsonObject PocketBase::send(const QString& path, const QJsonObject params) {
    QUrl url = buildUrl(path);
    qDebug() << "[PocketBase] Making a (" << params.value("method").toString() << ") request to '" << url.toString() << "'";


    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");

    // If there are query parameters, pass them into the URL
    if( params.contains("query") && !params.value("query").isNull() ) {
        // QUrlQuery q;
        /// TODO
    }

    if( params.contains("headers") ) {
        auto headers = params.value("headers").toObject();
        if( headers.contains("auth") && headers.value("auth").toBool() ) {
            request.setRawHeader("Authorization", "Bearer " + m_authStore->token().toUtf8());
        }
    }

    // QJsonDocument jsonDoc(reqConfig);
    // QByteArray jsonData = jsonDoc.toJson();

    QNetworkReply* reply;
    QJsonDocument doc(params.value("body").toObject());

    if(params.value("method").toString() == "GET") {
        reply = m_networkManager->get(request);
    } else if(params.value("method").toString() == "POST") {
        reply = m_networkManager->post(request, doc.toJson());
    } else {
        qDebug() << "Unhandled: " << params.value("method").toString();
        return QJsonObject();
    }

    QEventLoop wait_loop;
    connect(reply, &QNetworkReply::finished, &wait_loop, &QEventLoop::quit);
    wait_loop.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QJsonDocument resJsonDoc = QJsonDocument::fromJson(reply->readAll());
    // qDebug() << "res: " << resJsonDoc;

    QJsonObject responseObject;
    responseObject.insert("statusCode", statusCode);
    responseObject.insert("data", resJsonDoc.object());

    if( reply->error() != QNetworkReply::NoError ) {
        responseObject.insert("error", reply->errorString());
    }

    return responseObject;
}

// QUrl PocketBase::getFileUrl(const RecordService& record, const QString& filename, const QMap<QString, QString>& queryParams) {
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

QString PocketBase::getFileToken() {
    QJsonObject params;
    params.insert("method", "POST");

    auto reply = send("/api/files/token", params);

    // auto data = reply->readAll().data();

    //qDebug() << "Reply [File Token] " << data;

    // Handle reply and extract token

    return QString(); // Placeholder
}

QString PocketBase::baseUrl() const
{
    return this->m_baseurl;
}

void PocketBase::setBaseUrl(const QString &newBaseUrl)
{
    if (m_baseurl == newBaseUrl)
        return;
    m_baseurl = newBaseUrl;
    emit baseUrlChanged();

    qDebug() << m_baseurl;
}

QString PocketBase::lang() const
{
    return m_lang;
}

void PocketBase::setLang(const QString &newLang)
{
    if (m_lang == newLang)
        return;
    m_lang = newLang;
    emit langChanged();
}
