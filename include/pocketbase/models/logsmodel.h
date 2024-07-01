#ifndef LOGSMODEL_H
#define LOGSMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

namespace pb {
class LogsModel : public QObject {
    Q_OBJECT

public:
    explicit LogsModel(const QJsonObject& data = QJsonObject(), QObject* parent = nullptr)
        : QObject{parent}, m_data(data) {
        data.value("id").toString();
        m_id= data.value("id").toString();
        m_created= data.value("created").toString();
        m_updated= data.value("updated").toString();
        m_level= data.value("level").toInt();
        m_message= data.value("message").toString();
    }

    QJsonObject toJson() const {
        return m_data;
    }

private:
    QString m_id;
    QString m_created;
    QString m_updated;
    int m_level;
    QString m_message;
    QJsonObject m_data;
};
}

#endif // LOGSMODEL_H
