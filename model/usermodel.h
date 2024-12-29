#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>

class UserModel
{
public:
    //无参构造
    UserModel();
    //有参构造
    UserModel( const int &id,const QString &username ,const QString &password);

    // 设置和获取成员变量的公有方法
    void setId(const int &id);
    int getId() const;

    void setUsername(const QString &username);
    QString getUsername() const;

    void setPassword(const QString &password);
    QString getPassword()const;


private:
    int id;
    QString username;
    QString password;
};

#endif // USERMODEL_H
