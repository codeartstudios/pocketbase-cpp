#include "BackupService.h"
#include "qpocketbase.h"

namespace pb {
BackupService::BackupService(std::shared_ptr<QPocketBase> client)
    : BaseService(client) {}

std::shared_ptr<BackupModel> BackupService::decode(const QJsonObject& data) {
    return std::make_shared<BackupModel>(data);
}

QString BackupService::basePath() const {
    return "/api/backup";
}

void BackupService::create(const QString& name) {
    client->send(basePath(), {{"method", "POST"}, {"body", QJsonObject{{"name", name}}}});
}

QList<std::shared_ptr<BackupModel>> BackupService::getFullList(const QJsonObject& queryParams) {
    QJsonArray responseData = client->send(basePath(), {{"method", "GET"}, {"params", queryParams}})->toArray();
    QList<std::shared_ptr<BackupModel>> backup;
    for (const QJsonValue& item : responseData) {
        backup.append(decode(item.toObject()));
    }
    return backup;
}

QByteArray BackupService::download(const QString& key, const QString& fileToken) {
    QString token = fileToken.isEmpty() ? client->getFileToken() : fileToken;
    return client->sendRaw(basePath() + "/" + key, {{"method", "GET"}, {"params", QJsonObject{{"token", token}}}});
}

void BackupService::deleteBackup(const QString& key) {
    client->send(basePath() + "/" + key, {{"method", "DELETE"}});
}

void BackupService::restore(const QString& key) {
    client->send(basePath() + "/" + key + "/restore", {{"method", "POST"}});
}

void BackupService::upload(const FileUploadModel& fileUpload) {
    client->send(basePath() + "/upload", {{"method", "POST"}, {"body", QJsonObject{{"file", fileUpload}}}});
}
}