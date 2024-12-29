#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include "dao/usermanager.h"

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private:
    Ui::LoginView *ui;
    // 当界面构建之后需要初始化成员
    //管理数据的成员
    UserManager * userManager;
    //数据条目渲染的成员 qt
    QStandardItemModel *standardItemModel;
};

#endif // LOGINVIEW_H
