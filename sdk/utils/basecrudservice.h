#ifndef BASECRUDSERVICE_H
#define BASECRUDSERVICE_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include "baseservice.hpp"

class BaseModel;
class ListResult;


class BaseCrudService : public BaseService {
    Q_OBJECT

public:
    explicit BaseCrudService(QObject* parent = nullptr);

protected:
    virtual std::shared_ptr<BaseModel> decode(const QJsonObject& data) = 0;

    QList<std::shared_ptr<BaseModel>> _getFullList(const QString& basePath, int batchSize = 100, const QJsonObject& queryParams = QJsonObject());
    ListResult _getList(const QString& basePath, int page = 1, int perPage = 30, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> _getOne(const QString& basePath, const QString& id, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> _getFirstListItem(const QString& basePath, const QString& filter, const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> _create(const QString& basePath, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    std::shared_ptr<BaseModel> _update(const QString& basePath, const QString& id, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool _delete(const QString& basePath, const QString& id, const QJsonObject& queryParams = QJsonObject());

private:
    QList<std::shared_ptr<BaseModel>> requestFullList(QList<std::shared_ptr<BaseModel>>& result, const QString& basePath, int page, int batchSize, const QJsonObject& queryParams);
};

#endif // BASECRUDSERVICE_H
