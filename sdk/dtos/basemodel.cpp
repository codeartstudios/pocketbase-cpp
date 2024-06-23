#include "BaseModel.h"

BaseModel::BaseModel(const QJsonObject& data, QObject* parent)
    : QObject(parent) {
    load(data);
}

QString BaseModel::toString() const {
    return QString("<%1: %2>").arg(this->metaObject()->className()).arg(id);
}

QString BaseModel::repr() const {
    return toString();
}

void BaseModel::load(const QJsonObject& data) {
    id = data["id"].toString("");
    created = toDateTime(data["created"].toString(""));
    updated = toDateTime(data["updated"].toString(""));
}

bool BaseModel::isNew() const {
    return id.isEmpty();
}

QString BaseModel::getId() const {
    return id;
}

QDateTime BaseModel::getCreated() const {
    return created;
}

QDateTime BaseModel::getUpdated() const {
    return updated;
}

QDateTime BaseModel::toDateTime(const QString& datetimeStr) const {
    return QDateTime::fromString(datetimeStr, Qt::ISODate);
}
