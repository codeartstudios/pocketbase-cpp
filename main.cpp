#include <QCoreApplication>
#include "sdk/qpocketbase.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QPocketBase client;

    return a.exec();
}
