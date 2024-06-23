#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>

class FileService : public QObject
{
    Q_OBJECT
public:
    explicit FileService(QObject *parent = nullptr);

signals:
};

#endif // FILESERVICE_H
