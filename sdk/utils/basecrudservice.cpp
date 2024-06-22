#include "BaseCrudService.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>

#include "clientresponseerror.h"
#include "../models/utils/basemodel.h"
#include "../models/utils/listresult.h"
#include "../models/utils/schemafield.h"
#include "../qpocketbase.h"

BaseCrudService::BaseCrudService(QObject* parent)
    : BaseService(parent) {}

QList<std::shared_ptr<BaseModel>> BaseCrudService::_getFullList(const QString& basePath, int batchSize, const QJsonObject& queryParams) {
    QList<std::shared_ptr<BaseModel>> result;
    return requestFullList(result, basePath, 1, batchSize, queryParams);
}

ListResult BaseCrudService::_getList(const QString& basePath, int page, int perPage, const QJsonObject& queryParams) {
    QJsonObject params = queryParams;
    params["page"] = page;
    params["perPage"] = perPage;

    QJsonObject response_data; // = client->send(basePath, { {"method", "GET"}, {"params", params} }).toObject();
    QList<std::shared_ptr<BaseModel>> items;
    if (response_data.contains("items")) {
        QJsonArray itemsArray = response_data["items"].toArray();
        for (const QJsonValue& item : itemsArray) {
            items.append(decode(item.toObject()));
        }
    }
    return ListResult(
        response_data["page"].toInt(),
        response_data["perPage"].toInt(),
        response_data["totalItems"].toInt(),
        response_data["totalPages"].toInt(),
        items
        );
}

std::shared_ptr<BaseModel> BaseCrudService::_getOne(const QString& basePath, const QString& id, const QJsonObject& queryParams) {
    // return decode(client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), { {"method", "GET"}, {"params", queryParams} }).toObject());
    return std::shared_ptr<BaseModel>();
}

std::shared_ptr<BaseModel> BaseCrudService::_getFirstListItem(const QString& basePath, const QString& filter, const QJsonObject& queryParams) {
    QJsonObject params = queryParams;
    params["filter"] = filter;
    params["$cancelKey"] = "one_by_filter_" + basePath + "_" + filter;

    // ListResult result = getList(basePath, 1, 1, params);
    // if (result.items().isEmpty()) {
    //     throw ClientResponseError("The requested resource wasn't found.", 404);
    // }
    // return result.items().first();
    return std::shared_ptr<BaseModel>();
}

std::shared_ptr<BaseModel> BaseCrudService::_create(const QString& basePath, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    // return decode(client->send(basePath, { {"method", "POST"}, {"params", queryParams}, {"body", bodyParams} }).toObject());
    return std::shared_ptr<BaseModel>();
}

std::shared_ptr<BaseModel> BaseCrudService::_update(const QString& basePath, const QString& id, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    // return decode(client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), { {"method", "PATCH"}, {"params", queryParams}, {"body", bodyParams} }).toObject());
    return std::shared_ptr<BaseModel>();
}

bool BaseCrudService::_delete(const QString& basePath, const QString& id, const QJsonObject& queryParams) {
    client->send(QString("%1/%2").arg(basePath, QUrl::toPercentEncoding(id)), { {"method", "DELETE"}, {"params", queryParams} });
    return true;
}

QList<std::shared_ptr<BaseModel>> BaseCrudService::requestFullList(QList<std::shared_ptr<BaseModel>>& result, const QString& basePath, int page, int batchSize, const QJsonObject& queryParams) {
    ListResult list = _getList(basePath, page, batchSize, queryParams);
    result.append(list.items());
    if (!list.items().isEmpty() && list.totalItems() > result.size()) {
        return requestFullList(result, basePath, page + 1, batchSize, queryParams);
    }
    return result;
}
