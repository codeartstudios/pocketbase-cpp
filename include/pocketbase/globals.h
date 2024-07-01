#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QString>
#include <QRegularExpression>
#include <QJsonObject>
#include <QUrlQuery>

namespace pb {
inline QString camelToSnake(const QString &str) {
    QString result = str;
    auto _ = result.replace(QRegularExpression("([a-z])([A-Z])"), "\\1_\\2").toLower();
    return result;
}

inline QJsonObject urlQueryToJson(const QUrlQuery &query) {
    QJsonObject jsonObj;
    for (const QPair<QString, QString> &item : query.queryItems()) {
        jsonObj.insert(item.first, item.second);
    }
    return jsonObj;
}

inline QByteArray base64UrlDecode(const QString& base64Url) {
    QString base64 = base64Url;
    base64.replace('-', '+').replace('_', '/');

    while (base64.length() % 4 != 0) {
        base64.append('=');
    }

    return QByteArray::fromBase64(base64.toUtf8());
}
}

#endif // GLOBALUTILS_H
