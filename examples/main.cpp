#include <QCoreApplication>
#include <pocketbase/client.h>
#include <pocketbase/services/recordservice.h>
#include <pocketbase/services/baseauthstore.h>
#include <pocketbase/services/clientresponseerror.h>
#include <pocketbase/services/adminservice.h>
#include <pocketbase/services/collectionservice.h>
#include <pocketbase/services/healthservice.h>
#include <pocketbase/services/settingsservice.h>
#include <pocketbase/services/logservice.h>
#include <pocketbase/models/adminmodel.h>
#include <pocketbase/models/logsmodel.h>
#include <pocketbase/services/fileservice.h>

#include <QTimer>
#include <QFile>

using namespace pb;


void loginAdmin(pb::PocketBase* client);

void createAdminAccount(pb::PocketBase* client);

void deleteAdminAccount(pb::PocketBase* client);

void getAdmins(pb::PocketBase* client);

void subscribeToRecord(pb::PocketBase* client, const QString& recordIdOrName);

void testCollections(pb::PocketBase* client);

void testHealthCheck(pb::PocketBase* client);

void testLogService(pb::PocketBase* client);

void testUserAuthentication(pb::PocketBase* client);

void testCrudRecords(pb::PocketBase* client);

void testFileService(pb::PocketBase* client);

void testFilter(pb::PocketBase* client);

void testFileUpload(pb::PocketBase* client);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client{"https://apps.codeartstudios.net/finance-tracker"};

    // testFilter(&client);
    // testFileUpload(&client);

    // Admin Accounts
    // loginAdmin(&client);
    // createAdminAccount(&client);
    // getAdmins(&client);
    // deleteAdminAccount(&client);

    // subscribeToRecord(&client, "collectionIdOrName");
    // testCollections(&client);
    // testHealthCheck(&client);

    // testLogService(&client);
    // testUserAuthentication(&client);
    // testCrudRecords(&client);
    // testFileService(&client);

    return a.exec();
}

