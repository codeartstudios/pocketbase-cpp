#include "adminservice.h"
#include "AdminService.h"

AdminAuthResponse::AdminAuthResponse(const QString& token, std::shared_ptr<AdminModel> admin)
    : token(token), admin(admin) {}

QString AdminAuthResponse::getToken() const {
    return token;
}

std::shared_ptr<AdminModel> AdminAuthResponse::getAdmin() const {
    return admin;
}

AdminService::AdminService(std::shared_ptr<QPocketBase> client, QObject* parent)
    : CrudService(client, parent) {}

std::shared_ptr<BaseModel> AdminService::decode(const QJsonObject& data) {
    return std::make_shared<AdminModel>(data);
}

QString AdminService::baseCrudPath() const {
    return "/api/admins";
}

std::shared_ptr<BaseModel> AdminService::update(const QString& id, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    auto item = CrudService::update(id, bodyParams, queryParams);
    try {
        if (client->getAuthStore()->getModel()->getCollectionId().isNull() &&
            item->getId() == client->getAuthStore()->getModel()->getId()) {
            client->getAuthStore()->save(client->getAuthStore()->getToken(), item);
        }
    } catch (...) {
        // Handle exceptions
    }
    return item;
}

bool AdminService::deleteItem(const QString& id, const QJsonObject& queryParams) {
    auto item = CrudService::deleteItem(id, queryParams);
    try {
        if (client->getAuthStore()->getModel()->getCollectionId().isNull() &&
            item->getId() == client->getAuthStore()->getModel()->getId()) {
            client->getAuthStore()->clear();
        }
    } catch (...) {
        // Handle exceptions
    }
    return true;
}

AdminAuthResponse AdminService::authResponse(const QJsonObject& responseData) {
    QJsonObject adminData = responseData.value("admin").toObject();
    QString token = responseData.value("token").toString();
    auto admin = decode(adminData);
    if (!token.isEmpty() && admin) {
        client->getAuthStore()->save(token, admin);
    }
    return AdminAuthResponse(token, admin);
}

AdminAuthResponse AdminService::authWithPassword(const QString& email, const QString& password, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params = bodyParams;
    params.insert("identity", email);
    params.insert("password", password);
    QJsonObject responseData = client->send(baseCrudPath() + "/auth-with-password", { {"method", "POST"}, {"params", queryParams}, {"body", params}, {"headers", QJsonObject{{"Authorization", ""}}} });
    return authResponse(responseData);
}

AdminAuthResponse AdminService::authRefresh(const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject responseData = client->send(baseCrudPath() + "/auth-refresh", { {"method", "POST"}, {"params", queryParams}, {"body", bodyParams} });
    return authResponse(responseData);
}

bool AdminService::requestPasswordReset(const QString& email, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params = bodyParams;
    params.insert("email", email);
    client->send(baseCrudPath() + "/request-password-reset", { {"method", "POST"}, {"params", queryParams}, {"body", params} });
    return true;
}

bool AdminService::confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", passwordResetToken);
    params.insert("password", password);
    params.insert("passwordConfirm", passwordConfirm);
    client->send(baseCrudPath() + "/confirm-password-reset", { {"method", "POST"}, {"params", queryParams}, {"body", params} });
    return true;
}
