#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "baseservice.h"

namespace pb {
class PocketBase;
class LogsModel;
class ListResult;
class BaseService;
class BaseModel;

class LogService: public BaseService
{
    Q_OBJECT
public:
    explicit LogService(PocketBase* client, QObject *parent = nullptr);

    ListResult getList(int page = 1, int perPage = 30, const QJsonObject paramObj = QJsonObject());

    /// Returns a single log by its id.
    LogsModel getOne(QString id, const QJsonObject& params = QJsonObject());

    /// Returns request logs statistics.
    QJsonArray getStats(const QJsonObject& param);

    LogsModel* decode(const QJsonObject& data);

private:
    PocketBase* client;
};
}

#endif // LOGSERVICE_H
