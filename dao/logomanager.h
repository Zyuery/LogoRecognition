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
    QList<logoModel> getLogoList(const QString &keyword);//模糊查找，辅助计算totalPage
    QList<logoModel> getLogoList(const QString &keyword,int pageNow,int pageSize);//分页查询
    QList<logoModel> getAll();//查询all
    //增
    void putLogoList(QList<logoModel> &);//添加logo进数据库

    //删
    bool deleteLogolist(int id);

private:
    //准备数据库访问对象
    QSqlDatabase db;
};

#endif // LOGOMANAGER_H
