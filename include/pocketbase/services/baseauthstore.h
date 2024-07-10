#ifndef BASEAUTHSTORE_H
#define BASEAUTHSTORE_H

#include <QObject>
#include <QString>
#include <QJsonDocument>

namespace pb {
class RecordModel;

class BaseAuthStore : public QObject
{
    Q_OBJECT

public:
    enum SaveMode {
        NONE = 0,
        QSETTINGS
    };
    Q_ENUM(SaveMode)

    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged FINAL)
    Q_PROPERTY(SaveMode saveMode READ saveMode WRITE setSaveMode NOTIFY saveModeChanged FINAL)

    explicit BaseAuthStore(const QString& token = "",
                           RecordModel* baseModel = nullptr,
                           QObject* parent = nullptr);

    RecordModel* model() const;
    bool isValid() const;

    void save();
    void save(const QString& token,
              RecordModel* model = nullptr);

    void clear();

    // TODO
    QString getCollectionId() const;
    QString getCollectionName() const;

    QString token() const;
    void setToken(const QString &newToken);

    SaveMode saveMode() const;
    void setSaveMode(SaveMode newSaveMode);

signals:
    void tokenChanged(const QString &token);

    void saveModeChanged();

private:
    RecordModel* m_baseModel;
    QString m_token;
    SaveMode m_saveMode;
};
}

#endif // BASEAUTHSTORE_H
