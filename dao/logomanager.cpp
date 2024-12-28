#include "logomanager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

logoManager::logoManager(QSqlDatabase sdb):db(sdb){}

//查：查找数据库中的logo，将查到的数据放进一个QList列表里
QList<logoModel> logoManager::getlogoList(const QString &keyword)

{
     QList<logoModel> logolist;
     QSqlQuery query(db);
     QString sql="SELECT * FROM logos WHERE logo_name LIKE %'"+keyword+"'%;";
     query.exec(sql);
    while(query.next()){
        //依次循环取出结果集
        int id = query.value("id").toInt();
        QString logoName = query.value("logo_name").toString();
        float probability = query.value("probability").toFloat();
        int leftPosition = query.value("left_position").toInt();
        int topPosition = query.value("top_position").toInt();
        int width = query.value("width").toInt();
        int height = query.value("height").toInt();
        QString imageOrigin = query.value("image_origin").toString();
        QString recognitionTime = query.value("recognition_time").toString();
        int type = query.value("type").toInt();
        // 使用构造函数将查询结果封装为 logoModel 对象
        logoModel logo(id, logoName, probability, leftPosition, topPosition,
                       width, height, imageOrigin, recognitionTime, type);
        // 将 logoModel 对象添加到 QList 中
        logolist.append(logo);
    }
    return logolist;
}


//查：获得所有列表
QList<logoModel> logoManager::getAll(){
    QList<logoModel> logolist;
    QSqlQuery query(db);
    QString sql="SELECT * FROM logos;";
    query.exec(sql);
    while(query.next()){
       //依次循环取出结果集
       int id = query.value("id").toInt();
       QString logoName = query.value("logo_name").toString();
       float probability = query.value("probability").toFloat();
       int leftPosition = query.value("left_position").toInt();
       int topPosition = query.value("top_position").toInt();
       int width = query.value("width").toInt();
       int height = query.value("height").toInt();
       QString imageOrigin = query.value("image_origin").toString();
       QString recognitionTime = query.value("recognition_time").toString();
       int type = query.value("type").toInt();
       // 使用构造函数将查询结果封装为 logoModel 对象
       logoModel logo(id, logoName, probability, leftPosition, topPosition,
                      width, height, imageOrigin, recognitionTime, type);
       // 将 logoModel 对象添加到 QList 中
       logolist.append(logo);
   }
   return logolist;
}



//增
void logoManager::putlogoList(QList<logoModel> &logolist)
{
    QSqlQuery query(db);
    // 开始事务
    if (!db.transaction()) {
        qDebug() << "事务启动失败: " << db.lastError().text();
        return;
    }
    // 遍历 logoList，插入每个 logo 数据
    for (const logoModel &logo : logolist) {
        // SQL 插入语句
        QString sql = "INSERT INTO logos (logo_name, probability, left_position, top_position, "
                      "width, height, image_origin, recognition_time, type) "
                      "VALUES (:logo_name, :probability, :left_position, :top_position, "
                      ":width, :height, :image_origin, :recognition_time, :type)";

        query.prepare(sql);  // 准备 SQL

        // 调试输出 SQL 语句
        qDebug() << "SQL: " << sql;

        // 绑定参数
        query.bindValue(":logo_name", logo.getLogoName());
        query.bindValue(":probability", logo.getProbability());
        query.bindValue(":left_position", logo.getLeftPosition());
        query.bindValue(":top_position", logo.getTopPosition());
        query.bindValue(":width", logo.getWidth());
        query.bindValue(":height", logo.getHeight());

        // 确保 image_origin 是字符串，如果为空或者是无效值，绑定为 NULL
        if (logo.getImageOrigin().isEmpty()) {
            query.bindValue(":image_origin", QVariant(QVariant::String)); // NULL 绑定
        } else {
            query.bindValue(":image_origin", logo.getImageOrigin());
        }

        query.bindValue(":recognition_time", logo.getRecognitionTime());
        query.bindValue(":type", logo.getType());

        // 输出调试信息，确保所有参数已绑定
        qDebug() << "logo_name: " << logo.getLogoName();
        qDebug() << "probability: " << logo.getProbability();
        qDebug() << "left_position: " << logo.getLeftPosition();
        qDebug() << "top_position: " << logo.getTopPosition();
        qDebug() << "width: " << logo.getWidth();
        qDebug() << "height: " << logo.getHeight();
        qDebug() << "image_origin: " << logo.getImageOrigin();
        qDebug() << "recognition_time: " << logo.getRecognitionTime();
        qDebug() << "type: " << logo.getType();

        // 执行 SQL 插入
        if (!query.exec()) {
            qDebug() << "插入失败: " << query.lastError().text();  // 输出错误信息
            db.rollback();  // 如果插入失败，回滚事务
            return;  // 插入失败时返回
        }
    }

    // 提交事务，确保数据一致性
    if (!db.commit()) {
        qDebug() << "提交事务失败: " << db.lastError().text();
    } else {
        qDebug() << "插入成功，事务已提交";
    }
}

