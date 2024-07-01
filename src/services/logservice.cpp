#include <pocketbase/services/logservice.h>
#include <pocketbase/services/clientresponseerror.h>
#include <pocketbase/models/listresult.h>
#include <pocketbase/models/logsmodel.h>
#include <pocketbase/client.h>

namespace pb {
LogService::LogService(PocketBase* client, QObject *parent)
    : QObject(parent), BaseService(this, client), client(client) {}

LogsModel LogService::getOne(QString id, const QJsonObject &params) {
    if (id.isEmpty()) {
        QJsonObject error, data;
        error["url"] = client->buildUrl("/api/logs/").toString();
        error["statusCode"] = 404;
        data["code"] = 404;
        data["message"] = "Missing required log id.";
        data["data"] = "";
        error["response"] = data;

        throw ClientResponseError(error);
    }

    QJsonObject payload;
    payload["method"] = "GET";
    if( !payload.isEmpty() ) payload["query"] = params;

    auto jsonResponse = client->send(
        "/api/logs/" + QUrl::toPercentEncoding(id), payload);

    qDebug() << "[Get One] Response: " << jsonResponse;

    return LogsModel(jsonResponse.value("data").toObject());
}

QJsonArray LogService::getStats(const QJsonObject &param) {
    QJsonObject payload;
    payload["method"] = "GET";
    payload["query"] = param;
    auto statsResponse = client->send("/api/logs/stats", payload);
    qDebug() << "[Stats] Response: " << statsResponse;

    return statsResponse.value("data").toArray();
}

LogsModel* LogService::decode(const QJsonObject &data) {
    return new LogsModel(data);
}

/// Returns paginated logs list.
ListResult LogService::getList(int page, int perPage, const QJsonObject paramObj) {
    QJsonObject payload, params=paramObj;
    params["page"] = QString::number(page);
    params["perPage"] = QString::number(perPage);
    payload["method"] = "GET";
    payload["query"] = params;

    auto logsResponse = client->send("/api/logs", payload);
    QJsonObject data = logsResponse.value("data").toObject();
    QList<BaseModel*> items;

    if ( data.contains("items") ) {
        QJsonArray itemsArray = data["items"].toArray();
        for (const QJsonValue& item : itemsArray) {
            items.append(new BaseModel(item.toObject()));
        }
    }

    return ListResult(
        data["page"].toInt(),
        data["perPage"].toInt(),
        data["totalItems"].toInt(),
        data["totalPages"].toInt(),
        items
        );
}
}
