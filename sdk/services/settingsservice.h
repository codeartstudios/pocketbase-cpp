#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <QObject>
#include <QJsonObject>

class PocketBase;

class SettingsService : public QObject
{
    Q_OBJECT
public:
    explicit SettingsService(PocketBase* client, QObject *parent = nullptr);

    /// Fetch all available app settings.
    QJsonObject getAll( const QJsonObject& params = {} );

    /// Bulk updates app settings.
    QJsonObject update(const QJsonObject& params);

    /// Performs a S3 storage connection test.
    QJsonObject testS3(QString filesystem = "storage");

    /// Sends a test email.
    ///
    /// The possible `template` values are:
    /// - verification
    /// - password-reset
    /// - email-change
    bool testEmail(QString toEmail, QString emailTemplate="verification");

    /// Generates a new Apple OAuth2 client secret.
    QJsonObject generateAppleClientSecret( QString clientId, QString teamId, QString keyId,
                                          QString privateKey, int duration);

private:
    PocketBase* client;
};

#endif // SETTINGSSERVICE_H
