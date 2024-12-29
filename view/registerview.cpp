#include "registerview.h"
#include "ui_registerview.h"
#include "dbmanager.h"
#include "loginview.h"
#include <QDebug>
#include <QMessageBox>

RegisterView::RegisterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterView)
{
    ui->setupUi(this);
    // 链接数据库
        DBManager* dbmng = new DBManager();
        bool dbOpened = dbmng->open("D:\\qtProject\\TeamDemo\\project\\untitled\\sql\\demo.db");
        if (dbOpened) {
            qDebug() << "Database opened successfully!";
        } else {
            qDebug() << "Failed to open database!";
        }
    this->userManager=new UserManager(dbmng->getDb());
    //初始化页面对象
    QLineEdit *usernameEdit=ui->usernameEdit;
    QLineEdit *passwordEdit=ui->passwordEdit;
    QLineEdit * confirmEdit =ui->confirmEdit;

    // 监听注册按钮
    QPushButton *registerButton = ui->registerButton;
    connect(registerButton, &QPushButton::clicked, this, [=]{
        // 获取用户输入的用户名和密码
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        QString confirmPassword = confirmEdit->text();
        // 输入验证
        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "用户名或密码不能为空！");
            return;
        }
        // 检查用户名和密码长度是否大于等于 8
        if (username.length() < 8) {
            QMessageBox::warning(this, "输入错误", "用户名必须至少包含 8 个字符！");
            return;
        }
        if (password.length() < 8) {
            QMessageBox::warning(this, "输入错误", "密码必须至少包含 8 个字符！");
            return;
        }

        if (password != confirmPassword) {
            QMessageBox::warning(this, "输入错误", "密码与确认密码不一致！");
            return;
        }
        // 调用 UserManager 的 putUserByUsernameAndPassword 方法注册新用户
        UserModel newUser = userManager->putUserByUsernameAndPassword(username, password);
        // 如果返回的 UserModel 是有效的
        if (newUser.getId() != 0) {
            QMessageBox::information(this, "注册成功", "用户注册成功！");
            LoginView *login = new LoginView();
            login->setWindowTitle("登录页面");
            login->show();
            this->close();
        } else {
            QMessageBox::critical(this, "注册失败", "用户注册失败，请稍后再试！");
        }
    });


//监听返回主页图标
    QPushButton *homeButton = ui->homeButton;
     connect(homeButton,&QPushButton::clicked,this,[=]{
        LoginView *login = new LoginView();
        login->setWindowTitle("登录页面");
        login->setFixedSize(1300,750);
        login->show();
        this->close();
     });
}

RegisterView::~RegisterView()
{
    delete ui;
}
