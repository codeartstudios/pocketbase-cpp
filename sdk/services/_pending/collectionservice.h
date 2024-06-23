#ifndef COLLECTIONSERVICE_H
#define COLLECTIONSERVICE_H

#include <QObject>

class CollectionService : public QObject
{
    Q_OBJECT
public:
    explicit CollectionService(QObject *parent = nullptr);

signals:
};

#endif // COLLECTIONSERVICE_H
