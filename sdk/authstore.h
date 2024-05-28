#ifndef AUTHSTORE_H
#define AUTHSTORE_H

#include <QObject>

class AuthStore : public QObject
{
    Q_OBJECT
public:
    explicit AuthStore(QObject *parent = nullptr);

signals:
};

#endif // AUTHSTORE_H
