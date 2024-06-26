#ifndef BASECRUDSERVICE_H
#define BASECRUDSERVICE_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>

#include "baseservice.h"
#include "../dtos/basemodel.h"
#include "../dtos/listresult.h"
#include "../dtos/recordmodel.h"

class PocketBase;


class BaseCrudService : public BaseService {
    Q_OBJECT

public:
    explicit BaseCrudService(PocketBase* client, QObject* parent = nullptr);

protected:
    virtual BaseModel* decode(const QJsonObject& data) = 0;

    QList<BaseModel*> _getFullList(const QString& basePath, int batchSize = 100, const QJsonObject& queryParams = QJsonObject());
    ListResult _getList(const QString& basePath, int page = 1, int perPage = 30, bool skipTotal = false, const QJsonObject& queryParams = QJsonObject());
    BaseModel* _getOne(const QString& basePath, const QString& id, const QJsonObject& queryParams = QJsonObject());
    BaseModel* _getFirstListItem(const QString& basePath, const QString& filter, const QJsonObject& queryParams = QJsonObject());
    BaseModel* _create(const QString& basePath, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    BaseModel* _update(const QString& basePath, const QString& id, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool _delete(const QString& basePath, const QString& id, const QJsonObject& queryParams = QJsonObject());

private:
    QList<BaseModel*> requestFullList(QList<BaseModel*>& result, const QString& basePath, int page, int batchSize, const QJsonObject& queryParams);
    PocketBase* client;
};

#endif // BASECRUDSERVICE_H
