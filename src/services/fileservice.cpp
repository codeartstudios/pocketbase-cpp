#include <pocketbase/services/fileservice.h>
#include <pocketbase/client.h>
#include <pocketbase/models/recordmodel.h>

namespace pb {
FileService::FileService(PocketBase* client, QObject *parent)
    : QObject{parent}, client(client) {}

QString FileService::getUrl(const RecordModel &record,
                            const QString &filename,
                            const QString &thumb,
                            const QString &token,
                            bool download) {
    return getUrl(record.collectionId, record.getId(), filename, thumb, token, download);
}

QString FileService::getUrl(
    const QString &collectionIdOrName,
    const QString &recordId,
    const QString &filename,
    const QString &thumb,
    const QString &token,
    bool download) {
    if (filename.isEmpty() || recordId.isEmpty()) {
        return ""; // blank Url
    }

    QUrlQuery query;
    if( !thumb.isEmpty() ) query.addQueryItem("thumb", thumb);
    if( !token.isEmpty() ) query.addQueryItem("token", token);
    if( download ) query.addQueryItem("download", download ? "true" : "false");

    QString url = QString("/api/files/%1/%2/%3")
                      .arg(
                          QUrl::toPercentEncoding(collectionIdOrName),
                          QUrl::toPercentEncoding(recordId),
                          QUrl::toPercentEncoding(filename));

    if(query.isEmpty())
        return client->buildUrl(url).toString();

    return client->buildUrl(url).toString() + "?" + query.toString();
}

QString FileService::getToken() {
    QJsonObject payload;
    payload["method"] = "POST";

    auto jsonResponse = client->send("/api/files/token", payload);
    return jsonResponse["data"].toObject().value("token").toString();
}
}
