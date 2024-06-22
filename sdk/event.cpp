#include "event.h"

Event::Event(const QString& id, const QString& event, const QString& data, int retry)
    : id(id), event(event), data(data), retry(retry) {}

QString Event::getId() const {
    return id;
}

QString Event::getEvent() const {
    return event;
}

QString Event::getData() const {
    return data;
}

int Event::getRetry() const {
    return retry;
}

void Event::setId(const QString& id) {
    this->id = id;
}

void Event::setEvent(const QString& event) {
    this->event = event;
}

void Event::setData(const QString& data) {
    this->data = data;
}

void Event::setRetry(int retry) {
    this->retry = retry;
}
