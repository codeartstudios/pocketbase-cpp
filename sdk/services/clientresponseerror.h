#ifndef CLIENTRESPONSEERROR_H
#define CLIENTRESPONSEERROR_H

#include <stdexcept>
#include <QString>
#include <QMap>
#include <QVariant>

class ClientResponseError : public std::runtime_error {
public:
    ClientResponseError(const QString& message, int status = 0, const QString& url = "", const QMap<QString, QVariant>& data = {}, bool isAbort = false, const std::exception_ptr& originalError = nullptr)
        : std::runtime_error(message.toStdString()), m_url(url), m_status(status), m_data(data), m_isAbort(isAbort), m_originalError(originalError) {}

    QString url() const { return m_url; }
    void setUrl(const QString& url) { m_url = url; }

    int status() const { return m_status; }
    int setStatus(const int& nStatus) { return m_status = nStatus; }

    QMap<QString, QVariant> data() const { return m_data; }
    void getData(const QMap<QString, QVariant>& nData)  { m_data=nData; }

    bool isAbort() const { return m_isAbort; }
    void setIsAbort(const bool& nIsAbort)  { m_isAbort = nIsAbort; }

    std::exception_ptr originalError() const { return m_originalError; }

private:
    QString m_url;
    int m_status;
    QMap<QString, QVariant> m_data;
    bool m_isAbort;
    std::exception_ptr m_originalError;
};

#endif // CLIENTRESPONSEERROR_H
