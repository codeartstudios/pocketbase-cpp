#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QRegularExpression>
#include <QJsonObject>
#include <QUrlQuery>

namespace Utils {
QString camelToSnake(const QString &str) {
    QString result = str;
    result.replace(QRegularExpression("([a-z])([A-Z])"), "\\1_\\2").toLower();
    return result;
}

QJsonObject urlQueryToJson(const QUrlQuery &query) {
    QJsonObject jsonObj;
    for (const QPair<QString, QString> &item : query.queryItems()) {
        jsonObj.insert(item.first, item.second);
    }
    return jsonObj;
}
}

#endif // UTILS_H
