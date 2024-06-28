#include <QCoreApplication>
#include "sdk/client.h"
#include "sdk/services/recordservice.h"
#include "sdk/services/baseauthstore.h"
#include "sdk/services/clientresponseerror.h"
#include "sdk/services/adminservice.h"
#include "sdk/services/collectionservice.h"
#include "sdk/dtos/adminmodel.h"
#include "sdk/services/healthservice.h"
#include "sdk/services/settingsservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client{"http://127.0.0.1:5740/"};

    // ADMIN
    try {
        auto user = client.admins()->authWithPassword("admin@admin.com", "12345678901");
        qDebug() << "Admin User: " << user.getToken() << "\t: " << user.getAdmin()->data();
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }

    // try {
    //     QJsonObject body;
    //     body["email"] = "admin@admin.com";
    //     body["password"] = "12345678901";
    //     body["passwordConfirm"] = "12345678901";

    //     auto user = client.admins()->create(body);
    //     qDebug() << "New Admin User: " << user->data();
    // } catch (ClientResponseError e) {
    //     qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // Delete Admin Account
    // try {
    //     qDebug() << "Deleted? " << client.admins()->deleteOne("yb91mv96rfzdtuh");
    // } catch (ClientResponseError e) {
    //     qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // try {
    //     qDebug() << "Get One Admin ...";
    //     auto admin = client.admins()->getOne("ry4ntta74xnwbhh");
    //     qDebug() << "\nOne Admin: " << admin->data();

    //     qDebug() << "List All Admins ...";
    //     auto admins = client.admins()->getFullList();
    //     qDebug() << "\nNumber of Admins: " << admins.size();

    // } catch (ClientResponseError e) {
    //     qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // Collection Matters
    try {
        // Returns a paginated collections list.
        // qDebug() << "\n fetching paginated collections list";
        // auto list = client.collections()->getList(1, 30);
        // qDebug() << "Number of collections: " << list.items().size();

        // Returns a list with all collections batch fetched at once.
        // qDebug() << "\n list with all collections batch fetched at once";
        // auto fullList = client.collections()->getFullList();
        // qDebug() << "Number of all collections at once: " << fullList.size();

        // Returns the first found collection matching the specified filter.
        // auto col = client.collections()->getFirstListItem("name='temperature'");
        // qDebug() << "Col: " << col->data();

        // Returns a single collection by its id.
        // auto byId = client.collections()->getOne("temperature"); // "pgr0wspl39wn3xs");
        // qDebug() << "Is 'pgr0wspl39wn3xs' == " << byId->getId();

        // Creates (aka. register) a new collection.
        QJsonObject recordA, col;
        QJsonArray schema;
        recordA["name"] = "something";
        recordA["type"] = "base";
        // recordA["createRule"] = "id = @request.auth.id";
        // Column 1
        col["name"] = "col1";
        col["type"] = "text";
        schema.append(col);
        // Column 2
        col["name"] = "col2";
        col["type"] = "number";
        col["required"] = true;
        schema.append(col);

        recordA["schema"] = schema;
        // auto newCol = client.collections()->create(recordA);
        // qDebug() << "New Table: " << newCol->data();

        // Updates an existing collection by its id.
        // recordA["name"] = "somethings";
        // auto tableUpdate = client.collections()->update("something",  recordA);
        // qDebug() << "Update: " << tableUpdate->data();

        // Deletes a single collection by its id.
        // qDebug() << "Is deleted? " << client.collections()->deleteOne("somethings");

        // Imports the provided collections.
        // QJsonArray schemas;
        // schemas.append(recordA);
        // qDebug() << "Import Successful? " << client.collections()->import(schemas);

    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }

    // Health check
    // try {
    //     auto health = client.health()->check();
    //     qDebug() << "New Admin User: " << health;
    // } catch (ClientResponseError e) {
    //     qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    // }

    // Health check
    try {
        auto health = client.settings()->getAll();
        qDebug() << "Settings: " << health;
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }

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
