#ifndef SCHEMAFIELD_H
#define SCHEMAFIELD_H

#include <QString>
#include <QMap>
#include <QJsonObject>

namespace pb {
class SchemaField {
public:
    SchemaField(const QString& id = "", const QString& name = "", const QString& type = "text", bool system = false, bool required = false, bool presentable = false, bool unique = false, const QMap<QString, QVariant>& options = QMap<QString, QVariant>());
    SchemaField(const QJsonObject &json) {} // TODO

    QString getId() const;
    void setId(const QString& id);

    QString getName() const;
    void setName(const QString& name);

    QString getType() const;
    void setType(const QString& type);

    bool isSystem() const;
    void setSystem(bool system);

    bool isRequired() const;
    void setRequired(bool required);

    bool isPresentable() const;
    void setPresentable(bool presentable);

    bool isUnique() const;
    void setUnique(bool unique);

    QMap<QString, QVariant> getOptions() const;
    void setOptions(const QMap<QString, QVariant>& options);

private:
    QString id;
    QString name;
    QString type;
    bool system;
    bool required;
    bool presentable;
    bool unique;
    QMap<QString, QVariant> options;
};
}

#endif // SCHEMAFIELD_H
