#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslSocket>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //检查环境是否支持ssl
    if (QSslSocket::supportsSsl()) {
        qDebug() << "SSL is supported!";
    } else {
        qDebug() << "SSL is NOT supported!";
    }

    // 定义QNetworkAccessManager对象，保证后续发送请求
    manager = new QNetworkAccessManager(this);
    //设置access_token  &  content_type
    access_token = "24.16d450761fba636d66287b40a9c59b52.2592000.1737789418.282335-116852323";
    content_type = "application/x-www-form-urlencoded";
    //设置请求路由
    QUrl request_url("https://aip.baidubce.com/rest/2.0/image-classify/v2/logo?access_token="+access_token);
    QNetworkRequest request(request_url);
    // 设置 Content-Type 头
    request.setHeader(QNetworkRequest::ContentTypeHeader,content_type);

//  postData.addQueryItem("image", imageBase64); // Base64 编码后的图片数据


//一、决定传参方法
    //监听单选框
    QRadioButton *radioButton = ui->radioButton;
    connect(radioButton, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            qDebug() << "用户选择导入本地图片";
            this->method=0;
        }
    });
    QRadioButton *radioButton_2 = ui->radioButton_2;
    connect(radioButton_2, &QRadioButton::clicked, [=](bool checked) {
        if (checked) {
            qDebug() << "用户选择导入URL";
            this->method=1;
        }
    });

//二、监听发送请求

    //监听查询请求发送的按钮searchButton
    QPushButton *searchbutton = ui->searchButton;
    connect(searchbutton,&QPushButton::clicked,this,[=]{
        if(method==0){//传base64

        }
        else if(method==1){//传URL

                qDebug()<<"用户开始查询";
                // 设置 POST 请求的参数
                QUrlQuery postData;
                QString url = ui->urlEdit->text();
                postData.addQueryItem("url", url);
                QByteArray data = postData.toString(QUrl::FullyEncoded).toUtf8();// 转换为 QByteArray
                reply = manager->post(request, data);// 发送 POST 请求
                // 连接请求的 finished 信号，处理返回的数据
                connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
        }
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

//三、处理响应数据
void MainWindow::onFinished()
{
    if (reply) {
        QByteArray responseData = reply->readAll();  // 获取响应内容
        qDebug() << "Response:" << responseData;

        // 处理 JSON 响应（如果需要）
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull()) {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "JSON Response:" << jsonObj;
        } else {
            qDebug() << "无效json响应";
        }
        reply->deleteLater();  // 在不需要时删除 reply 对象
        reply = nullptr;  // 清空 reply 指针
    }
}



