#include "recordservice.h"

RecordAuthResponse::RecordAuthResponse(const QString &token, const RecordModel &record)
    : token(token), record(record) {}

bool RecordAuthResponse::isValid() const {
    return validateToken(token);
}

bool RecordAuthResponse::validateToken(const QString &token) const {
    // TODO
    // Implement token validation logic here
    return true;
}

AuthProviderInfo::AuthProviderInfo(const QString &name, const QString &state, const QString &codeVerifier, const QString &codeChallenge, const QString &codeChallengeMethod, const QString &authUrl)
    : name(name), state(state), codeVerifier(codeVerifier), codeChallenge(codeChallenge),
    codeChallengeMethod(codeChallengeMethod), authUrl(authUrl) {}

AuthMethodsList::AuthMethodsList(bool usernamePassword, bool emailPassword, const QList<AuthProviderInfo> &authProviders)
    : usernamePassword(usernamePassword), emailPassword(emailPassword), authProviders(authProviders) {}

RecordService::RecordService(QPocketBase* client, const QString &collectionIdOrName)
    : CrudService(client), collectionIdOrName(collectionIdOrName) {}

std::shared_ptr<BaseModel> RecordService::decode(const QJsonObject &data) {
    // TODO
    return std::make_shared<RecordModel>(data);
}

QString RecordService::baseCrudPath() const {
    return baseCollectionPath() + "/records";
}

QString RecordService::baseCollectionPath() const {
    return "/api/collections/" + QUrl::toPercentEncoding(collectionIdOrName);
}

QString RecordService::getFileUrl(const RecordModel &record, const QString &filename, const QUrlQuery &queryParams) const {
    QString baseUrl = client->baseUrl();
    if (baseUrl.endsWith("/")) {
        baseUrl.chop(1);
    }
    QString result = QString("%1/api/files/%2/%3/%4")
                         .arg(baseUrl, record.collectionId, record.getId(), filename);
    if (!queryParams.isEmpty()) {
        result += "?" + queryParams.toString();
    }
    return result;
}

void RecordService::subscribe(std::function<void (const QVariant &)> callback) {
    // client->realtime()->subscribe(collectionIdOrName, callback);
}

void RecordService::subscribeOne(const QString &recordId, std::function<void (const QVariant &)> callback) {
    // client->realtime()->subscribe(collectionIdOrName + "/" + recordId, callback);
}

void RecordService::unsubscribe(const QStringList &recordIds) {
    /*if (!recordIds.isEmpty()) {
        QStringList subs;
        for (const auto& id : recordIds) {
            subs.append(collectionIdOrName + "/" + id);
        }
        client->realtime()->unsubscribe(subs);
    } else {
        client->realtime()->unsubscribeByPrefix(collectionIdOrName);
    } */
}

RecordModel RecordService::update(const QString &id, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    auto item = CrudService::update(id, bodyParams, Utils::urlQueryToJson(queryParams));
    try {
        // if (client->authStore()->model().collectionId != nullptr && item.id() == client->authStore()->model().id()) {
        //     client->authStore()->save(client->authStore()->token(), item);
        // }
    } catch (...) {
    }

    // TODO cast item to RecordModel
    return RecordModel{QJsonObject{}};
}

bool RecordService::deleteRecord(const QString &id, const QUrlQuery &queryParams) {
    bool success = CrudService::deleteOne(id, Utils::urlQueryToJson(queryParams));
    try {
        // if (success && client->authStore()->model().collectionId() != nullptr && id == client->authStore()->model().id()) {
        //     client->authStore()->clear();
        // }
    } catch (...) {
    }
    return success;
}

RecordAuthResponse RecordService::authResponse(const QJsonObject &responseData) {
    qDebug() << "Auth response ...";
    // auto record = std::dynamic_pointer_cast<RecordModel>(decode(responseData["record"].toObject()));
    // QString token = responseData["token"].toString();
    // if (!token.isEmpty()) { // && !record.isEmpty()) {
    //     client->authStore()->save(token, record);
    // }
    // return RecordAuthResponse(token, record);
    return RecordAuthResponse{"", RecordModel{QJsonObject{}}};
}

