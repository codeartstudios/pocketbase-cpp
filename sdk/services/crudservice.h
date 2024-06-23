#ifndef CRUDSERVICE_H
#define CRUDSERVICE_H

#include <QString>
#include <QJsonObject>
#include <QList>

#include "basecrudservice.h"
#include "../client.h"


class CrudService : public BaseCrudService {
    Q_OBJECT

public:
    explicit CrudService(PocketBase* client, QObject* parent = nullptr);

    QList<BaseModel*> getFullList(int batch = 200, const QJsonObject& queryParams = QJsonObject());
    ListResult getList(int page = 1, int perPage = 30, const QJsonObject& queryParams = QJsonObject());
    BaseModel* getFirstListItem(const QString& filter, const QJsonObject& queryParams = QJsonObject());
    BaseModel* getOne(const QString& id, const QJsonObject& queryParams = QJsonObject());
    BaseModel* create(const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    BaseModel* update(const QString& id, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool deleteOne(const QString& id, const QJsonObject& queryParams = QJsonObject());

protected:
    virtual QString baseCrudPath() const = 0;
};

#endif // CRUDSERVICE_H
