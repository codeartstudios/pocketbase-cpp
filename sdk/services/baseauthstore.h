#ifndef BASEAUTHSTORE_H
#define BASEAUTHSTORE_H

#include <QObject>
#include <QString>
#include <QJsonDocument>

class RecordModel;

class BaseAuthStore : public QObject
{
    Q_OBJECT
public:
    explicit BaseAuthStore(const QString& token = "", RecordModel* baseModel = nullptr, QObject* parent = nullptr);

    QString token() const;
    RecordModel* model() const;
    bool isValid() const;

    void save(const QString& token = "", RecordModel* model = nullptr);
    void clear();

    // TODO
    QString getCollectionId() const { return ""; }
    QString getCollectionName() const { return ""; }

signals:

private:
    QString m_baseToken;
    RecordModel* m_baseModel;
};

#endif // BASEAUTHSTORE_H
