#include "dbmanager.h"

QSqlDatabase DBManager::db = QSqlDatabase::addDatabase("QSQLITE");

bool DBManager::open(QString dbtype)
{
   db.setDatabaseName(dbtype);
   return db.open();
}

void DBManager::close()
{
   db.close();
}
QSqlDatabase DBManager:: getDb(){
    return db;
}
