#include "adminservice.h"
#include "baseauthstore.h"
#include "../client.h"
#include "../dtos/adminmodel.h"
#include "../dtos/basemodel.h"

AdminAuthResponse::AdminAuthResponse(const QString& token, AdminModel* admin)
    : m_token(token), m_admin(admin) {}

QString AdminAuthResponse::getToken() const {
    return m_token;
}

AdminModel* AdminAuthResponse::getAdmin() const {
    return m_admin;
}

AdminService::AdminService(PocketBase* client, QObject* parent)
    : CrudService(client, parent), client(client) {}

BaseModel* AdminService::decode(const QJsonObject& data) {
    return new AdminModel(data);
}

QString AdminService::baseCrudPath() const {
    return "/api/admins";
}

BaseModel* AdminService::update(const QString& id, const QJsonObject& params) {
    auto item = CrudService::update(id, params);

    // Create a record model out of a basemodel
    auto record = new RecordModel(item);

    try {
        if (client->authStore()->model()->collectionId.isNull() &&
            item->getId() == client->authStore()->model()->getId()) {
            client->authStore()->save(client->authStore()->token(), record);
        }
    } catch (...) {
        // Handle exceptions
    }

    return record;
}

bool AdminService::deleteItem(const QString& id) {
    auto ok = CrudService::deleteOne(id);

    try {
        if (ok && client->authStore()->model()->collectionId.isNull() &&
            id == client->authStore()->model()->getId()) {
            client->authStore()->clear();
        }
    } catch (...) {
        // Handle exceptions
    }

    return ok;
}

AdminAuthResponse AdminService::authResponse(const QJsonObject& responseData) {
    QJsonObject adminData = responseData.value("admin").toObject();
    QString token = responseData.value("token").toString();
    auto admin = decode(adminData);
    if (!token.isEmpty() && admin) {
        // client->authStore()->save(token, admin);
    }
    return AdminAuthResponse(token, admin);
}

QJsonObject AdminService::authWithPassword(const QString& email, const QString& password, const QJsonObject& params) {
    QJsonObject payload, body, auth;
    body.insert("identity", email);
    body.insert("password", password);
    params.insert("method", "POST");
    params.insert("params", queryParams);
    params.insert("body", body);
    auth.insert("Authorization", "");
    params.insert("headers", auth);

    auto reply = client->send(baseCrudPath() + "/auth-with-password", params);

    connect(reply, &QNetworkReply::finished, this, [&](){
        // QJsonObject responseData
        qDebug() << "[AdminService] Reply finished" << reply->readAll();
    });
    // return authResponse(responseData);
    return QJsonObject();
}

AdminAuthResponse AdminService::authRefresh(const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    // QJsonObject responseData = client->send(baseCrudPath() + "/auth-refresh", { {"method", "POST"}, {"params", queryParams}, {"body", bodyParams} });
    return authResponse(QJsonObject{});
}

bool AdminService::requestPasswordReset(const QString& email, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params = bodyParams;
    params.insert("email", email);
    // client->send(baseCrudPath() + "/request-password-reset", { {"method", "POST"}, {"params", queryParams}, {"body", params} });
    return true;
}

bool AdminService::confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", passwordResetToken);
    params.insert("password", password);
    params.insert("passwordConfirm", passwordConfirm);
    // client->send(baseCrudPath() + "/confirm-password-reset", { {"method", "POST"}, {"params", queryParams}, {"body", params} });
    return true;
}
