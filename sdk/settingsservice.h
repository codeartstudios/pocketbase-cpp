#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <QObject>

class SettingsService : public QObject
{
    Q_OBJECT
public:
    explicit SettingsService(QObject *parent = nullptr);

signals:
};

#endif // SETTINGSSERVICE_H
