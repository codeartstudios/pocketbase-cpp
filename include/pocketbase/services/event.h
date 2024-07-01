#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QJsonObject>

namespace pb {
class Event {
public:
    Event(const QString& id = "",
          const QString& event = "message",
          const QJsonObject& data = {},
          int retry = -1);

    QString id() const;
    QString event() const;
    QJsonObject data() const;
    int retry() const;

    void setId(const QString& id);
    void setEvent(const QString& event);
    void setData(const QJsonObject& data);
    void setRetry(int retry);

private:
    QString m_id;
    QString m_event;
    QJsonObject m_data;
    int m_retry;
};
}

#endif // EVENT_H
