#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QObject>
#include <memory>
#include <QJsonObject>

class QPocketBase; // Forward declaration of QPocketBase class

class BaseService : public QObject {
    Q_OBJECT

public:
    explicit BaseService(QObject* parent = nullptr, std::shared_ptr<QPocketBase> client = {})
        : QObject(parent), client(client) {}

protected:
    std::shared_ptr<QPocketBase> client;
};

#endif // BASESERVICE_H
