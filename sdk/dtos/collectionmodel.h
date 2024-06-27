#ifndef COLLECTIONMODEL_HPP
#define COLLECTIONMODEL_HPP

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QMap>

#include "basemodel.h"
#include "schemafield.h"

class CollectionModel : public BaseModel {
public:
    CollectionModel(const QJsonObject& data = QJsonObject());

    QString getName() const;
    void setName(const QString& name);

    QString getType() const;
    void setType(const QString& type);

    QList<SchemaField> getSchema() const;
    void setSchema(const QList<SchemaField>& schema);

    bool isSystem() const;
    void setSystem(bool system);

    QString getListRule() const;
    void setListRule(const QString& listRule);

    QString getViewRule() const;
    void setViewRule(const QString& viewRule);

    QString getCreateRule() const;
    void setCreateRule(const QString& createRule);

    QString getUpdateRule() const;
    void setUpdateRule(const QString& updateRule);

    QString getDeleteRule() const;
    void setDeleteRule(const QString& deleteRule);

    QMap<QString, QVariant> getOptions() const;
    void setOptions(const QMap<QString, QVariant>& options);

    void load(const QJsonObject& data);

    bool isBase() const;
    bool isAuth() const;
    bool isSingle() const;

private:
    QString name;
    QString type;
    QList<SchemaField> schema;
    bool system;
    QString listRule;
    QString viewRule;
    QString createRule;
    QString updateRule;
    QString deleteRule;
    QMap<QString, QVariant> options;
};

CollectionModel::CollectionModel(const QJsonObject& data)
    : BaseModel(data), system(false) {
    load(data);
}

QString CollectionModel::getName() const {
    return name;
}

void CollectionModel::setName(const QString& name) {
    this->name = name;
}

QString CollectionModel::getType() const {
    return type;
}

void CollectionModel::setType(const QString& type) {
    this->type = type;
}

QList<SchemaField> CollectionModel::getSchema() const {
    return schema;
}

void CollectionModel::setSchema(const QList<SchemaField>& schema) {
    this->schema = schema;
}

bool CollectionModel::isSystem() const {
    return system;
}

void CollectionModel::setSystem(bool system) {
    this->system = system;
}

QString CollectionModel::getListRule() const {
    return listRule;
}

void CollectionModel::setListRule(const QString& listRule) {
    this->listRule = listRule;
}

QString CollectionModel::getViewRule() const {
    return viewRule;
}

void CollectionModel::setViewRule(const QString& viewRule) {
    this->viewRule = viewRule;
}

QString CollectionModel::getCreateRule() const {
    return createRule;
}

void CollectionModel::setCreateRule(const QString& createRule) {
    this->createRule = createRule;
}

QString CollectionModel::getUpdateRule() const {
    return updateRule;
}

void CollectionModel::setUpdateRule(const QString& updateRule) {
    this->updateRule = updateRule;
}

QString CollectionModel::getDeleteRule() const {
    return deleteRule;
}

void CollectionModel::setDeleteRule(const QString& deleteRule) {
    this->deleteRule = deleteRule;
}

QMap<QString, QVariant> CollectionModel::getOptions() const {
    return options;
}

void CollectionModel::setOptions(const QMap<QString, QVariant>& options) {
    this->options = options;
}

void CollectionModel::load(const QJsonObject& data) {
    BaseModel::load(data);
    name = data.value("name").toString();
    type = data.value("type").toString("base");
    system = data.value("system").toBool(false);
    options = data.value("options").toObject().toVariantMap();

    listRule = data.value("listRule").toString();
    viewRule = data.value("viewRule").toString();
    createRule = data.value("createRule").toString();
    updateRule = data.value("updateRule").toString();
    deleteRule = data.value("deleteRule").toString();

    QJsonArray schemaArray = data.value("schema").toArray();
    schema.clear();
    for (const QJsonValue& fieldValue : schemaArray) {
        QJsonObject fieldObject = fieldValue.toObject();
        SchemaField field(fieldObject); // ---- TODO ---
        schema.append(field);
    }
}

bool CollectionModel::isBase() const {
    return type == "base";
}

bool CollectionModel::isAuth() const {
    return type == "auth";
}

bool CollectionModel::isSingle() const {
    return type == "single";
}

#endif // COLLECTIONMODEL_HPP
