#include <QCoreApplication>
#include "sdk/client.h"
// #include "sdk/RecordService.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PocketBase client1{"http://127.0.0.1:5740/"};
    Client client2{"http://127.0.0.1:5740/"};
    qDebug() << client1.lang() << client2.lang();
    // client.collection("users")->authWithPassword("user@email.com", "123456789");

    return a.exec();
}
