#include "CrudService.h"
#include "../qpocketbase.h"
#include "basecrudservice.h"
#include "../models/utils/listresult.h"

CrudService::CrudService(QPocketBase* client, QObject* parent)
    : BaseCrudService(parent) { Q_UNUSED(client) }

QList<std::shared_ptr<BaseModel>> CrudService::getFullList(int batch, const QJsonObject& queryParams) {
    return _getFullList(baseCrudPath(), batch, queryParams);
}

ListResult CrudService::getList(int page, int perPage, const QJsonObject& queryParams) {
    return _getList(baseCrudPath(), page, perPage, queryParams);
}

std::shared_ptr<BaseModel> CrudService::getFirstListItem(const QString& filter, const QJsonObject& queryParams) {
    return _getFirstListItem(baseCrudPath(), filter, queryParams);
}

std::shared_ptr<BaseModel> CrudService::getOne(const QString& id, const QJsonObject& queryParams) {
    return _getOne(baseCrudPath(), id, queryParams);
}

std::shared_ptr<BaseModel> CrudService::create(const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    return _create(baseCrudPath(), bodyParams, queryParams);
}

std::shared_ptr<BaseModel> CrudService::update(const QString& id, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    return _update(baseCrudPath(), id, bodyParams, queryParams);
}

bool CrudService::deleteOne(const QString& id, const QJsonObject& queryParams) {
    return _delete(baseCrudPath(), id, queryParams);
}
