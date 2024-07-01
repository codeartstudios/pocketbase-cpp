#include "SchemaField.h"

namespace pb {
SchemaField::SchemaField(const QString& id, const QString& name, const QString& type, bool system, bool required, bool presentable, bool unique, const QMap<QString, QVariant>& options)
    : id(id), name(name), type(type), system(system), required(required), presentable(presentable), unique(unique), options(options) {}

QString SchemaField::getId() const {
    return id;
}

void SchemaField::setId(const QString& id) {
    this->id = id;
}

QString SchemaField::getName() const {
    return name;
}

void SchemaField::setName(const QString& name) {
    this->name = name;
}

QString SchemaField::getType() const {
    return type;
}

void SchemaField::setType(const QString& type) {
    this->type = type;
}

bool SchemaField::isSystem() const {
    return system;
}

void SchemaField::setSystem(bool system) {
    this->system = system;
}

bool SchemaField::isRequired() const {
    return required;
}

void SchemaField::setRequired(bool required) {
    this->required = required;
}

bool SchemaField::isPresentable() const {
    return presentable;
}

void SchemaField::setPresentable(bool presentable) {
    this->presentable = presentable;
}

bool SchemaField::isUnique() const {
    return unique;
}

void SchemaField::setUnique(bool unique) {
    this->unique = unique;
}

QMap<QString, QVariant> SchemaField::getOptions() const {
    return options;
}

void SchemaField::setOptions(const QMap<QString, QVariant>& options) {
    this->options = options;
}
}