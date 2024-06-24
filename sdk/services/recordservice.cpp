#include "recordservice.h"
#include "baseauthstore.h"
#include "../client.h"

RecordAuthResponse::RecordAuthResponse(const QString &token, RecordModel* record)
    : token(token), record(record) {}

bool RecordAuthResponse::isValid() const {
    return validateToken(token);
}

bool RecordAuthResponse::validateToken(const QString &token) const {
    auto parts = token.split(".");

    if (parts.size() != 3) {
        return false;
    }

    QByteArray payload = Utils::base64UrlDecode(parts[1]);
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(payload, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON payload:" << parseError.errorString();
        return false;
    }

    return jsonDoc["exp"].toDouble() > QDateTime::currentSecsSinceEpoch();
}

AuthProviderInfo::AuthProviderInfo(const QString &name, const QString &state, const QString &codeVerifier, const QString &codeChallenge, const QString &codeChallengeMethod, const QString &authUrl)
    : name(name), state(state), codeVerifier(codeVerifier), codeChallenge(codeChallenge),
    codeChallengeMethod(codeChallengeMethod), authUrl(authUrl) {}

AuthMethodsList::AuthMethodsList(bool usernamePassword, bool emailPassword, const QList<AuthProviderInfo> &authProviders)
    : usernamePassword(usernamePassword), emailPassword(emailPassword), authProviders(authProviders) {}

RecordService::RecordService(PocketBase* client, const QString &collectionIdOrName)
    : CrudService(client),
    m_collectionIdOrName(collectionIdOrName),
    client(client) {}

BaseModel* RecordService::decode(const QJsonObject &data) {
    // TODO
    return new RecordModel(data);
}

QString RecordService::baseCollectionPath() const {
    return "/api/collections/" + QUrl::toPercentEncoding(m_collectionIdOrName);
}

QString RecordService::baseCrudPath() const {
    return baseCollectionPath() + "/records";
}

// QString RecordService::getFileUrl(const Record &record, const QString &filename, const QUrlQuery &queryParams) const {
//     QString baseUrl = client->baseUrl();
//     if (baseUrl.endsWith("/")) {
//         baseUrl.chop(1);
//     }
//     QString result = QString("%1/api/files/%2/%3/%4")
//                          .arg(baseUrl, record.collectionId, record.getId(), filename);
//     if (!queryParams.isEmpty()) {
//         result += "?" + queryParams.toString();
//     }
//     return result;
// }

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

RecordModel RecordService::update(
    const QString &id,
    const QJsonObject &bodyParams,
    const QUrlQuery &queryParams) {
    auto item = CrudService::update(id, bodyParams, Utils::urlQueryToJson(queryParams));
    try {
        if (client->authStore()->model()->collectionId != nullptr && item->getId() == client->authStore()->model()->getId()) {
            // client->authStore()->save(client->authStore()->token(), item);
        }
    } catch (...) {
    }

    // TODO cast item to Record
    return RecordModel{QJsonObject{}};
}

bool RecordService::deleteRecord(
    const QString &id,
    const QUrlQuery &queryParams) {
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
    // qDebug() << "[RecordService] Auth response";

    // Ensure response is well formed before creating the RecordAuthResponse
    if( responseData.contains("data") &&
        responseData["data"].toObject().contains("record") &&
        responseData["data"].toObject().contains("token")) {
        auto data = responseData["data"].toObject();
        auto record = new RecordModel(data["record"].toObject());
        QString token = data["token"].toString();

        if (!token.isEmpty() && !record->isEmpty()) {
            client->authStore()->save(token, record);
            // qDebug() << "[RecordService] Token Saved!";
        }

        return RecordAuthResponse(token, record);
    }

    return RecordAuthResponse{"", nullptr};
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

RecordAuthResponse RecordService::authWithPassword(const QString &usernameOrEmail, const QString &password, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    // qDebug() << "[RecordService] Authenticate with password";

    QJsonObject body, params;
    body.insert("identity", usernameOrEmail);
    body.insert("password", password);

    params.insert("method", "POST");
    params.insert("body", body);

    // qDebug() << "[RecordService] Request parameters: " << params;

    QJsonObject httpResponse = client->send(baseCollectionPath() + "/auth-with-password", params);

    return authResponse(httpResponse);
}

RecordAuthResponse RecordService::authWithOAuth2(const QString &provider, const QString &code, const QString &codeVerifier, const QString &redirectUrl, const QJsonObject &createData, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    // QJsonObject params = bodyParams;
    // params.insert("provider", provider);
    // params.insert("code", code);
    // params.insert("codeVerifier", codeVerifier);
    // params.insert("redirectUrl", redirectUrl);
    // params.insert("createData", createData);
    // QJsonObject responseData = client->send(baseCollectionPath() + "/auth-with-oauth2", "POST", queryParams, params);
    return authResponse(QJsonObject());
}

RecordAuthResponse RecordService::authRefresh(const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject payload, headers;
    payload.insert("method", "POST");
    payload.insert("body", bodyParams);
    headers.insert("auth", true);
    payload.insert("headers", headers);

    QJsonObject responseData = client->send(baseCollectionPath() + "/auth-refresh", payload);
    return authResponse(responseData);
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
    QJsonObject payload, body;
    body["email"] = email;
    payload.insert("method", "POST");
    payload.insert("body", body);

    auto jsonResponse = client->send(baseCollectionPath() + "/request-password-reset", payload);
    // qDebug() << "Password Reset: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}

bool RecordService::requestVerification(const QString &email) {
    QJsonObject payload, headers, body;
    body["email"] = email;
    payload.insert("method", "POST");
    payload.insert("body", body);
    headers.insert("auth", true);
    payload.insert("headers", headers);

    auto jsonResponse = client->send(baseCollectionPath() + "/request-verification", payload);
    // qDebug() << "Verification Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}

bool RecordService::confirmPasswordReset(const QString &passwordResetToken, const QString &password, const QString &passwordConfirm, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject params = bodyParams;
    params.insert("token", passwordResetToken);
    params.insert("password", password);
    params.insert("passwordConfirm", passwordConfirm);
    // client->send(baseCollectionPath() + "/confirm-password-reset", "POST", queryParams, params);
    return true;
}

bool RecordService::confirmVerification(const QString &token) {
    QJsonObject payload, headers, body;
    body["token"] = token;
    payload.insert("method", "POST");
    payload.insert("body", body);
    headers.insert("auth", true);
    payload.insert("headers", headers);
    auto jsonResponse = client->send(baseCollectionPath() + "/confirm-verification", payload);
    qDebug() << "Confirm Verification Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}
