#include "usermanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// 构造函数：初始化数据库对象
UserManager::UserManager(QSqlDatabase db)
    : db(db)
{
    // 检查数据库是否已打开
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
    }
}

// 增：通过用户名和密码插入用户信息
UserModel UserManager::putUserByUsernameAndPassword(QString username, QString password)
{
    UserModel addedUser;
    // 检查数据库连接
    if (!db.isOpen()) {
        qDebug() << "数据库未打开，无法插入数据";
        return addedUser;  // 如果数据库未打开，直接返回一个空的 UserModel
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        // 插入成功后获取插入的 ID 并更新到 UserModel
        addedUser.setUsername(username);
        addedUser.setPassword(password);
        addedUser.setId(query.lastInsertId().toInt());  // 获取插入的 ID
        return addedUser;  // 返回插入的用户模型
    } else {
        qDebug() << "插入用户失败：" << query.lastError().text();
        return UserModel();  // 插入失败返回一个空的 UserModel
    }
}

// 查：通过用户名和密码查询用户信息
UserModel UserManager::getUserByUsernameAndPassword(QString username, QString password)
{
    UserModel foundUser;
    // 检查数据库连接
    if (!db.isOpen()) {
        qDebug() << "数据库未打开，无法查询数据";
        return foundUser;  // 如果数据库未打开，返回一个空的 UserModel
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, username, password FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {  // 检查是否有结果
        foundUser.setId(query.value(0).toInt());
        foundUser.setUsername(query.value(1).toString());
        foundUser.setPassword(query.value(2).toString());
        return foundUser;  // 返回找到的用户
    } else {
        qDebug() << "查询用户失败：" << query.lastError().text();
        return UserModel();  // 查询失败返回一个空的 UserModel
    }
}
