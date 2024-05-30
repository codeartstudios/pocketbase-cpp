#include "LocalAuthStore.h"
#include "RecordService.h"
#include "AdminService.h"

#include <QVariant>
#include <QVariantMap>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

LocalAuthStore::LocalAuthStore(const QString& filename, const QString& filepath, const QString& baseToken, std::shared_ptr<RecordService> baseModel, QObject* parent)
    : BaseAuthStore(baseToken, baseModel, parent), filename(filename), filepath(filepath) {
    completeFilepath = QDir(filepath).filePath(filename);
}

QString LocalAuthStore::token() const {
    QVariantMap data = storageGet(completeFilepath);
    if (data.isEmpty() || !data.contains("token")) {
        return QString();
    }
    return data["token"].toString();
}

std::shared_ptr<RecordService> LocalAuthStore::model() const {
    QVariantMap data = storageGet(completeFilepath);
    if (data.isEmpty() || !data.contains("model")) {
        return nullptr;
    }
    // Assuming Record has a method to deserialize from QVariant
    return std::make_shared<RecordService>(data["model"].toMap());
}

void LocalAuthStore::save(const QString& token, std::shared_ptr<RecordService> model) {
    QVariantMap data;
    data["token"] = token;
    if (model) {
        // Assuming Record has a method to serialize to QVariant
        data["model"] = QVariant::fromValue(model->toVariantMap());
    }

    storageSet(completeFilepath, data);
    BaseAuthStore::save(token, model);
}

void LocalAuthStore::clear() {
    storageRemove(completeFilepath);
    BaseAuthStore::clear();
}

void LocalAuthStore::storageSet(const QString& key, const QVariantMap& value) const {
    QFile file(key);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << value;
        file.close();
    }
}

QVariantMap LocalAuthStore::storageGet(const QString& key) const {
    QVariantMap value;
    QFile file(key);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> value;
        file.close();
    }
    return value;
}

void LocalAuthStore::storageRemove(const QString& key) const {
    QFile file(key);
    if (file.exists()) {
        file.remove();
    }
}
