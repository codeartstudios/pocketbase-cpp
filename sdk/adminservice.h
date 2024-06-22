#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <QObject>
#include <QString>
#include <memory>
#include <QString>
#include <QJsonObject>

#include "qpocketbase.h"
#include "utils/crudservice.h"
#include "models/AdminModel.h"
#include "baseauthstore.h"
#include "recordservice.h"

class AdminAuthResponse {
public:
    AdminAuthResponse(const QString& token, std::shared_ptr<AdminModel> admin);

    QString getToken() const;
    std::shared_ptr<AdminModel> getAdmin() const;

private:
    QString token;
    std::shared_ptr<AdminModel> admin;
};

class AdminService : public CrudService {
    Q_OBJECT

public:
    explicit AdminService(std::shared_ptr<QPocketBase> client, QObject* parent = nullptr);

    std::shared_ptr<BaseModel> decode(const QJsonObject& data) override;
    QString baseCrudPath() const override;

    std::shared_ptr<BaseModel> update(const QString& id, const QJsonObject& bodyParams, const QJsonObject& queryParams = QJsonObject());
    bool deleteItem(const QString& id, const QJsonObject& queryParams = QJsonObject());

    AdminAuthResponse authResponse(const QJsonObject& responseData);
    AdminAuthResponse authWithPassword(const QString& email, const QString& password, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    AdminAuthResponse authRefresh(const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool requestPasswordReset(const QString& email, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
    bool confirmPasswordReset(const QString& passwordResetToken, const QString& password, const QString& passwordConfirm, const QJsonObject& bodyParams = QJsonObject(), const QJsonObject& queryParams = QJsonObject());
};


#endif // ADMINSERVICE_H
