#include "loginview.h"
#include "ui_loginview.h"
#include <QLineEdit>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <dbmanager.h>
#include "mainwindow.h"
#include "registerview.h"

LoginView::LoginView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginView)
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
//监听登录按钮
    QPushButton *loginButton = ui->loginButton;
    connect(loginButton,&QPushButton::clicked,this,[=]{
        // 获取输入的用户名和密码
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        // 验证输入是否为空
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "用户名或密码不能为空");
            return;
        }
        // 调用 UserManager 类的查询方法来验证用户
        UserModel user = userManager->getUserByUsernameAndPassword(username, password);

        // 检查返回的用户是否有效
        if (user.getId() != 0) {
            // 登录成功，跳转到下一个界面或进行其他操作
            qDebug() << "登录成功，欢迎" << user.getUsername();
            // 例如：跳转到主页面
            MainWindow *mainWindow = new MainWindow(nullptr, username);
            mainWindow->setUsername(user.getUsername());
            mainWindow->setWindowTitle("LogoRecognition");
            mainWindow->show();
            this->close(); // 关闭当前登录窗口
        } else {
            // 登录失败，提示用户
            QMessageBox::warning(this, "登录失败", "用户名或密码错误");
        }
    });

//监听注册按钮
    QPushButton *registerButton = ui->registerButton;
    connect(registerButton,&QPushButton::clicked,this,[=]{
        RegisterView * registerView = new RegisterView();
        registerView->setWindowTitle("注册页面");
        registerView->setFixedSize(1300,750);
        registerView->show();
        this->close();
    });
}


LoginView::~LoginView()
{
    delete ui;
}



