#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QString>
#include <QRegularExpression>
#include <QJsonObject>
#include <QUrlQuery>

namespace Utils {
inline QString camelToSnake(const QString &str) {
    QString result = str;
    result.replace(QRegularExpression("([a-z])([A-Z])"), "\\1_\\2").toLower();
    return result;
}

inline QJsonObject urlQueryToJson(const QUrlQuery &query) {
    QJsonObject jsonObj;
    for (const QPair<QString, QString> &item : query.queryItems()) {
        jsonObj.insert(item.first, item.second);
    }
    return jsonObj;
}
}

#endif // GLOBALUTILS_H
