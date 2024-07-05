#include <pocketbase/services/baseauthstore.h>
#include <pocketbase/models/recordmodel.h>

namespace pb {
BaseAuthStore::BaseAuthStore(const QString& baseToken,
                             RecordModel* baseModel,
                             QObject* parent)
    : QObject(parent),
    m_baseModel(baseModel),
    m_token(baseToken) {}

RecordModel* BaseAuthStore::model() const {
    return m_baseModel;
}

bool BaseAuthStore::isValid() const
{
    auto parts = m_token.split(".");

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

void BaseAuthStore::save()
{
    // Save to QSETTINGS
    if(m_saveMode==SaveMode::NONE) return;

}

void BaseAuthStore::save(const QString& token, RecordModel* model) {
    setToken(token.isEmpty() ? "" : token);
    m_baseModel = model ? model : nullptr;

    save();
}

void BaseAuthStore::clear() {
    m_token.clear();
    m_baseModel=nullptr;
    setToken("");
    save();
}

QString BaseAuthStore::getCollectionId() const { return ""; }

QString BaseAuthStore::getCollectionName() const { return ""; }

QString BaseAuthStore::token() const { return m_token; }

void BaseAuthStore::setToken(const QString &newToken)
{
    m_token = newToken;
    emit tokenChanged(newToken);
    save();
}

BaseAuthStore::SaveMode BaseAuthStore::saveMode() const
{
    return m_saveMode;
}

void BaseAuthStore::setSaveMode(SaveMode newSaveMode)
{
    if (m_saveMode == newSaveMode)
        return;
    m_saveMode = newSaveMode;
    emit saveModeChanged();
}

}
