#ifndef RECORDMODEL_HPP
#define RECORDMODEL_HPP

#include <QString>
#include <QMap>
#include <QVariant>

#include "../globals.h"
#include "../models/basemodel.h"

namespace pb {
class RecordModel : public BaseModel {
public:
    QString collectionId;
    QString collectionName;
    QMap<QString, QVariant> expand;

    RecordModel(const QJsonObject &data) {
        collectionName = data.value("collectionName").toString("");
        collectionId = data.value("collectionId").toString("");
        load(data);
    }

    RecordModel(BaseModel* baseModel) {
        auto data = baseModel->data();
        collectionName = data.value("collectionName").toString("");
        collectionId = data.value("collectionId").toString("");

        load(data);
    }

    bool isEmpty() const { return expand.isEmpty(); }

    void load(const QJsonObject &data) override {
        BaseModel::load(data);
        expand.clear();

        for (auto it = data.begin(); it != data.end(); ++it) {
            QString key = pb::camelToSnake(it.key()).replace("@", "");
            expand.insert(key, QVariant(it.value()));
            this->setProperty(key.toStdString().c_str(), QVariant(it.value()));
        }

        loadExpanded();
    }

    static QList<RecordModel> parseExpanded(const QVariant &data) {
        QList<RecordModel> recordModels;
        if (data.typeId() == QMetaType::QJsonArray) {
            // for (const auto &item : data.toList()) {
            //     //recordModels.append(RecordModel(item.toJsonObject()));
            // }
        } else if (data.typeId() == QMetaType::QVariantMap) {
            //recordModels.append(RecordModel(data.toJsonObject()));
        }
        return recordModels;
    }

    void loadExpanded() {
        for (auto it = expand.begin(); it != expand.end(); ++it) {
            // auto l = parseExpanded(it.value());
            // expand[it.key()] = QVariantList(l.begin(), l.end());
        }
    }

    QString getCollectionName() const { return collectionName; }

    QString getCollectionId() const { return collectionId; }
};
}

#endif // RECORDMODEL_HPP
