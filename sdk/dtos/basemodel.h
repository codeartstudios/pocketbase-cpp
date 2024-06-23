#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

class BaseModel : public QObject {
    Q_OBJECT

public:
    explicit BaseModel(const QJsonObject& data = QJsonObject(), QObject* parent = nullptr);

    BaseModel(const BaseModel &data) {
        id = data.getId();
        created = data.getCreated();
        updated = data.getUpdated();
    }


    virtual QString toString() const;
    virtual QString repr() const;
    virtual void load(const QJsonObject& data);
    virtual bool isNew() const;

    virtual QString getId() const;
    virtual QDateTime getCreated() const;
    virtual QDateTime getUpdated() const;

protected:
    QString id;
    QDateTime created;
    QDateTime updated;

    QDateTime toDateTime(const QString& datetimeStr) const;
};

#endif // BASEMODEL_H
