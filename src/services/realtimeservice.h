#ifndef REALTIMESERVICE_H
#define REALTIMESERVICE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <functional>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QThread>

#include "event.h"
#include "sseclient.h"

namespace pb {
class PocketBase;

class RealtimeService : public QObject {
    Q_OBJECT
public:
    explicit RealtimeService(PocketBase* client,
                             QObject* parent = nullptr);

    // Returns the established SSE connection client id (if any).
    QString clientID() const;

    void subscribe( QString topic,
                   std::function<void(const Event&)> listener,
                   const QJsonObject& params = QJsonObject());

    void unsubscribe(const QString& topic = "");

    QMap<QString, QList<std::function<void(const Event&)>>>
    getSubscriptionsByTopic(const QString& topic);

    bool hasNonEmptyTopic();

    bool connectSSE();

    void disconnectSSE();

    bool submitSubscriptions();

signals:
    void dataChanged(const QString& event,
                     const QJsonObject& data);

private slots:
    void onSSEConnected(const QJsonObject&);
    void handleMessages(const QString &id,
                        const QString &event,
                        const QString &list);

private:
    SSEClient* m_sseClient;
    QString m_clientID = "";
    QMap<QString, QList<std::function<void(const Event&)>>> m_subscriptions {};
    PocketBase* client;
    QThread* m_sseThread;
};
}

#endif // REALTIMESERVICE_H
