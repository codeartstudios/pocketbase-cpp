#ifndef EVENT_H
#define EVENT_H

#include <QString>

class Event {
public:
    Event(const QString& id = "",
          const QString& event = "message",
          const QString& data = "",
          int retry = -1);

    QString getId() const;
    QString getEvent() const;
    QString getData() const;
    int getRetry() const;

    void setId(const QString& id);
    void setEvent(const QString& event);
    void setData(const QString& data);
    void setRetry(int retry);

private:
    QString id;
    QString event;
    QString data;
    int retry;
};

#endif // EVENT_H
