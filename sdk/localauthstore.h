#ifndef LOCALAUTHSTORE_H
#define LOCALAUTHSTORE_H

#include "BaseAuthStore.h"
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <memory>

class RecordService;
class AdminService;

class LocalAuthStore : public BaseAuthStore {
    Q_OBJECT

public:
    explicit LocalAuthStore(const QString& filename = "pocketbase_auth.data", const QString& filepath = "", const QString& baseToken = "", std::shared_ptr<RecordService> baseModel = nullptr, QObject* parent = nullptr);

    QString token() const;
    std::shared_ptr<RecordService> model() const;

    void save(const QString& token = "", std::shared_ptr<RecordService> model = nullptr);
    void clear();

private:
    QString filename;
    QString filepath;
    QString completeFilepath;

    void storageSet(const QString& key, const QVariantMap& value) const;
    QVariantMap storageGet(const QString& key) const;
    void storageRemove(const QString& key) const;
};

#endif // LOCALAUTHSTORE_H

