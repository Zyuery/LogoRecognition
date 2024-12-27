#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QObject>
#include <QDebug>
#include <QRadioButton>
#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //测试ssl连接
    void testSSL()
    {
        QSslSocket sslSocket;
        sslSocket.connectToHostEncrypted("www.example.com", 443);

        if (sslSocket.waitForEncrypted()) {
            qDebug() << "SSL connection successful!";
        } else {
            qDebug() << "SSL connection failed: " << sslSocket.errorString();
        }
    }
public slots:
    // 处理请求响应的槽函数
    void onFinished();




private:
    Ui::MainWindow *ui;
//请求参数
    //参数
    QString access_token;
    //Headers
    QString content_type;
    //Body
    QString url;
    QImage image;
//辅助属性
    int method;//method==0为image传参，method==1为url传参
    QNetworkAccessManager *manager;  // 网络管理器
    QNetworkReply *reply;           // 网络请求的回复指针
    QString urlEncodedBase64;       //传入的图片的base64编码

};
#endif // MAINWINDOW_H

