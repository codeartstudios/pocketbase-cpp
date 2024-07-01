#include <pocketbase/services/collectionservice.h>
#include <pocketbase/client.h>

namespace pb {
CollectionService::CollectionService(PocketBase* client, QObject *parent)
    : QObject{parent},
      CrudService(client, parent),
      client(client) {}

QString CollectionService::baseCrudPath() const { return "/api/collections"; }

bool CollectionService::import(const QJsonArray& collections, bool deleteMissing,
                               const QJsonObject& params) {
    QJsonObject payload = params, body;
    body["collections"] = collections;
    body["deleteMissing"] = deleteMissing;
    payload["method"] = "PUT";
    payload["body"] = body;

    qDebug() << "[Import] Response: " << client->send(QString("%1/import").arg(baseCrudPath()), payload);

    return true;
}

BaseModel *CollectionService::decode(const QJsonObject &data) { return new BaseModel(data);}

}
