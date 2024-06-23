#ifndef RECORDModelMODEL_HPP
#define RECORDModelMODEL_HPP

#include <QString>
#include <QMap>
#include <QVariant>
#include "utils/basemodel.h"
#include "../utils/utils.h"

class RecordModel : public BaseModel {
public:
    QString collectionId;
    QString collectionName;
    QMap<QString, QVariant> expand;

    RecordModel(const QJsonObject &data) {
        qDebug() << "[Data Record Model] " << data;
        load(data);
    }

    void load(const QJsonObject &data) override {
        BaseModel::load(data);
        expand.clear();

        for (auto it = data.begin(); it != data.end(); ++it) {
            QString key = Utils::camelToSnake(it.key()).replace("@", "");
            expand.insert(key, QVariant(it.value()));
            this->setProperty(key.toStdString().c_str(), QVariant(it.value()));
        }

        loadExpanded();
    }

    static QList<RecordModel> parseExpanded(const QVariant &data) {
        QList<RecordModel> recordModels;
        if (data.type() == QVariant::List) {
            for (const auto &item : data.toList()) {
                // recordModels.append(RecordModel(item.toJsonObject()));
            }
        } else if (data.type() == QVariant::Map) {
            // recordModels.append(RecordModel(data.toJsonObject()));
        }
        return recordModels;
    }

    void loadExpanded() {
        for (auto it = expand.begin(); it != expand.end(); ++it) {
            auto l = parseExpanded(it.value());
            // expand[it.key()] = QVariantList(l.begin(), l.end());
        }
    }
};

#endif // RECORDModelMODEL_HPP
