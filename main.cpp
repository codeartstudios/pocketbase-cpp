#include <QCoreApplication>
#include "sdk/client.h"
#include "sdk/services/recordservice.h"
#include "sdk/services/baseauthstore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client{"http://127.0.0.1:5740/"};

    // Auth with password
    client.collection("users")->authWithPassword("test@user.com", "123456789");
    // qDebug() << client.authStore()->isValid() << user.isValid();
    // qDebug() << client.authStore()->token();
    // qDebug() << client.authStore()->model()->getId();
    // qDebug() << client.authStore()->model()->repr();

    // Auth Refresh
    //auto refreshUser = client.collection("users")->authRefresh();
    //qDebug() << refreshUser.isValid();

    // Request Verification
    // client.collection("users")->requestVerification("allankoech.ke@gmail.com");

    qDebug() << client.collection("users")->confirmVerification("12345");

    qDebug() << client.collection("users")->requestPasswordReset("allankoech.ke@gmail.com");

    return a.exec();
}
