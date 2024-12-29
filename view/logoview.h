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
    explicit logoView(QWidget *parent = nullptr);
    ~logoView();
    void showDataList(const int  type);

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
};

#endif // LOGOVIEW_H
