#include <pocketbase/services/crudservice.h>
#include <pocketbase/client.h>

namespace pb {
CrudService::CrudService(PocketBase* client, QObject* parent)
    : BaseCrudService(client, parent) {}

QList<BaseModel*> CrudService::getFullList(int batch, const QJsonObject& queryParams) {
    return _getFullList(baseCrudPath(), batch, queryParams);
}

ListResult CrudService::getList(int page, int perPage, bool skipTotal, const QJsonObject& queryParams) {
    return _getList(baseCrudPath(), page, perPage, skipTotal, queryParams);
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

bool CrudService::deleteOne(const QString& id) {
    return _delete(baseCrudPath(), id);
}
}
