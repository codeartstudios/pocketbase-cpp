#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <functional>
#include "../event.h"

class EventLoop : public QThread {
    Q_OBJECT

public:
    explicit EventLoop(const QString& url, const QString& method = "GET", const QMap<QString, QString>& headers = {}, const QByteArray& payload = QByteArray(), const QString& encoding = "utf-8", QObject* parent = nullptr);
    void addEventListener(const QString& event, std::function<void(const Event&)> callback);
    void removeEventListener(const QString& event);

protected:
    void run() override;

private:
    void readStream();
    void processEvent(const QByteArray& data);

    QString url;
    QString method;
    QMap<QString, QString> headers;
    QByteArray payload;
    QString encoding;
    QNetworkAccessManager networkManager;
    bool kill;
    QMap<QString, std::function<void(const Event&)>> listeners;
};

#endif // EVENTLOOP_H
