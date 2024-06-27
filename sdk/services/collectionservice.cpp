#include "collectionservice.h"
#include "../client.h"

CollectionService::CollectionService(PocketBase* client, QObject *parent)
    : QObject{parent},
      CrudService(client, parent),
      client(client) {}

QString CollectionService::baseCrudPath() const {
    return "/api/collections";
}

bool CollectionService::import(QJsonArray& collections, bool deleteMissing, QJsonObject headers, QJsonObject query) {
    QJsonObject payload, body;
    body["collections"] = collections;
    body["deleteMissing"] = deleteMissing;
    payload["method"] = "PUT";
    payload["body"] = body;
    payload["headers"] = headers;
    payload["query"] = query;

    qDebug() << "[Import] Response: " << client->send(QString("%1/import").arg(baseCrudPath()), payload);

    return true;
}

BaseModel *CollectionService::decode(const QJsonObject &data)
{ return new BaseModel(QJsonObject());} // We are not using it