void loginAdmin(pb::PocketBase* client) {
    try {
        auto user = client->admins()->authWithPassword("admin@admin.com", "12345678901");
        qDebug() << "Admin User Token: " << user.getToken();
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void createAdminAccount(pb::PocketBase* client) {
    try {
        QJsonObject body;
        body["email"] = "admin@admin.com";
        body["password"] = "12345678901";
        body["passwordConfirm"] = "12345678901";

        auto user = client->admins()->create(body);
        qDebug() << "New Admin User: " << user->data();
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void deleteAdminAccount(pb::PocketBase* client) {
    // Delete Admin Account
    try {
        qDebug() << "Deleted? " << client->admins()->deleteOne("<admin ID>");
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void getAdmins(pb::PocketBase* client) {
    try {
        // Get single admin from the given ID
        auto admin = client->admins()->getOne("<admin ID>");
        qDebug() << admin->data();

        // List All admins
        auto admins = client->admins()->getFullList();
        qDebug() << "\nNumber of Admins: " << admins.size();

    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void subscribeToRecord(pb::PocketBase* client, const QString& recordIdOrName) {
    try {
        client->collection(recordIdOrName)->subscribe([&](const Event& data){
            qDebug() << "Data: " << data.data();
        });

        // Lets unsubscribe after 20s
        QTimer::singleShot(20000, [&]() {
            client->collection(recordIdOrName)->unsubscribe();
            qDebug() << "Unsubscribe from 'cow'";
        });

    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testCollections(pb::PocketBase* client) {
    // Collection Matters
    try {
        // Returns a paginated collections list.
        qDebug() << "\n fetching paginated collections list";
        auto list = client->collections()->getList(1, 30);
        qDebug() << "Number of collections: " << list.items().size();

        // Returns a list with all collections batch fetched at once.
        auto fullList = client->collections()->getFullList();
        qDebug() << "Number of all collections at once: " << fullList.size();

        // Returns the first found collection matching the specified filter.
        auto column = client->collections()->getFirstListItem("name='<something>'");
        qDebug() << "Col: " << column->data();

        // Returns a single collection by its id.
        auto colByID = client->collections()->getOne("recordNameOrID");
        qDebug() << "Record ID: " << colByID->getId();

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
        auto newCol = client->collections()->create(recordA);
        qDebug() << "New Collection: " << newCol->data();

        // Updates an existing collection by its id.
        recordA["name"] = "somethings";
        auto tableUpdate = client->collections()->update("something",  recordA);
        qDebug() << "Update: " << tableUpdate->data();

        // Deletes a single collection by its id.
        qDebug() << "Is deleted? " << client->collections()->deleteOne("somethings");

        // Imports the provided collections.
        QJsonArray schemas;
        schemas.append(recordA); // Add collections to schema
        qDebug() << "Import Successful? " << client->collections()->import(schemas);

    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testLogService(pb::PocketBase* client) {
    try {
        QJsonObject params;
        params["filter"] = "level>0";
        auto logsmodel = client->logs()->getList();
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testUserAuthentication(pb::PocketBase* client) {
    try {
        auto user = client->collection("users")->authWithPassword("test@user.com", "123456789");
        qDebug() << client->authStore()->isValid();

        // Auth Refresh
        auto refreshUser = client->collection("users")->authRefresh();
        qDebug() << refreshUser.isValid();

        // Request Verification
        qDebug() << client->collection("users")->requestVerification("EMAIL");

        qDebug() << client->collection("users")->confirmVerification("CODE");

        qDebug() << client->collection("users")->requestPasswordReset("EMAIL");

        qDebug() << client->collection("users")->confirmPasswordReset("TOKEN", "PASSWORD", "CONFIRM PASSWORD");

        qDebug() << client->collection("users")->requestEmailChange("EMAIL");

        qDebug() << client->collection("users")->confirmEmailChange("TOKEN", "PASSWORD");

        client->collection("users")->listAuthMethods();
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testCrudRecords(pb::PocketBase* client) {
    try {
        auto items = client->collection("collectionIdOrName")->getList(2,30);
        qDebug() << "Items: " << items.items().size();

        QJsonObject filter;
        filter["sort"] = "-created";
        filter["filter"] = "created>'2024-06-22 10:28'";
        auto records = client->collection("collectionIdOrName")->getFullList(500, filter);
        qDebug() << "Items: " << records.data();

        // Get single record by ID
        auto item = client->collection("collectionIdOrName")->getOne("<row ID>");
        qDebug() << "One Item: " << item->data();

        auto firstItem = client->collection("collectionIdOrName")->getFirstListItem("value=2121");
        qDebug() << "First Item -> " << firstItem->data();

        // Create
        QJsonObject temp;
        temp.insert("value", 321.345);
        auto d = client->collection("collectionIdOrName")->create(temp);
        qDebug() << d->data();

        // Update
        temp.insert("value", 320.5);
        auto e = client->collection("collectionIdOrName")->update("0gje5svwe1uemcy", temp);
        qDebug() << e.data();

        qDebug() << "Deleted? " << client->collection("collectionIdOrName")->deleteRecord("0gje5svwe1uemcy");
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testHealthCheck(pb::PocketBase* client) {
    try {
        auto health = client->health()->check();
        qDebug() << "Response: " << health;
    } catch (ClientResponseError e) {
        qDebug() << "Error thrown! -> " << e.what() << "\t" << e.status();
    }
}

void testFileService(pb::PocketBase* client) {
    try {
        QObject::connect(client, &PocketBase::tokenChanged, [&](QString token){
            qDebug() << "New Token: " << token;
        });

        auto loggedUser = client->collection("users")->authWithPassword("test@user.com", "123456789");
        // qDebug() << client->files()->getUrl(loggedUser.record, "cat_TMRWGgRq8Z.jpg","","", true);
        QJsonObject params;
        params["download"] = true;
        params["token"] = client->getFileToken();
        qDebug() << client->getFileUrl(loggedUser.record, "cat_TMRWGgRq8Z.jpg", params);
    } catch(ClientResponseError e) {
        qDebug() << QString("> [%1] %2")
                        .arg(QString::number(e.status()), e.what());
    }
}

void testFilter(pb::PocketBase* client) {
    try {
        QJsonObject filter;
        filter["filter"] = "value=5";
        auto user = client->collection("posts")->getFullList(20, filter);
        qDebug() << "Admin User Token: " << user.size();
    } catch (ClientResponseError e) {
        qDebug() << QString("%1 %2").arg(QString::number(e.status()), e.what());
    }
}

void testFileUpload(pb::PocketBase* client) {
    try {
        auto resp = client->collection("users")->authWithPassword("<email or id>", "<password>");
        QJsonObject updateObj;
        QStringList f = {"<file url>"};
        updateObj["<key>"] = FileUpload(f).toObject();
        client->collection("<record>")->update("<id>", updateObj);
    } catch (ClientResponseError e) {
        qDebug() << QString("%1 %2").arg(QString::number(e.status()), e.what());
    }
}
