#ifndef COLLECTIONSERVICE_H
#define COLLECTIONSERVICE_H

#include <QObject>
#include <QDebug>

#include "crudservice.h"

class CollectionService : public QObject, public CrudService
{
    Q_OBJECT
public:
    explicit CollectionService(PocketBase* client, QObject *parent = nullptr);

    QString baseCrudPath() const override;
    bool import(QJsonArray& collections, bool deleteMissing = false,
                QJsonObject headers = {}, QJsonObject query = {} );
    BaseModel* decode(const QJsonObject& data) override;
private:
    PocketBase* client;
};

#endif // COLLECTIONSERVICE_H
