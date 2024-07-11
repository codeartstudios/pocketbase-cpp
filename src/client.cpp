#include <pocketbase/client.h>
#include <pocketbase/services/recordservice.h>
#include <pocketbase/services/baseauthstore.h>
#include <pocketbase/services/clientresponseerror.h>
#include <pocketbase/services/collectionservice.h>
#include <pocketbase/services/adminservice.h>
#include <pocketbase/services/healthservice.h>
#include <pocketbase/services/settingsservice.h>
#include <pocketbase/services/logservice.h>
#include <pocketbase/services/realtimeservice.h>
#include <pocketbase/services/fileservice.h>

namespace pb {
PocketBase::PocketBase(const QString& baseUrl, const QString& lang, int timeout, QObject* parent)
    : QObject(parent),
    m_baseurl(baseUrl),
    m_lang(lang),
    m_timeout(timeout),
    m_networkManager( new QNetworkAccessManager(this) ),
    m_authStore( new BaseAuthStore("", nullptr, this) ),
    m_collectionService( new CollectionService(this, this) ),
    m_adminService( new AdminService(this, this) ),
    m_healthService( new HealthService(this, this) ),
    m_settingsService( new SettingsService(this, this)),
    m_logService( new LogService(this, this) ),
    m_realtimeService( new RealtimeService(this, this) ),
    m_fileService( new FileService(this, this) ) {
    connect(m_authStore, &BaseAuthStore::tokenChanged, this, [&](QString token){
        qDebug() << "Token Changed ...";
        emit tokenChanged(token);
    });

    if(qApp->applicationName() == "")
        qApp->setApplicationName("PocketBaseLib");
    if(qApp->organizationName() == "")
        qApp->setOrganizationName("PocketBaseLibCpp");

    qsettings = new QSettings(qApp->organizationName(), qApp->applicationName());
}

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

SettingsService *PocketBase::settings() const { return m_settingsService; }

LogService *PocketBase::logs() const { return m_logService; }

RealtimeService *PocketBase::realtime() const { return m_realtimeService; }

FileService *PocketBase::files() const { return m_fileService; }

QJsonObject PocketBase::send(const QString& path, const QJsonObject params) {
    QUrl url = buildUrl(path);
    QNetworkRequest request;

    // If there are query parameters, pass them into the URL
    if( params.contains("query") && !params.value("query").isNull() ) {
        QUrlQuery q1;
        // QStringList q;

        for( const auto& key : params.value("query").toObject().keys() ) {
            QString value = params.value("query").toObject().value(key).toString();
            // QString p = QString("%1=%2").arg(key, value);
            // q.append(QUrl::toPercentEncoding(p));
            q1.addQueryItem(key, value);
        }
        //auto newUrl = url.toString() + "?" + q.join("&");
        //qDebug() << "> New Url: " << newUrl;
        // url.setUrl(newUrl);
        url.setQuery(q1);
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

    auto bodyJson = params.value("body").toObject(QJsonObject());
    QStringList textKeys, fileKeys;

    // Extract the keys, into the text and file arrays
    for(const auto& key : bodyJson.keys()) {
        auto obj = bodyJson.value(key).toObject();
        if( obj.value("type").toString("") == "files" ) {
            fileKeys.append(key);
        } else {
            textKeys.append(key);
        }
    }

    QNetworkReply* reply;
    QJsonDocument doc(bodyJson);

    // If we have files to upload, lets handle it in the multipart
    if ( fileKeys.size() > 0 &&
        (params.value("method").toString() == "POST" ||
         params.value("method").toString() == "PUT" ||
         params.value("method").toString() == "PATCH" )) {
        QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        for(const auto& key : textKeys ) {
            QHttpPart jsonPart;
            jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant(QString("form-data; name=\"%1\"").arg(key)));

            auto ba = convertJsonValueToByteArray(bodyJson.value(key));
            jsonPart.setBody(ba);
            multiPart->append(jsonPart);
        }

        // Add files to multipart
        for ( const auto& key : fileKeys ) {
            auto obj = bodyJson.value(key).toObject();
            auto filesArray = obj.value("files").toArray();

            for( const auto& filePath : filesArray ) {
                QFile* file = new QFile(filePath.toString());
                QString fileName = QFileInfo(file->fileName()).fileName();

                if(!file->exists()) {
                    throw ClientResponseError("File not found", 0, file->fileName());
                }

                if (!file->open(QIODevice::ReadOnly)) {
                    // QString err = file->errorString();
                    throw ClientResponseError("Error opening attached file", 0, file->fileName());
                }

                QMimeDatabase mimeDatabase;
                QMimeType mimeType = mimeDatabase.mimeTypeForFile(file->fileName());
                QString mimeTypeName = mimeType.name(); // This holds the MIME type (e.g., "image/jpeg")

                QHttpPart filePart;
                filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeTypeName));
                filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                   QVariant(QString("form-data; name=\"%1\"; filename=\"%2\"").arg(key, fileName)));
                filePart.setBodyDevice(file);
                file->setParent(multiPart); // We set the parent to ensure file is deleted with multiPart
                multiPart->append(filePart);
            }
        }

        if (params.value("method").toString() == "POST") {
            reply = m_networkManager->post(request, multiPart);
        }

        else if (params.value("method").toString() == "PUT") {
            reply = m_networkManager->put(request, multiPart);
        }

        else {
            reply = m_networkManager->sendCustomRequest(request, "PATCH", multiPart);
        }

        multiPart->setParent(reply); // Ensure multiPart is deleted with reply
    }

    else {
        request.setRawHeader("Content-Type", "application/json");

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

    if( statusCode >= 400 || statusCode < 200 ) {
        qDebug() << resJsonDoc;
        QString msg = resJsonDoc.object()["message"].toString();
        throw ClientResponseError(msg, statusCode, request.url().toString());
    }

    return responseObject;
}

