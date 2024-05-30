#include "SSEClient.h"

Event::Event(const QString& id, const QString& event, const QString& data, int retry)
    : id(id), event(event), data(data), retry(retry) {}

QString Event::getId() const { return id; }

QString Event::getEvent() const { return event; }

QString Event::getData() const { return data; }

int Event::getRetry() const { return retry; }

EventLoop::EventLoop(const QString& url, const QString& method, const QMap<QString, QString>& headers, const QByteArray& payload, const QString& encoding, QObject* parent)
    : QThread(parent),
    url(url),
    method(method),
    headers(headers),
    payload(payload),
    encoding(encoding),
    kill(false)
{
    networkManager = new QNetworkAccessManager(this);
}

void EventLoop::addListener(const QString& event, std::function<void(Event)> listener) {
    listeners[event] = listener;
}

void EventLoop::removeListener(const QString& event) {
    listeners.remove(event);
}

void EventLoop::run() {
    QNetworkRequest request(QUrl{url});
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    if (method.toUpper() == "POST") {
        reply = networkManager->post(request, payload);
    } else {
        reply = networkManager->get(request);
    }

    connect(reply, &QNetworkReply::readyRead, this, &EventLoop::onReadyRead);

    exec();
}

void EventLoop::stop() {
    kill = true;
    if (reply) {
        reply->abort();
    }
    quit();
    wait();
}

void EventLoop::onReadyRead() {
    if (kill) return;
    readStream();
}

void EventLoop::readStream() {
    QByteArray data = reply->readAll();
    processEvent(data);
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

SSEClient::SSEClient(const QString& url, const QString& method, const QMap<QString, QString>& headers, const QByteArray& payload, const QString& encoding, QObject* parent)
    : QObject(parent) {
    loopThread = new EventLoop(url, method, headers, payload, encoding, this);
    connect(loopThread, &EventLoop::finished, loopThread, &QObject::deleteLater);
    loopThread->start();
}

void SSEClient::addEventListener(const QString& event, std::function<void(const Event&)> callback) {
    listeners[event] = callback;
    loopThread->addListener(event, callback);
}

void SSEClient::removeEventListener(const QString& event) {
    listeners.remove(event);
    loopThread->removeListener(event);
}

void SSEClient::close() {
    loopThread->stop();
    loopThread->wait();
}

