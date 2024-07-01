#include "baseauthstore.h"
#include "../models/recordmodel.h"

namespace pb {
BaseAuthStore::BaseAuthStore(const QString& baseToken, RecordModel* baseModel, QObject* parent)
    : QObject(parent), m_baseToken(baseToken), m_baseModel(baseModel) {}

QString BaseAuthStore::token() const {
    return m_baseToken;
}

RecordModel* BaseAuthStore::model() const {
    return m_baseModel;
}

bool BaseAuthStore::isValid() const
{
    auto parts = m_baseToken.split(".");

    if (parts.size() != 3) {
        return false;
    }

    QByteArray payload = pb::base64UrlDecode(parts[1]);
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(payload, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON payload:" << parseError.errorString();
        return false;
    }

    return jsonDoc["exp"].toDouble() > QDateTime::currentSecsSinceEpoch();
}

void BaseAuthStore::save(const QString& token, RecordModel* model) {
    m_baseToken = token.isEmpty() ? "" : token;
    m_baseModel = model ? model : nullptr;
}

void BaseAuthStore::clear() {
    m_baseToken.clear();
    m_baseModel=nullptr;
}
}
