#include <pocketbase/services/healthservice.h>
#include <pocketbase/client.h>

namespace pb {
HealthService::HealthService(PocketBase* client, QObject *parent)
    : QObject{parent}, client(client) {}

QJsonObject HealthService::check(const QJsonObject &params) {
    QJsonObject payload=params;
    payload["method"] = "GET";
    return client->send("/api/health", payload);
}
}
