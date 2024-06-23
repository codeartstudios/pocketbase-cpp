#include "realtimeservice.h"

SSEClient::SSEClient(const QString& url, const QString& method, const QMap<QString, QString>& headers, const QByteArray& payload, const QString& encoding, QObject* parent)
    : QObject(parent) {
    loopThread = new EventLoop(url, method, headers, payload, encoding, this);
    connect(loopThread, &EventLoop::finished, loopThread, &QObject::deleteLater);
    loopThread->start();
}

void SSEClient::addEventListener(const QString& event, std::function<void(const Event&)> callback) {
    listeners[event] = callback;
    loopThread->addEventListener(event, callback);
}

void SSEClient::removeEventListener(const QString& event) {
    listeners.remove(event);
    loopThread->removeEventListener(event);
}

void SSEClient::close() {
    loopThread->kill = true;
    loopThread->wait();
}
