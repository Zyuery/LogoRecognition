#ifndef LOGOMANAGER_H
#define LOGOMANAGER_H
#include <QList>
#include <QSqlDatabase>
#include "model/logomodel.h"

class logoManager
{
public:
    logoManager(QSqlDatabase db);
    //需要包含操作logo表的方法

    //查
    QList<logoModel> getlogoList(const QString &keyword);//查询logo列表
    QList<logoModel> getAll();
    //增
    void putlogoList(QList<logoModel> &);//添加logo进数据库

private:
    //准备数据库访问对象
    QSqlDatabase db;
};

#endif // LOGOMANAGER_H