AuthMethodsList RecordService::listAuthMethods(const QUrlQuery &queryParams) {
    // QJsonObject responseData = client->send(baseCollectionPath() + "/auth-methods", "GET", queryParams);
    // bool usernamePassword = responseData["usernamePassword"].toBool();
    // bool emailPassword = responseData["emailPassword"].toBool();
    // QList<AuthProviderInfo> authProviders;
    // for (const auto& provider : responseData["authProviders"].toArray()) {
    //     authProviders.append(AuthProviderInfo(
    //         provider["name"].toString(),
    //         provider["state"].toString(),
    //         provider["codeVerifier"].toString(),
    //         provider["codeChallenge"].toString(),
    //         provider["codeChallengeMethod"].toString(),
    //         provider["authUrl"].toString()));
    // }
    return AuthMethodsList(); // usernamePassword, emailPassword, authProviders);
}

QNetworkReply* RecordService::authWithPassword(const QString &usernameOrEmail, const QString &password, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    qDebug() << "Auth with Password begin";

    QJsonObject body, params;
    body.insert("identity", usernameOrEmail);
    body.insert("password", password);

    params.insert("method", "POST");
    params.insert("body", body);

    qDebug() << params;

    QEventLoop wait_loop;
    QNetworkReply* reply = client->send(baseCollectionPath() + "/auth-with-password", params);

    connect(reply, &QNetworkReply::finished, &wait_loop, [&](){ qDebug() << "Finished!";});
    connect(reply, &QNetworkReply::finished, &wait_loop, &QEventLoop::quit);
    wait_loop.exec();

    qDebug() << "Loop finished!";
    auto data = reply->readAll();
    qDebug() << "res: " << data;

    if( reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Password authentication error!" << reply->errorString();
    }


    // QJsonObject responseData =
    // return authResponse(QJsonObject());
    return nullptr;
}

QNetworkReply* RecordService::authWithOAuth2(const QString &provider, const QString &code, const QString &codeVerifier, const QString &redirectUrl, const QJsonObject &createData, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("provider", provider);
    params.insert("code", code);
    params.insert("codeVerifier", codeVerifier);
    params.insert("redirectUrl", redirectUrl);
    params.insert("createData", createData);
    // QJsonObject responseData = client->send(baseCollectionPath() + "/auth-with-oauth2", "POST", queryParams, params);
    // return authResponse(QJsonObject());
    return nullptr;
}

QNetworkReply* RecordService::authRefresh(const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    // QJsonObject responseData = client->send(baseCollectionPath() + "/auth-refresh", "POST", queryParams, bodyParams);
    // return authResponse(QJsonObject());
    return nullptr;
}

bool RecordService::requestEmailChange(const QString &newEmail, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("newEmail", newEmail);
    // client->send(baseCollectionPath() + "/request-email-change", "POST", queryParams, params);
    return true;
}

bool RecordService::confirmEmailChange(const QString &token, const QString &password, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", token);
    params.insert("password", password);
    // client->send(baseCollectionPath() + "/confirm-email-change", "POST", queryParams, params);
    return true;
}

bool RecordService::requestPasswordReset(const QString &email, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("email", email);
    // client->send(baseCollectionPath() + "/request-password-reset", "POST", queryParams, params);
    return true;
}

bool RecordService::requestVerification(const QString &email, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("email", email);
    // client->send(baseCollectionPath() + "/request-verification", "POST", queryParams, params);
    return true;
}

bool RecordService::confirmPasswordReset(const QString &passwordResetToken, const QString &password, const QString &passwordConfirm, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", passwordResetToken);
    params.insert("password", password);
    params.insert("passwordConfirm", passwordConfirm);
    // client->send(baseCollectionPath() + "/confirm-password-reset", "POST", queryParams, params);
    return true;
}

bool RecordService::confirmVerification(const QString &token, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", token);
    // client->send(baseCollectionPath() + "/confirm-verification", "POST", queryParams, params);
    return true;
}
