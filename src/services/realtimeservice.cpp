#include <pocketbase/services/realtimeservice.h>
#include <pocketbase/client.h>

namespace pb {
RealtimeService::RealtimeService(PocketBase *client, QObject *parent)
    : QObject{parent},
    client(client) {
    m_sseClient = new SSEClient( client->buildUrl("/api/realtime").toString(), client );

    connect(m_sseClient, &SSEClient::dataReceived,
            this, &RealtimeService::handleMessages);

    m_sseThread = new QThread(this);
    m_sseClient->moveToThread(m_sseThread);
    connect(m_sseThread, &QThread::finished, m_sseThread, &QThread::quit);
    m_sseThread->start();
}

QString RealtimeService::clientID() const { return m_clientID; }

void RealtimeService::subscribe(QString topic,
                                std::function<void (const Event &)> listener,
                                const QJsonObject &params) {
    QString key = topic;

    QJsonObject payload = params;

    if ( !m_subscriptions.contains(key) ) {
        m_subscriptions[key] = {};
    }

    m_subscriptions[key].append(listener);

    // bool isConnected = false;

    // start a new sse connection
    if( !m_sseClient->isConnected() ) {
        m_sseClient->connectSSE();
    }

    if ( m_sseClient->isOpen() &&
        !m_clientID.isEmpty() &&
        m_subscriptions[key].size() >= 1 ) {
        // otherwise - just persist the updated subscriptions
        // (if it is the first for the topic)
        qDebug() << "Sumitted Results? " << submitSubscriptions();
    }
}

void RealtimeService::unsubscribe(const QString &topic) {
    bool needToSubmit = false;

    if ( topic.isEmpty() ) {
        // remove all subscriptions
        m_subscriptions.clear();
    } else {
        auto subs = getSubscriptionsByTopic(topic);

        for ( const auto& key : subs.keys() ) {
            m_subscriptions.remove(key);
            needToSubmit = true;
        }
    }

    // no other subscriptions -> close the sse connection
    if ( !hasNonEmptyTopic() ) {
        disconnectSSE();
    }

    // otherwise - notify the server about the subscription changes
    if ( !m_clientID.isEmpty() && needToSubmit) {
        submitSubscriptions();
    }
}

QMap<QString, QList<std::function<void (const Event &)> > >
RealtimeService::getSubscriptionsByTopic(const QString &topic) {
    QMap<QString, QList<std::function<void(const Event&)>>> result{};

    // "?" so that it can be used as end delimiter for the topic
    QString _topic = topic.contains("?") ? topic : topic+"?";

    for( const QString& key : m_subscriptions.keys() ) {
        auto value = m_subscriptions[key];
        QString _key = key+"?";

        if (_key.startsWith(_topic)) {
            result[key] = value;
        }
    }

    return result;
}

bool RealtimeService::hasNonEmptyTopic() {
    for ( const auto& key : m_subscriptions.keys() ) {
        if ( !m_subscriptions[key].isEmpty() ) {
            return true; // has at least one listener
        }
    }

    return false;
}

bool RealtimeService::connectSSE() {
    disconnectSSE();

    QString url = client->buildUrl("/api/realtime").toString();
    m_sseClient->setUrl(url);

    // bind subscriptions listener
    // _sse?.onMessage.listen((msg) {
    //   if (!_subscriptions.containsKey(msg.event)) {
    //     return;
    //   }

    //   _subscriptions[msg.event]?.forEach((fn) {
    //     fn.call(msg);
    //   });

    return false;
}

void RealtimeService::disconnectSSE() {
    m_sseClient->close();
    m_clientID.clear();
}

bool RealtimeService::submitSubscriptions() {
    qDebug() << "[Realtime Service] Submitting subscriptions";

    QJsonObject payload, body;
    body["clientId"] = m_clientID;
    body["subscriptions"] = QJsonArray::fromStringList(m_subscriptions.keys());
    payload["method"] = "POST";
    payload["body"] = body;

    auto responseJSON = client->send("/api/realtime", payload);
    qDebug() << "[Submitted Subs] Response: " << responseJSON;
    return responseJSON.value("statusCode").toInt() == 204;
}

void RealtimeService::onSSEConnected(const QJsonObject& data)
{
    // Get the client ID
    m_clientID = data.value("clientId").toString("");

    // Lets resend this if it fails
    if( !submitSubscriptions() ) {
        QTimer::singleShot(1000, [&]() { onSSEConnected(data); });
    }
}

void RealtimeService::handleMessages(const QString& id,
                                     const QString& event,
                                     const QString& data)
{
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject obj = doc.object();

    emit dataChanged(event, obj);

    if( event == "PB_CONNECT" ) {
        onSSEConnected(obj);
    }

    // Call the callbacks
    if( m_subscriptions.contains(event) ) {
        auto callbacks = m_subscriptions[event];
        for( const auto& callback : callbacks ) {
            auto e = Event(id, event, obj);
            callback(e);
        }
    }
}
}
