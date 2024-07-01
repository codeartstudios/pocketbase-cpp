#ifndef SSECLIENT_H
#define SSECLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>

namespace pb {
class PocketBase;

class SSEClient : public QObject
{
    Q_OBJECT
public:
    explicit SSEClient(const QString& url,
                       PocketBase* client,
                       QObject* parent = nullptr);
    ~SSEClient();

    void connectSSE();
    void close();
    void setUrl(const QString url);

    bool isOpen();      // SSEClient Open
    bool isConnected(); // SSEClient connected, PB_CONNECT not yet received

private slots:
    // Slots called when the Network Manager has data ready
    // for reading and when its closed
    void onReplyReadyRead();
    void onReplyFinished();

signals:
    // Signal emitted for each event from the SSEClient
    void dataReceived(QString id,
                      QString event,
                      QString data );

private:
    QNetworkAccessManager * m_nman;
    QString m_url;
    bool m_connected, m_open;
    PocketBase* client;
};
}

#endif // SSECLIENT_H
