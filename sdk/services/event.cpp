#include "event.h"

Event::Event(const QString& id,
             const QString& event,
             const QJsonObject& data,
             int retry)
    : m_id(id),
    m_event(event),
    m_data(data),
    m_retry(retry) {}

QString Event::id() const {
    return m_id;
}

QString Event::event() const {
    return m_event;
}

QJsonObject Event::data() const {
    return m_data;
}

int Event::retry() const {
    return m_retry;
}

void Event::setId(const QString& id) {
    m_id = id;
}

void Event::setEvent(const QString& event) {
    m_event = event;
}

void Event::setData(const QJsonObject& data) {
    m_data = data;
}

void Event::setRetry(int retry) {
    m_retry = retry;
}
