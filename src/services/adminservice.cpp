#include "adminservice.h"
#include "baseauthstore.h"
#include "../client.h"
#include "../models/adminmodel.h"
#include "../models/basemodel.h"

namespace pb {
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
        qDebug() << "Update token ...";
        if (item->getId() == client->authStore()->model()->getId()) {
            qDebug() << "Updating record ...";
            client->authStore()->save(client->authStore()->token(), record);
        }
    } catch (...) {
        // Handle exceptions
    }

    return record;
}

bool AdminService::deleteOne(const QString& id) {
    auto ok = CrudService::deleteOne(id);

    try {
        if (ok && id == client->authStore()->model()->getId()) {
            client->authStore()->clear();
        }
    } catch (...) {
        qDebug() << "Exeception on Admin Service deleteOne";
    }

    return ok;
}

AdminAuthResponse AdminService::authResponse(const QJsonObject& responseData) {
    QJsonObject adminData = responseData.value("admin").toObject();
    QString token = responseData.value("token").toString();
    auto admin = new AdminModel(decode(adminData));

    if (!token.isEmpty() && admin) {
        client->authStore()->save(token, new RecordModel(admin));
    }

    return AdminAuthResponse(token, admin);
}

AdminAuthResponse AdminService::authWithPassword(const QString& email, const QString& password, const QJsonObject& params) {
    QJsonObject body, payload;
    body.insert("identity", email);
    body.insert("password", password);

    payload.insert("method", "POST");
    payload.insert("body", body);
    if( !params.isEmpty() ) payload.insert("query", params);

    QJsonObject httpResponse = client->send(baseCrudPath() + "/auth-with-password", payload);
    return authResponse(httpResponse.value("data").toObject());
}

AdminAuthResponse AdminService::authRefresh(const QJsonObject& params) {
    QJsonObject payload = params;
    payload.insert("method", "POST");
    QJsonObject responseData = client->send(baseCrudPath() + "/auth-refresh", payload);
    return authResponse(responseData);
}

bool AdminService::requestPasswordReset(const QString& email) {
    QJsonObject payload, body;
    body["email"] = email;
    payload.insert("method", "POST");
    payload.insert("body", body);

    auto jsonResponse = client->send(baseCrudPath() + "/request-password-reset", payload);
    qDebug() << "[Admin Password Reset] Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}

bool AdminService::confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm) {
    QJsonObject payload, headers, body;
    body["token"] = passwordResetToken;
    body["password"] = password;
    body["passwordConfirm"] = passwordConfirm;
    payload.insert("method", "POST");
    payload.insert("body", body);

    auto jsonResponse = client->send(baseCrudPath() + "/confirm-password-reset", payload);
    qDebug() << "[Admin Confirm Password Reset] Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}
}