#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <QObject>

class AdminService : public QObject
{
    Q_OBJECT
public:
    explicit AdminService(QObject *parent = nullptr);

signals:
};

#endif // ADMINSERVICE_H
