#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QString>
#include "utils/basemodel.h"

class AdminModel : public BaseModel {
public:
    AdminModel(const QJsonObject& data)
        : BaseModel(data), avatar(0), email("") {
        load(data);
    }

    int getAvatar() const {
        return avatar;
    }

    void setAvatar(int avatar) {
        this->avatar = avatar;
    }

    QString getEmail() const {
        return email;
    }

    void setEmail(const QString& email) {
        this->email = email;
    }

    void load(const QJsonObject& data) {
        BaseModel::load(data);
        avatar = data.value("avatar").toInt();
        email = data.value("email").toString();
    }

private:
    int avatar;
    QString email;
};


#endif // ADMINMODEL_H
