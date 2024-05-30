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

    QString toString() const;
    QString repr() const;
    void load(const QJsonObject& data);
    bool isNew() const;

    QString getId() const;
    QDateTime getCreated() const;
    QDateTime getUpdated() const;

private:
    QString id;
    QDateTime created;
    QDateTime updated;

    QDateTime toDateTime(const QString& datetimeStr) const;
};

#endif // BASEMODEL_H
