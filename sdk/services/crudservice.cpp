#include "crudservice.h"
#include "../client.h"

CrudService::CrudService(PocketBase* client, QObject* parent)
    : BaseCrudService(parent) { Q_UNUSED(client) }

QList<BaseModel*> CrudService::getFullList(int batch, const QJsonObject& queryParams) {
    return _getFullList(baseCrudPath(), batch, queryParams);
}

ListResult CrudService::getList(int page, int perPage, const QJsonObject& queryParams) {
    return _getList(baseCrudPath(), page, perPage, queryParams);
}

BaseModel* CrudService::getFirstListItem(const QString& filter, const QJsonObject& queryParams) {
    return _getFirstListItem(baseCrudPath(), filter, queryParams);
}

BaseModel* CrudService::getOne(const QString& id, const QJsonObject& queryParams) {
    return _getOne(baseCrudPath(), id, queryParams);
}

BaseModel* CrudService::create(const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    return _create(baseCrudPath(), bodyParams, queryParams);
}

BaseModel* CrudService::update(const QString& id, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    return _update(baseCrudPath(), id, bodyParams, queryParams);
}

bool CrudService::deleteOne(const QString& id, const QJsonObject& queryParams) {
    return _delete(baseCrudPath(), id, queryParams);
}
