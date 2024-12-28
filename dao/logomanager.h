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

    QList<logoModel> getlogoList(const QString &keyword);//查询logo列表
    void putlogoList(QList<logoModel> &);//添加logo进数据库
    int  getPageNow();
    void setPageNow(const int &pageNow);
    int  getTotalPage();
private:
    //准备数据库访问对象
    QSqlDatabase db;
    int pageNow = 1;//当前页
    int pageSize = 10;//每页大小
    int totalPage = 0;
    QString kw;
};

#endif // LOGOMANAGER_H
