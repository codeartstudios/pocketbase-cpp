#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QString>

#include "basemodel.h"

namespace pb {
class AdminModel : public BaseModel {
public:
    AdminModel(const QJsonObject& data)
        : BaseModel(data), m_avatar(0), m_email("") {
        load(data);
    }

    AdminModel(BaseModel* baseModel) {
        load(baseModel->data());
    }

    int getAvatar() const {
        return m_avatar;
    }

    void setAvatar(int avatar) {
        this->m_avatar = avatar;
    }

    QString getEmail() const {
        return m_email;
    }

    void setEmail(const QString& email) {
        this->m_email = email;
    }

    void load(const QJsonObject& data) {
        BaseModel::load(data);
        m_avatar = data.value("avatar").toInt();
        m_email = data.value("email").toString();
        m_data = data;
    }

private:
    int m_avatar;
    QString m_email;
};
}

#endif // ADMINMODEL_H
