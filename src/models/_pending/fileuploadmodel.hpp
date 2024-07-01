#ifndef FILEUPLOADMODEL_HPP
#define FILEUPLOADMODEL_HPP

#include <QString>
#include <QVariant>
#include <QList>
#include <QPair>

namespace pb {
using FileUploadTypes = QVariant;  // Use QVariant to handle multiple types

class FileUploadModel {
public:
    FileUploadModel(const QList<FileUploadTypes>& files);

    QList<QPair<QString, FileUploadTypes>> get(const QString& key) const;

private:
    QList<FileUploadTypes> files;
};

FileUploadModel::FileUploadModel(const QList<FileUploadTypes>& files)
    : files(files) {}

QList<QPair<QString, FileUploadTypes>> FileUploadModel::get(const QString& key) const {
    QList<QPair<QString, FileUploadTypes>> result;
    for (const auto& file : files) {
        result.append(qMakePair(key, file));
    }
    return result;
}
}

#endif // FILEUPLOADMODEL_HPP
