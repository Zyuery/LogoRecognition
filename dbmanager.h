#ifndef DBMANAGER_H
 #define DBMANAGER_H

#include <QSqlDatabase>

class DBManager
 {
 public:
    //声明打开和关闭函数
    static bool open(QString dbtype);
    static void close();
    QSqlDatabase getDb();
 private:
    //声明静态对象
    static QSqlDatabase db;
 };

#endif // DBMANAGER_H
