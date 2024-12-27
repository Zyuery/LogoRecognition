#include "logomanager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

logoManager::logoManager(QSqlDatabase sdb):db(sdb){}

int logoManager::getPageNow(){
    return this->pageNow;
}
//设置当前页
void logoManager::setPageNow(const int &pageNow){
    this->pageNow = pageNow;
}

int logoManager::getTotalPage(){
    return this->totalPage;
}
//查找数据库中的logo，将查到的数据放进一个QList列表里
QList<logoModel> logoManager::getlogoList(const QString &keyword,const int type)
{
        //xxx来实现
}
void logoManager::putlogoList(QList<logoModel> &)
{
    //xxx来实现
}
