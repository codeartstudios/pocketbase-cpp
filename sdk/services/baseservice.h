#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QObject>
#include <QJsonObject>

#include "../client.h"

class BaseService : public QObject {
    Q_OBJECT

public:
    explicit BaseService(QObject* parent = nullptr, PocketBase* client = {})
        : QObject(parent), client(client) {}

protected:
    PocketBase* client;
};

#endif // BASESERVICE_H
