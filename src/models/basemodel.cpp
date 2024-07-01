#include "BaseModel.h"

namespace pb {
BaseModel::BaseModel(const QJsonObject& data, QObject* parent)
    : QObject(parent) {
    load(data);
}

QString BaseModel::toString() const {
    return QString("<%1: %2>").arg(this->metaObject()->className()).arg(m_id);
}

QString BaseModel::repr() const {
    return toString();
}

void BaseModel::load(const QJsonObject& data) {
    m_id = data["id"].toString("");
    m_created = toDateTime(data["created"].toString(""));
    m_updated = toDateTime(data["updated"].toString(""));
    m_data = data;
}

bool BaseModel::isNew() const {
    return m_id.isEmpty();
}

QString BaseModel::getId() const {
    return m_id;
}

QDateTime BaseModel::getCreated() const {
    return m_created;
}

QDateTime BaseModel::getUpdated() const {
    return m_updated;
}

QDateTime BaseModel::toDateTime(const QString &datetimeStr) {
    return QDateTime::fromString(datetimeStr, Qt::ISODate);
}
}