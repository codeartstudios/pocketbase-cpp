#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QObject>

class BackupService : public QObject
{
    Q_OBJECT
public:
    explicit BackupService(QObject *parent = nullptr);

signals:
};

#endif // BACKUPSERVICE_H
