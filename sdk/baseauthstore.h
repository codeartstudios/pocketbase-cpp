#ifndef BASEAUTHSTORE_H
#define BASEAUTHSTORE_H

#include <QObject>
#include <QString>
#include <memory>

class RecordService;
class AdminService;

class BaseAuthStore : public QObject
{
    Q_OBJECT
public:
    explicit BaseAuthStore(const QString& token = "", std::shared_ptr<RecordService> baseModel = nullptr, QObject* parent = nullptr);

    QString token() const;
    std::shared_ptr<RecordService> model() const;

    void save(const QString& token = "", std::shared_ptr<RecordService> model = nullptr);
    void clear();

signals:

private:
    QString m_baseToken;
    std::shared_ptr<RecordService> m_baseModel;
};

#endif // BASEAUTHSTORE_H
