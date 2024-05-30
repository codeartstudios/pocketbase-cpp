#include "baseauthstore.h"
#include "RecordService.h"
#include "AdminService.h"

BaseAuthStore::BaseAuthStore(const QString& baseToken, std::shared_ptr<RecordService> baseModel, QObject* parent)
    : QObject(parent), m_baseToken(baseToken), m_baseModel(baseModel) {}

QString BaseAuthStore::token() const {
    return m_baseToken;
}

std::shared_ptr<RecordService> BaseAuthStore::model() const {
    return m_baseModel;
}

void BaseAuthStore::save(const QString& token, std::shared_ptr<RecordService> model) {
    m_baseToken = token.isEmpty() ? "" : token;
    m_baseModel = model ? model : nullptr;
}

void BaseAuthStore::clear() {
    m_baseToken.clear();
    m_baseModel.reset();
}
