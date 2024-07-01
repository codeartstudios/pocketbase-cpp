#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

namespace pb {
class BaseModel : public QObject {
    Q_OBJECT

public:
    explicit BaseModel(const QJsonObject& data = QJsonObject(), QObject* parent = nullptr);

    BaseModel(const BaseModel &data) {
        m_id = data.getId();
        m_created = data.getCreated();
        m_updated = data.getUpdated();
        m_data = data.data();
    }


    virtual QString toString() const;
    virtual QString repr() const;
    virtual void load(const QJsonObject& data);
    virtual bool isNew() const;
    virtual QJsonObject data() const { return m_data; }

    virtual QString getId() const;
    virtual QDateTime getCreated() const;
    virtual QDateTime getUpdated() const;
    virtual QJsonObject toJSON() const { return m_data; };
    static QDateTime toDateTime(const QString& datetimeStr);

protected:
    QString m_id;
    QDateTime m_created;
    QDateTime m_updated;
    QJsonObject m_data;
};
}

#endif // BASEMODEL_H
