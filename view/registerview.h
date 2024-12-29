#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include "dao/usermanager.h"

namespace Ui {
class RegisterView;
}

class RegisterView : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterView(QWidget *parent = nullptr);
    ~RegisterView();

private:
    Ui::RegisterView *ui;
    //管理数据的成员
    UserManager * userManager;
    //数据条目渲染的成员
    QStandardItemModel *standardItemModel;
};

#endif // REGISTERVIEW_H