QString PocketBase::getFileToken() { return m_fileService->getToken(); }

QUrl PocketBase::getFileUrl(
    const RecordModel &record,
    const QString &filename,
    const QJsonObject &queryParams) {
    QString thumb = queryParams.value("thumb").toString("");
    QString token = queryParams.value("token").toString("");
    bool download = queryParams.value("download").toBool(false);

    QString url = m_fileService->getUrl(record, filename, thumb, token, download);
    return QUrl(url);
}

QUrl PocketBase::getFileUrl(
    const QString& collectionIdOrName,
    const QString& recordId,
    const QString &filename,
    const QJsonObject &queryParams) {
    QString thumb = queryParams.value("thumb").toString("");
    QString token = queryParams.value("token").toString("");
    bool download = queryParams.value("download").toBool(false);

    QString url = m_fileService->getUrl(collectionIdOrName, recordId, filename, thumb, token, download);
    return QUrl(url);
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

void PocketBase::setValue(QString key, QString category, QVariant value)
{
    qsettings->setValue(key+"/"+category,value.toString());
}

QVariant PocketBase::getValue(QString key, QString category)
{
    QVariant value = qsettings->value(key+"/"+category).toString();

    return value;
}

QByteArray PocketBase::convertJsonValueToByteArray(const QJsonValue &value) {
    QByteArray byteArray;

    switch (value.type()) {
    case QJsonValue::Bool:
        byteArray = value.toBool() ? "true" : "false";
        break;
    case QJsonValue::Double:
        byteArray = QByteArray::number(value.toDouble());
        break;
    case QJsonValue::String:
        byteArray = value.toString().toUtf8();
        break;
    case QJsonValue::Array: {
        QJsonDocument doc(value.toArray());
        byteArray = doc.toJson(QJsonDocument::Compact);
        break;
    }
    case QJsonValue::Object: {
        QJsonDocument doc(value.toObject());
        byteArray = doc.toJson(QJsonDocument::Compact);
        break;
    }
    case QJsonValue::Null:
    case QJsonValue::Undefined:
        byteArray = "null";
        break;
    }

    return byteArray;
}
}
