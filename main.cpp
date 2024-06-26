#include <QCoreApplication>
#include "sdk/client.h"
#include "sdk/services/recordservice.h"
#include "sdk/services/baseauthstore.h"
#include "sdk/services/clientresponseerror.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client{"http://127.0.0.1:5740/"};

    // Auth with password
    // auto user = client.collection("users")->authWithPassword("test@user.com", "123456789");
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

    // auto items = client.collection("temperature")->getList(2,30);
    // qDebug() << "Items: " << items.items().size();

    // QJsonObject filter;
    // filter["sort"] = "-created";
    // filter["filter"] = "created>'2024-06-22 10:28'";
    // auto items = client.collection("temperature")->getFullList(500, filter);
    // qDebug() << "Items: " << items.size();

    // for(const auto &item : items) {
    //     qDebug() << item->data()["created"].toString();
    // }

    // auto item = client.collection("temperature")->getOne("8oc6rhllnbekc0y");
    // qDebug() << "One Item: " << item->data();

    // auto d = client.collection("temperature")->getFirstListItem("value=2121");

    // qDebug() << "Result -> " << d->data();

    // Create
    // QJsonObject temp;
    // temp.insert("value", 321.345);
    // auto d = client.collection("temperature")->create(temp);
    // qDebug() << d->data();

    // Update

    // try {
    //     temp.insert("value", 320.5);
    //     auto e = client.collection("temperature")->update("0gje5svwe1uemcy", temp);
    //     qDebug() << e.data();
    // } catch (ClientResponseError e) {
        //qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // try {
    //     qDebug() << "Deleted? " << client.collection("temperature")->deleteRecord("0gje5svwe1uemcy");
    // } catch (ClientResponseError e) {
    //     qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // Delete

    return a.exec();
}
