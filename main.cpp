#include "mainwindow.h"
#include "view/loginview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginView login;
    login.setWindowTitle("登录页面");
    login.setFixedSize(1300,750);
    login.show();

//    MainWindow w;
//    w.setWindowTitle("LogoRecognition");
//    w.show();

    return a.exec();
}
