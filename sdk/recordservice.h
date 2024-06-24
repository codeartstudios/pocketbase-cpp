#ifndef RECORDSERVICE_H
#define RECORDSERVICE_H

#include <QString>
#include <QUrlQuery>
#include <QVariant>
#include <QList>
#include <QPair>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

#include "crudservice.h"
#include "../dtos/recordmodel.h"
#include "../client.h"

// Forward declaration
class PocketBase;

class RecordAuthResponse {
public:
    QString token;
    RecordModel record;

    RecordAuthResponse(const QString& token, const RecordModel& record);

    bool isValid() const;

private:
    bool validateToken(const QString& token) const;
};

class AuthProviderInfo {
public:
    QString name;
    QString state;
    QString codeVerifier;
    QString codeChallenge;
    QString codeChallengeMethod;
    QString authUrl;

    AuthProviderInfo(const QString& name, const QString& state, const QString& codeVerifier,
                     const QString& codeChallenge, const QString& codeChallengeMethod,
                     const QString& authUrl);
};

class AuthMethodsList {
public:
    bool usernamePassword;
    bool emailPassword;
    QList<AuthProviderInfo> authProviders;

    // TEMPORARY
    AuthMethodsList(){}

    AuthMethodsList(bool usernamePassword, bool emailPassword, const QList<AuthProviderInfo>& authProviders);
};

class RecordService : public CrudService {
public:
    RecordService(PocketBase* client, const QString& collectionIdOrName);

    BaseModel* decode(const QJsonObject& data) override;

    QString baseCollectionPath() const;

    QString baseCrudPath() const override;

    QString getFileUrl(const RecordModel& record, const QString& filename, const QUrlQuery& queryParams = QUrlQuery()) const;

    void subscribe(std::function<void(const QVariant&)> callback);

    void subscribeOne(const QString& recordId, std::function<void(const QVariant&)> callback);

    void unsubscribe(const QStringList& recordIds);

    RecordModel update(const QString& id, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool deleteRecord(const QString& id, const QUrlQuery& queryParams = QUrlQuery());

    RecordAuthResponse authResponse(const QJsonObject& responseData);

    AuthMethodsList listAuthMethods(const QUrlQuery& queryParams = QUrlQuery());

    QNetworkReply* authWithPassword(const QString& usernameOrEmail, const QString& password, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    QNetworkReply* authWithOAuth2(const QString& provider, const QString& code, const QString& codeVerifier, const QString& redirectUrl, const QJsonObject& createData = QJsonObject(), const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    QNetworkReply* authRefresh(const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool requestEmailChange(const QString& newEmail, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool confirmEmailChange(const QString& token, const QString& password, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool requestPasswordReset(const QString& email, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool requestVerification(const QString& email, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

    bool confirmVerification(const QString& token, const QJsonObject& bodyParams = QJsonObject(), const QUrlQuery& queryParams = QUrlQuery());

private:
    QString m_collectionIdOrName;
    // PocketBase *client;
};

#endif // RECORDSERVICE_H
