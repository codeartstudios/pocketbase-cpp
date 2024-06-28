#ifndef HEALTHSERVICE_H
#define HEALTHSERVICE_H

#include <QObject>
#include <QJsonObject>

class PocketBase;

class HealthService : public QObject
{
    Q_OBJECT
public:
    explicit HealthService(PocketBase* client, QObject *parent = nullptr);

    /// Checks the health status of the api.
    QJsonObject check(const QJsonObject& params = {});

private:
    PocketBase* client;
};

#endif // HEALTHSERVICE_H
