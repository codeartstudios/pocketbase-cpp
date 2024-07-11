#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

namespace pb {
class PocketBase;
class RecordModel;

// Handle file attachment for uploading
// Do not pass file path as a string to the
// endpoints as the API will assume its a text field
// Usage
// auto f1 = FileUpload("/home/x/user.png")
// auto f2 = FileUpload(["/home/x/user.png", "/home/x/user2.png"])
//
// Pass as a json parameter
// {
//      "name": "John Doe",
//      "avatar": f1.toObject()
// }
class FileUpload {
public:
    FileUpload(const QString& path);

    FileUpload(const QStringList& paths);

    QJsonObject toObject();

private:
    QList<QString> fileList;
};

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
