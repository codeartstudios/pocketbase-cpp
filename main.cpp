#include <QCoreApplication>
#include "sdk/client.h"
#include "sdk/services/recordservice.h"
#include "sdk/services/baseauthstore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client{"http://127.0.0.1:5740/"};

    // Auth with password
    // auto user = client.collection("users")->authWithPassword("EMAIL", "PASSWORD");
    // qDebug() << client.authStore()->isValid() << user.isValid();
    // qDebug() << client.authStore()->token();
    // qDebug() << client.authStore()->model()->getId();
    // qDebug() << client.authStore()->model()->repr();

    // Auth Refresh
    //auto refreshUser = client.collection("users")->authRefresh();
    //qDebug() << refreshUser.isValid();

    // Request Verification
    // qDebug() << client.collection("users")->requestVerification("EMAIL");

    //qDebug() << client.collection("users")->confirmVerification("CODE");

    // qDebug() << client.collection("users")->requestPasswordReset("EMAIL");

    // qDebug() << client.collection("users")->confirmPasswordReset("TOKEN", "PASSWORD", "CONFIRM PASSWORD");

    // qDebug() << client.collection("users")->requestEmailChange("EMAIL");

    // qDebug() << client.collection("users")->confirmEmailChange("TOKEN", "PASSWORD");

    // client.collection("users")->listAuthMethods();

    return a.exec();
}
