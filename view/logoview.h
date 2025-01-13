#ifndef LOGOVIEW_H
#define LOGOVIEW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include "dao/logomanager.h"
#include "mainwindow.h"

namespace Ui {
class logoView;
}

class logoView : public QMainWindow
{
    Q_OBJECT

public:
    explicit logoView(QWidget *parent = nullptr,QString username="admin");
    ~logoView();
    //username的getter和Setter 方法
    void setUsername(const QString& newUsername) {
        username = newUsername;
    }
    QString getUsername() const {
        return username;
    }

private:
    Ui::logoView *ui;
    // 当界面构建之后需要初始化成员
    //管理数据的成员
    logoManager * logomanager;
    //数据条目渲染的成员 qt
    QStandardItemModel *standardItemModel;
    int pageNow;
    int pageSize;
    int totalPage;
    QString keyword;
    int id;
    QString username;//当前登录的用户
};

#endif // LOGOVIEW_H
