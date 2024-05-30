#ifndef CRUDSERVICE_H
#define CRUDSERVICE_H

#include <QString>
#include <QJsonObject>
#include <QList>
#include <memory>

#include "basecrudservice.h"

class BaseModel;
class ListResult;

class CrudService : public BaseCrudService {
    Q_OBJECT

public:
    explicit CrudService(std::shared_ptr<QPocketBase> client, QObject* parent = nullptr);

    QList<std::shared_ptr<BaseModel>> getFullList(int batch = 200, const QJsonObject& queryParams = QJsonObject());
    ListResult getList(int page = 1, int perPage = 30, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> getFirstListItem(const QString& filter, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> getOne(const QString& id, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> create(const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> update(const QString& id, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool deleteItem(const QString& id, const QJsonObject& queryParams = QJsonObject());

protected:
    virtual QString baseCrudPath() const = 0;
};

#endif // CRUDSERVICE_H
