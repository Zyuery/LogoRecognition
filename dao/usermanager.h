#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QList>
#include <QSqlDatabase>
#include "model/usermodel.h"

class UserManager
{
public:
    UserManager(QSqlDatabase db);

//增
    UserModel putUserByUsernameAndPassword(QString username,QString password);

//查
    UserModel getUserByUsernameAndPassword(QString username,QString password);

private:
    //准备数据库访问对象
    QSqlDatabase db;

};

#endif // USERMANAGER_H
