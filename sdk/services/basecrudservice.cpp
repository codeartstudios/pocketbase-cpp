#include "basecrudservice.h"
#include "../client.h"
#include "clientresponseerror.h"

BaseCrudService::BaseCrudService(PocketBase* client, QObject* parent)
    : BaseService(parent), client(client) {}

QList<BaseModel*> BaseCrudService::_getFullList(
    const QString& basePath,
    int batchSize,
    const QJsonObject& queryParams) {
    QList<BaseModel*> result;
    return requestFullList(result, basePath, 1, batchSize, queryParams);
}

ListResult BaseCrudService::_getList(
    const QString& basePath, int page,
    int perPage,
    bool skipTotal,
    const QJsonObject& queryParams) {
    QJsonObject params = queryParams;
    params["page"] = QString::number(page);
    params["perPage"] = QString::number(perPage);
    params["skipTotal"] = skipTotal ? "true":"false";

    QJsonObject payload;
    payload.insert("method", "GET");
    payload.insert("query", params);

    QJsonObject responseData = client->send(basePath, payload);
    QJsonObject data = responseData.value("data").toObject();
    QList<BaseModel*> items;

    if ( data.contains("items") ) {
        QJsonArray itemsArray = data["items"].toArray();
        for (const QJsonValue& item : itemsArray) {
            items.append(decode(item.toObject()));
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

BaseModel* BaseCrudService::_getOne(
    const QString& basePath,
    const QString& id,
    const QJsonObject& queryParams) {

    QJsonObject payload;
    payload.insert("method", "GET");
    payload.insert("query", queryParams);

    auto one = client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), payload);
    return new BaseModel(one.value("data").toObject());
}

BaseModel* BaseCrudService::_getFirstListItem(
    const QString& basePath,
    const QString& filter,
    const QJsonObject& queryParams) {
    QJsonObject payload, params = queryParams;
    params["filter"] = filter;
    params["$cancelKey"] = "one_by_filter_" + basePath + "_" + filter;

    ListResult result = _getList(basePath, 1, 1, false, params);

    if (result.items().isEmpty()) {
        throw ClientResponseError("The requested resource wasn't found.", 404);
    }

    return result.items().first();
}

BaseModel* BaseCrudService::_create(
    const QString& basePath,
    const QJsonObject& bodyParams,
    const QJsonObject& queryParams) {
    QJsonObject payload;
    payload.insert("method", "POST");
    if( !bodyParams.isEmpty() ) payload.insert("body", bodyParams);
    if( !queryParams.isEmpty() ) payload.insert("query", queryParams);

    auto responseJSON = client->send(basePath, payload);
    return decode(responseJSON["data"].toObject());
}

BaseModel* BaseCrudService::_update(
    const QString& basePath,
    const QString& id,
    const QJsonObject& bodyParams,
    const QJsonObject& queryParams) {
    QJsonObject payload;
    payload.insert("method", "PATCH");
    if( !bodyParams.isEmpty() ) payload.insert("body", bodyParams);
    if( !queryParams.isEmpty() ) payload.insert("query", queryParams);
    auto responseJSON = client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), payload);

    return decode(responseJSON.value("data").toObject());
}

bool BaseCrudService::_delete(
    const QString& basePath,
    const QString& id ) {
    QJsonObject payload;
    payload.insert("method", "DELETE");
    auto responseJSON = client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), payload);
    return responseJSON["statusCode"].toInt() == 204;
}

QList<BaseModel*> BaseCrudService::requestFullList(
    QList<BaseModel*>& result,
    const QString& basePath,
    int page, int batchSize,
    const QJsonObject& queryParams) {
    ListResult list = _getList(basePath, page, batchSize, !false, queryParams);
    result.append(list.items());

    if (!list.items().isEmpty() && list.totalItems() > result.size()) {
        return requestFullList(result, basePath, page + 1, batchSize, queryParams);
    }

    return result;
}
