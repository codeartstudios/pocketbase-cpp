#ifndef RECORDSERVICE_H
#define RECORDSERVICE_H

#include <QObject>

class RecordService : public QObject
{
    Q_OBJECT
public:
    explicit RecordService(const QString &collectionIdOrName, QObject *parent = nullptr);

signals:
};

#endif // RECORDSERVICE_H
