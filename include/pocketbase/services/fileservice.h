#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QUrl>

namespace pb {
class PocketBase;
class RecordModel;

class FileService : public QObject
{
    Q_OBJECT
public:
    explicit FileService(PocketBase* client, QObject *parent = nullptr);

    /// Builds and returns an absolute record file url.
    QString getUrl(const RecordModel& record,
                   const QString& filename = "",
                   const QString& thumb = "",
                   const QString& token = "",
                   bool download = false);

    QString getUrl(const QString& collectionIdOrName,
                   const QString& recordId,
                   const QString& filename = "",
                   const QString& thumb = "",
                   const QString& token = "",
                   bool download = false);

    /// Requests a new private file access token for the current auth model.
    Q_INVOKABLE QString getToken();

private:
    PocketBase* client;
};
}

#endif // FILESERVICE_H
