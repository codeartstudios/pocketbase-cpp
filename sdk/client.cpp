#include "client.h"
#include "services/recordservice.h"
#include "services/baseauthstore.h"
#include "services/clientresponseerror.h"
#include "services/collectionservice.h"
#include "services/adminservice.h"
#include "services/healthservice.h"

PocketBase::PocketBase(const QString& baseUrl, const QString& lang, int timeout, QObject* parent)
    : QObject(parent),
    m_baseurl(baseUrl),
    m_lang(lang),
    m_timeout(timeout),
    m_networkManager( new QNetworkAccessManager(this) ),
    m_authStore( new BaseAuthStore("", nullptr, this) ),
    m_collectionService( new CollectionService(this, this) ),
    m_adminService( new AdminService(this, this) ),
    m_healthService( new HealthService(this, this) ) {}

AdminService *PocketBase::admins() const { return m_adminService; }

RecordService* PocketBase::collection(const QString& idOrName) {
    if (!m_recordServices.contains(idOrName)) {
        m_recordServices[idOrName] = new RecordService(this, idOrName);
    }

    return m_recordServices[idOrName];
}

CollectionService *PocketBase::collections() const { return m_collectionService; }

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

HealthService *PocketBase::health() const { return m_healthService; }

QJsonObject PocketBase::send(const QString& path, const QJsonObject params) {
    QUrl url = buildUrl(path);
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    // If there are query parameters, pass them into the URL
    if( params.contains("query") && !params.value("query").isNull() ) {
        QUrlQuery q;

        for( const auto& key : params.value("query").toObject().keys() ) {
            QString value = params.value("query").toObject().value(key).toString();
            q.addQueryItem(key, value);
        }

        url.setQuery(q);
    }

    request.setUrl(url);
    qDebug() << "\n[PocketBase] Making a ("
             << params.value("method").toString()
             << ") request to '" << request.url().toString() << "'";

    // If no auth disabled by user
    if( params.contains("headers") &&
        params.value("headers").toObject().contains("auth") &&
        !params.value("headers").toObject().value("auth").toBool()) {}
    // Explicitly pass the auth header unless specified to be ignored
    else {
        request.setRawHeader("Authorization", "Bearer " + m_authStore->token().toUtf8());
    }

    QNetworkReply* reply;
    QJsonDocument doc(params.value("body").toObject());

    if(params.value("method").toString() == "GET") {
        reply = m_networkManager->get(request);
    }

    else if(params.value("method").toString() == "POST") {
        reply = m_networkManager->post(request, doc.toJson());
    }

    else if(params.value("method").toString() == "PATCH" ) {
        reply = m_networkManager->sendCustomRequest(request, "PATCH", doc.toJson());
    }

    else if(params.value("method").toString() == "PUT") {
        reply = m_networkManager->put(request, doc.toJson());
    }

    else if(params.value("method").toString() == "DELETE") {
        reply = m_networkManager->deleteResource(request);
    }

    else {
        throw ClientResponseError("Unhandled Method", 404);
        qDebug() << "Unhandled: " << params.value("method").toString();
    }

    QEventLoop wait_loop;
    connect(reply, &QNetworkReply::finished, &wait_loop, &QEventLoop::quit);
    wait_loop.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QJsonDocument resJsonDoc = QJsonDocument::fromJson(reply->readAll());

    QJsonObject responseObject;
    responseObject.insert("statusCode", statusCode);
    responseObject.insert("data", resJsonDoc.object());

    if( reply->error() != QNetworkReply::NoError ) {
        responseObject.insert("error", reply->errorString());
    }

    if( statusCode >= 400 ) {
        QString msg = resJsonDoc.object()["message"].toString();
        throw ClientResponseError(msg, statusCode, request.url().toString());
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
