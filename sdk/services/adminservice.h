#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "crudservice.h"

class PocketBase;
class AdminModel;

class AdminAuthResponse {
public:
    AdminAuthResponse(const QString& token, AdminModel* admin);

    QString getToken() const;
    AdminModel* getAdmin() const;

private:
    QString m_token;
    AdminModel* m_admin;
};

class AdminService : public CrudService {
    Q_OBJECT

public:
    explicit AdminService(PocketBase* client, QObject* parent = nullptr);

    BaseModel* decode(const QJsonObject& data) override;
    QString baseCrudPath() const override;

    BaseModel* update(const QString& id, const QJsonObject& params = QJsonObject());
    bool deleteItem(const QString& id);

    AdminAuthResponse authResponse(const QJsonObject& responseData);
    QJsonObject authWithPassword(const QString& email, const QString& password, const QJsonObject& params = QJsonObject());
    AdminAuthResponse authRefresh(const QJsonObject& params = QJsonObject());
    bool confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm, const QJsonObject& params = QJsonObject());

private:
    PocketBase* client;
};


#endif // ADMINSERVICE_H
