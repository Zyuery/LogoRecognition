#include "usermodel.h"


// 无参构造函数，初始化成员变量为默认值
UserModel::UserModel()
    : id(0), username(""), password("") // 默认值可以根据需要调整
{
}

// 有参构造函数，允许初始化所有成员变量
UserModel::UserModel(const int &id, const QString &username, const QString &password)
    : id(id), username(username), password(password)
{
}

// 设置和获取id的公有方法
void UserModel::setId(const int &id) {
    this->id = id;
}

int UserModel::getId() const {
    return id;
}

// 设置和获取username的公有方法
void UserModel::setUsername(const QString &username) {
    this->username = username;
}

QString UserModel::getUsername() const {
    return username;
}

// 设置和获取password的公有方法
void UserModel::setPassword(const QString &password) {
    this->password = password;
}

QString UserModel::getPassword() const {
    return password;
}

