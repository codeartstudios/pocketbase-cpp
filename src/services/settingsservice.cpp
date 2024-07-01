#include <pocketbase/services/settingsservice.h>
#include <pocketbase/client.h>

namespace pb {
SettingsService::SettingsService(PocketBase* client, QObject *parent)
    : QObject{parent}, client(client) {}

QJsonObject SettingsService::getAll(const QJsonObject &params) {
    QJsonObject payload = params;
    payload["method"] = "GET";
    return client->send( "/api/settings", payload );
}

QJsonObject SettingsService::update(const QJsonObject &params) {
    QJsonObject payload = params;
    payload["method"] = "PATCH";
    return client->send("/api/settings", payload);
}

QJsonObject SettingsService::testS3(QString filesystem) {
    QJsonObject payload, body;
    payload["method"] = "POST";
    body["filesystem"] = filesystem;

    return client->send("/api/settings/test/s3", payload);
}

bool SettingsService::testEmail(QString toEmail, QString emailTemplate) {
    // verification, password-reset or email-change
    QJsonObject payload, body;
    body["email"] = toEmail;
    body["template"] = emailTemplate;
    payload["body"] = body;
    payload["method"] = "POST";

    auto responseJSON = client->send("/api/settings/test/email", payload);

    return responseJSON.value("statusCode").toInt() == 204;
}

QJsonObject SettingsService::generateAppleClientSecret(
    QString clientId, QString teamId, QString keyId, QString privateKey, int duration) {
    QJsonObject payload, body;
    body["clientId"] = clientId;
    body["teamId"] = teamId;
    body["keyId"] = keyId;
    body["privateKey"] = privateKey;
    body["duration"] = duration;
    payload["method"] = "POST";
    payload["body"] = body;

    return client->send("/api/settings/apple/generate-client-secret", payload);
}
}
