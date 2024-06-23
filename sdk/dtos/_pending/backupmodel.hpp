#ifndef BACKUPMODEL_H
#define BACKUPMODEL_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

#include "utils/BaseModel.h"

class BackupModel : public BaseModel {
public:
    BackupModel(const QJsonObject& data = QJsonObject());

    QString getKey() const;
    void setKey(const QString& key);

    QDateTime getModified() const;
    void setModified(const QDateTime& modified);

    int getSize() const;
    void setSize(int size);

    void load(const QJsonObject& data);

private:
    QString key;
    QDateTime modified;
    int size;

    QDateTime toDateTime(const QString& datetimeStr) const;
};

BackupModel::BackupModel(const QJsonObject& data)
    : BaseModel(data), key(""), size(0) {
    load(data);
}

QString BackupModel::getKey() const {
    return key;
}

void BackupModel::setKey(const QString& key) {
    this->key = key;
}

QDateTime BackupModel::getModified() const {
    return modified;
}

void BackupModel::setModified(const QDateTime& modified) {
    this->modified = modified;
}

int BackupModel::getSize() const {
    return size;
}

void BackupModel::setSize(int size) {
    this->size = size;
}

void BackupModel::load(const QJsonObject& data) {
    BaseModel::load(data);
    key = data.value("key").toString();
    modified = toDateTime(data.value("modified").toString());
    size = data.value("size").toInt();
}

QDateTime BackupModel::toDateTime(const QString& datetimeStr) const {
    return QDateTime::fromString(datetimeStr, Qt::ISODate);
}

#endif // BACKUP_H
