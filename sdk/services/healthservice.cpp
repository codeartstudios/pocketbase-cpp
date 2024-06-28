#include "healthservice.h"
#include "../client.h"

HealthService::HealthService(PocketBase* client, QObject *parent)
    : QObject{parent}, client(client)
{}

QJsonObject HealthService::check(const QJsonObject &params) {
    QJsonObject payload=params;
    payload["method"] = "GET";
    return client->send("/api/health", payload);
}
