#ifndef SSECLIENT_H
#define SSECLIENT_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Event : public QObject {
    Q_OBJECT

public:
    explicit Event(const QString& id = "",
                   const QString& event = "message",
                   const QString& data = "",
                   int retry = -1);

    QString getId() const;
    void setId(const QString& nId) { id = nId; };

    QString getEvent() const;
    void setEvent(const QString& nEvent) { event = nEvent; };

    QString getData() const;
    void setData(const QString& nData) { data = nData; };

    int getRetry() const;
    void setRetry(const int& nRetry) { retry = nRetry; };

private:
    QString id;
    QString event;
    QString data;
    int retry;
};

class EventLoop : public QThread {
    Q_OBJECT

public:
    explicit EventLoop(const QString& url,
                       const QString& method = "GET",
                       const QMap<QString, QString>& headers = {},
                       const QByteArray& payload = {},
                       const QString& encoding = "utf-8",
                       QObject* parent = nullptr);
    void addListener(const QString& event, std::function<void(Event)> listener);
    void removeListener(const QString& event);
    void run() override;
    void stop();

signals:
    void eventReceived(const Event& event);

private slots:
    void onReadyRead();

private:
    void readStream();
    void processEvent(const QByteArray& chunk);

    QString url;
    QString method;
    QMap<QString, QString> headers;
    QByteArray payload;
    QString encoding;
    QNetworkAccessManager* networkManager;
    QNetworkReply* reply;
    bool kill;
    QMap<QString, std::function<void(Event)>> listeners;
};

class SSEClient : public QObject {
    Q_OBJECT

public:
    explicit SSEClient(const QString& url,
                       const QString& method = "GET",
                       const QMap<QString, QString>& headers = {},
                       const QByteArray& payload = QByteArray(),
                       const QString& encoding = "utf-8",
                       QObject* parent = nullptr);
    void addEventListener(const QString& event,
                          std::function<void(const Event&)> callback);
    void removeEventListener(const QString& event);
    void close();

private:
    EventLoop* loopThread;
    QMap<QString, std::function<void(const Event&)>> listeners;
};

#endif // SSECLIENT_H
