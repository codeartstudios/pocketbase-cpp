#include "adminservice.h"

AdminAuthResponse::AdminAuthResponse(const QString& token, std::shared_ptr<AdminModel> admin)
    : token(token), admin(admin) {}

QString AdminAuthResponse::getToken() const {
    return token;
}

std::shared_ptr<AdminModel> AdminAuthResponse::getAdmin() const {
    return admin;
}

AdminService::AdminService(QPocketBase* client, QObject* parent)
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
        // if (client->authStore()->model()->getCollectionId().isNull() &&
        //     item->getId() == client->authStore()->getModel()->getId()) {
        //     client->authStore()->save(client->authStore()->getToken(), item);
        // }
    } catch (...) {
        // Handle exceptions
    }
    return item;
}

bool AdminService::deleteItem(const QString& id, const QJsonObject& queryParams) {
    auto item = CrudService::deleteOne(id, queryParams);
    try {
        // if (client->authStore()->getModel()->getCollectionId().isNull() &&
        //     item->getId() == client->authStore()->getModel()->getId()) {
        //     client->authStore()->clear();
        // }
    } catch (...) {
        // Handle exceptions
    }
    return true;
}

AdminAuthResponse AdminService::authResponse(const QJsonObject& responseData) {
    // QJsonObject adminData = responseData.value("admin").toObject();
    // QString token = responseData.value("token").toString();
    // auto admin = decode(adminData);
    // if (!token.isEmpty() && admin) {
    //     client->authStore()->save(token, admin);
    // }
    // return AdminAuthResponse(token, admin);
    return AdminAuthResponse{"", nullptr};
}

QJsonObject AdminService::authWithPassword(const QString& email, const QString& password, const QJsonObject& bodyParams, const QJsonObject& queryParams) {
    QJsonObject params, body, auth;
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
