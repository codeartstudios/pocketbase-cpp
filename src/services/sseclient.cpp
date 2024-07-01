#include <pocketbase/services/sseclient.h>
#include <pocketbase/services/baseauthstore.h>
#include <pocketbase/client.h>

namespace pb {
SSEClient::SSEClient(const QString& url, PocketBase* client, QObject *parent)
    : QObject(parent),
    m_url(url),
    m_connected(false),
    m_open(false),
    client(client) {}

SSEClient::~SSEClient() {
    m_nman->deleteLater();
}

void SSEClient::connectSSE()
{
    m_nman =  new QNetworkAccessManager();
    QNetworkRequest request(QUrl{m_url});

    auto token = client->authStore()->token().toUtf8();
    qDebug() << "Token: " << token;

    request.setRawHeader("Accept","text/event-stream");
    request.setRawHeader("Authorization", "Bearer " + token);
    // request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::AlwaysNetwork);

    qDebug() << "Sending GET request to > " << request.url().toString();

    QNetworkReply *reply = m_nman->get(request);
    connect(reply, &QNetworkReply::readyRead,
            this, &SSEClient::onReplyReadyRead);
    connect(reply, &QNetworkReply::finished,
            this, &SSEClient::onReplyFinished);

    // QEventLoop wait_loop;
    // connect(reply, &QNetworkReply::finished, &wait_loop, &QEventLoop::quit);
    // wait_loop.exec();
}

void SSEClient::close()
{
    if( m_nman ) {
        //
    }
}

void SSEClient::setUrl(const QString url) { m_url = url; }

bool SSEClient::isOpen() { return m_open; }

bool SSEClient::isConnected() { return m_connected; }

void SSEClient::onReplyReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if(reply)
    {
        m_connected = true;

        QString response(reply->readAll());
        QStringList responseList = response.split("\n");

        if( responseList.size() > 2 )
        {
            QString id=responseList.at(0);
            id = id.replace("id:","");

            QString event=responseList.at(1);
            event=event.replace("event:","");

            QStringList dataArray;

            // for(int i=2; i<responseList.size(); i++) {
            //     QString data=responseList.at(i);
            //     data = data.trimmed();
            //     if(data.startsWith("data:"))
            //         data=data.replace("data:","");
            //     if( data!="" ) dataArray.append(data);
            // }

            QString data=responseList.at(2);
            data = data.trimmed();
            if(data.startsWith("data:"))
                data=data.replace("data:","");
            if( data!="" ) dataArray.append(data);

            if( event == "PB_CONNECT") {
                m_open = true;
            }

            emit dataReceived(id,event,data);
        }
    }

    else
        m_connected = false;
}

void SSEClient::onReplyFinished()
{
    m_open = false;
    m_connected = false;
    QTimer::singleShot(1000, [&](){ connectSSE(); });
}
}
