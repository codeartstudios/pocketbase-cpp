#include "eventloop.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

EventLoop::EventLoop(const QString& url, const QString& method, const QMap<QString, QString>& headers, const QByteArray& payload, const QString& encoding, QObject* parent)
    : QThread(parent), url(url), method(method), headers(headers), payload(payload), encoding(encoding), kill(false) {}

void EventLoop::addEventListener(const QString& event, std::function<void(const Event&)> callback) {
    listeners[event] = callback;
}

void EventLoop::removeEventListener(const QString& event) {
    listeners.remove(event);
}

void EventLoop::run() {
    while (!kill) {
        readStream();
    }
}

void EventLoop::readStream() {
    QNetworkRequest request(QUrl{url});
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply* reply = networkManager.sendCustomRequest(request, method.toUtf8(), payload);
    connect(reply, &QNetworkReply::readyRead, this, [this, reply]() {
        QByteArray data = reply->readAll();
        processEvent(data);
    });

    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void EventLoop::processEvent(const QByteArray& data) {
    Event event;
    QList<QByteArray> lines = data.split('\n');
    for (const QByteArray& line : lines) {
        if (line.startsWith("id:")) {
            event.setId(line.mid(3).trimmed());
        } else if (line.startsWith("event:")) {
            event.setEvent(line.mid(6).trimmed());
        } else if (line.startsWith("data:")) {
            event.setData(line.mid(5).trimmed());
        } else if (line.startsWith("retry:")) {
            event.setRetry(line.mid(6).trimmed().toInt());
        }
    }
    if (listeners.contains(event.getEvent())) {
        listeners[event.getEvent()](event);
    }
}
