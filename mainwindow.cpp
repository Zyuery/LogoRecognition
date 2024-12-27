#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
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
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QtCore/QIODevice>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isSslSupported();//检查环境是否支持ssl


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

    //监听用户导入图片
    QPushButton *selectbutton = ui->selectButton;
    QLabel *imagelabel = ui->imageLabel;
    connect(selectbutton,&QPushButton::clicked,this,[=](){
        QString fileName = QFileDialog::getOpenFileName(this, "选择图片", QString(), "Images (*.png *.jpg *.bmp)");
        QString mimeType = QFileInfo(fileName).suffix().toLower();
        QImage image(fileName);
        //检查图片格式
        isFormatSupported(mimeType,fileName,imagelabel);


//图片编码操作
        //Base64编码
        QString base64String=QImageToBase64(image,mimeType);
        if (base64String.isEmpty()) {
            QMessageBox::warning(this, "错误", "Base64编码失败！");
            return;
        }
        // URL编码
         urlEncodedBase64 = QUrl::toPercentEncoding(base64String);
         if(urlEncodedBase64.isEmpty()){
             qDebug() << "urlEncodedBase64为空";
         }
    });


    //监听查询请求发送的按钮searchButton
    QPushButton *searchbutton = ui->searchButton;
    connect(searchbutton,&QPushButton::clicked,this,[=]{
        if(method==0){//传base64
                qDebug()<<"用户开始查询";
                // 设置 POST 请求的参数
                QUrlQuery postData;
                postData.addQueryItem("image", urlEncodedBase64);
//                qDebug()<<urlEncodedBase64;
                QByteArray data = postData.toString(QUrl::FullyEncoded).toUtf8();// 转换为 QByteArray
                reply = manager->post(request, data);// 发送 POST 请求
                // 连接请求的 finished 信号，处理返回的数据
                connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
        }
        else if(method==1){//传URL
                qDebug()<<"用户开始查询";
                // 设置 POST 请求的参数
                QUrlQuery postData;
                QString image_url = ui->urlEdit->text();
                //检验url格式*******
                postData.addQueryItem("url", image_url);
                QByteArray data = postData.toString(QUrl::FullyEncoded).toUtf8();// 转换为 QByteArray
                reply = manager->post(request, data);// 发送 POST 请求
                // 连接请求的 finished 信号，处理返回的数据
                connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
        }
        else{
            QMessageBox::critical(this, "错误提示","请选择传参方法！");
        }
    });
}
//析构
MainWindow::~MainWindow()
{
    delete ui;
}

//图片转base64
QString MainWindow::QImageToBase64(const QImage &image,QString mimeType) {
    // 创建一个QByteArray和QBuffer来存储图像的二进制数据
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    // 根据mimeType决定图像保存的格式
    QString format = mimeType.toLower(); // 统一转换为小写
    if (format == "png") {
        image.save(&buffer, "PNG");
        qDebug()<<"image后缀为"<<format;
    } else if (format == "jpg" || format == "jpeg") {
        image.save(&buffer, "JPEG");
        qDebug()<<"image后缀为"<<format;
    } else if (format == "bmp") {
        image.save(&buffer, "BMP");
        qDebug()<<"image后缀为"<<format;
    } else {
        // 默认保存为PNG格式
        image.save(&buffer, "PNG");
        qDebug()<<"image后缀为"<<format;
    }
    // 将二进制数据转换为Base64编码
    QByteArray base64Data = byteArray.toBase64();

    // 返回Base64编码的字符串
    return QString::fromUtf8(base64Data);
}
//检查环境是否支持ssl
void MainWindow::isSslSupported(){
    if (QSslSocket::supportsSsl()) {
        qDebug() << "SSL is supported!";
    } else {
        qDebug() << "SSL is NOT supported!";
    }
}
//检查图片格式是否正确
void MainWindow::isFormatSupported(QString mimeType,QString fileName,QLabel* imagelabel){
    if (!(mimeType == "png" || mimeType == "jpg" || mimeType == "bmp")) {
        QMessageBox::warning(this, "错误", "不支持的图片格式！请使用PNG、JPG或BMP格式");
        return;
    }

    if (fileName.isEmpty()) {
        QMessageBox::critical(this, "错误提示","文件不合法！");
        return;  // 用户取消选择，退出函数
    }

    //检查图片大小是否符合条件
    QFileInfo fileInfo(fileName);
    if (fileInfo.size() > 4 * 1024 * 1024) {  // 大小超过4MB
        QMessageBox::warning(this, "错误", "图片大小不能超过4MB");
        return;
    }

    // 加载图片
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::warning(this, "无效图片", "加载图片失败");
        return;  // 图片加载失败，退出函数
    }

    //检查图片大小是否符合规范
    int width = image.width();
    int height = image.height();
    if (width < 15 || height < 15) {
        QMessageBox::warning(this, "错误", "图片的最短边必须至少15px");
        return;
    }
    if (width > 4096 || height > 4096) {
        QMessageBox::warning(this, "错误", "图片的最长边不能超过4096px");
        return;
    }

    qDebug() << "图片文件路径：" << fileName;
    // 显示图片
    imagelabel->setPixmap(QPixmap::fromImage(image));
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



