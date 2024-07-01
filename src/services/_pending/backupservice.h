#ifndef BACKUPSSERVICE_H
#define BACKUPSSERVICE_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QJsonObject>
#include <memory>

#include "utils/baseservice.hpp"
#include "models/fileuploadmodel.hpp"
#include "models/backupmodel.hpp"

namespace pb {
class BackupService : public BaseService {
public:
    explicit BackupService(std::shared_ptr<QPocketBase> client);

    std::shared_ptr<BackupModel> decode(const QJsonObject& data);
    QString basePath() const;

    void create(const QString& name);
    QList<std::shared_ptr<BackupModel>> getFullList(const QJsonObject& queryParams = QJsonObject());
    QByteArray download(const QString& key, const QString& fileToken = QString());
    void deleteBackup(const QString& key);
    void restore(const QString& key);
    void upload(const FileUploadModel& fileUpload);
};
}

#endif // BACKUPSSERVICE_H
