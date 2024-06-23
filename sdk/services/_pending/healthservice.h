#ifndef HEALTHSERVICE_H
#define HEALTHSERVICE_H

#include <QObject>

class HealthService : public QObject
{
    Q_OBJECT
public:
    explicit HealthService(QObject *parent = nullptr);

signals:
};

#endif // HEALTHSERVICE_H
