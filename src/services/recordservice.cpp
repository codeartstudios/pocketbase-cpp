#include "recordservice.h"
#include "baseauthstore.h"
#include "../client.h"
#include "realtimeservice.h"

namespace pb {
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

    QByteArray payload = pb::base64UrlDecode(parts[1]);
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

void RecordService::subscribe(std::function<void (const Event &)> callback) {
    client->realtime()->subscribe(m_collectionIdOrName, callback);
}

void RecordService::subscribeOne(const QString &recordId, std::function<void (const Event &)> callback) {
    client->realtime()->subscribe(m_collectionIdOrName + "/" + recordId, callback);
}

void RecordService::unsubscribe() {
    client->realtime()->unsubscribe( m_collectionIdOrName );
}

void RecordService::unsubscribe(const QString &id) {
    if ( !id.isEmpty() ) {
        QString topic = m_collectionIdOrName + "/" + id;
        client->realtime()->unsubscribe(topic);
    }
}

void RecordService::unsubscribe(const QStringList &recordIds) {
    if ( !recordIds.isEmpty() ) {
        for (const auto& id : recordIds) {
            client->realtime()->unsubscribe(m_collectionIdOrName + "/" + id);
        }
    }
}

RecordModel RecordService::update(
    const QString &id,
    const QJsonObject &bodyParams,
    const QJsonObject &queryParams) {
    auto item = CrudService::update(id, bodyParams, queryParams);

    // Create a record model out of a basemodel
    auto record = RecordModel(item);

    try {
        if (client->authStore()->model()->collectionId != nullptr && item->getId() == client->authStore()->model()->getId()) {
            client->authStore()->save(client->authStore()->token(), &record);
        }
    } catch (...) {
    }

    return record;
}

bool RecordService::deleteRecord(
    const QString &id) {
    bool success = CrudService::deleteOne(id);
    try {
        if (success && client->authStore()->model()->collectionId != nullptr && id == client->authStore()->model()->getId()) {
            client->authStore()->clear();
        }
    } catch (...) {
    }

    return success;
}

RecordAuthResponse RecordService::authResponse(const QJsonObject &responseData) {
    // Ensure response is well formed before creating the RecordAuthResponse
    if( responseData.contains("data") &&
        responseData["data"].toObject().contains("record") &&
        responseData["data"].toObject().contains("token")) {
        auto data = responseData["data"].toObject();
        auto record = new RecordModel(data["record"].toObject());
        QString token = data["token"].toString();

        if (!token.isEmpty() && !record->isEmpty()) {
            client->authStore()->save(token, record);
        }

        return RecordAuthResponse(token, record);
    }

    return RecordAuthResponse{"", nullptr};
}

AuthMethodsList RecordService::listAuthMethods(const QUrlQuery &queryParams) {
    QJsonObject query, payload;
    query.insert("query", queryParams.toString());
    payload.insert("method", "POST");
    payload.insert("query", query);

    QJsonObject responseData = client->send(baseCollectionPath() + "/auth-methods", payload);
    qDebug() << "[List Auth Methods] Response: " << responseData;

    QList<AuthProviderInfo> authProviders;

    if( responseData.value("statusCode").toInt() == 200 ) {
        responseData = responseData["data"].toObject();

        bool usernamePassword = responseData["usernamePassword"].toBool();
        bool emailPassword = responseData["emailPassword"].toBool();

        for ( const auto& provider : responseData["authProviders"].toArray() ) {
            authProviders.append(AuthProviderInfo(
                provider.toObject().value("name").toString(),
                provider.toObject().value("state").toString(),
                provider.toObject().value("codeVerifier").toString(),
                provider.toObject().value("codeChallenge").toString(),
                provider.toObject().value("codeChallengeMethod").toString(),
                provider.toObject().value("authUrl").toString()));
        }
        return AuthMethodsList(usernamePassword, emailPassword, authProviders);
    } else {
        return AuthMethodsList(false, false, authProviders);
    }
}

RecordAuthResponse RecordService::authWithPassword(const QString &usernameOrEmail, const QString &password, const QJsonObject &bodyParams, const QUrlQuery &queryParams) {
    QJsonObject body, params;
    body.insert("identity", usernameOrEmail);
    body.insert("password", password);

    params.insert("method", "POST");
    params.insert("body", body);

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
    payload.insert("headers", headers);

    QJsonObject responseData = client->send(baseCollectionPath() + "/auth-refresh", payload);
    return authResponse(responseData);
}

bool RecordService::requestEmailChange(const QString &newEmail) {
    QJsonObject payload, body, headers;
    body["newEmail"] = newEmail;
    payload.insert("method", "POST");
    payload.insert("body", body);
    headers.insert("auth", true);
    payload.insert("headers", headers);

    // 400 - "Failed to authenticate."
    // 401 -  "The request requires valid record authorization token to be set."
    // 403 - "The authorized record model is not allowed to perform this action.

    auto jsonResponse = client->send(baseCollectionPath() + "/request-email-change", payload);
    qDebug() << "[Request Email Change] Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
}

bool RecordService::confirmEmailChange(const QString &token, const QString &password) {
    QJsonObject payload, body, headers;
    body["token"] = token;
    body["password"] = password;
    payload.insert("method", "POST");
    payload.insert("body", body);

    // 204 - Null
    // 400 - "Failed to authenticate."

    auto jsonResponse = client->send(baseCollectionPath() + "/confirm-email-change", payload);
    qDebug() << "[Confirm Email Change] Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
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

bool RecordService::confirmPasswordReset(const QString &passwordResetToken, const QString &password, const QString &passwordConfirm) {
    QJsonObject payload, headers, body;
    body["token"] = passwordResetToken;
    body["password"] = password;
    body["passwordConfirm"] = passwordConfirm;
    payload.insert("method", "POST");
    payload.insert("body", body);

    auto jsonResponse = client->send(baseCollectionPath() + "/confirm-password-reset", payload);
    qDebug() << "[Confirm Password Reset] Response: " << jsonResponse;
    return jsonResponse.value("statusCode").toInt() == 204;
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
}
