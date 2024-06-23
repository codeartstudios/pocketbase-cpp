#ifndef SSECLIENT_H
#define SSECLIENT_H

#include <QObject>
#include <QString>
#include <QMap>
#include <functional>
#include "EventLoop.h"
#include "Event.h"

class SSEClient : public QObject {
    Q_OBJECT

public:
    explicit SSEClient(const QString& url, const QString& method = "GET", const QMap<QString, QString>& headers = {}, const QByteArray& payload = QByteArray(), const QString& encoding = "utf-8", QObject* parent = nullptr);
    void addEventListener(const QString& event, std::function<void(const Event&)> callback);
    void removeEventListener(const QString& event);
    void close();

private:
    EventLoop* loopThread;
    QMap<QString, std::function<void(const Event&)>> listeners;
};

#endif // SSECLIENT_H
