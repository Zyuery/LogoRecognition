#include "mainwindow.h"
#include "ui_mainwindow.h"  // 包含由 uic 生成的 UI 头文件
#include "dbmanager.h"
#include "model/logomodel.h"
#include "dao/logomanager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
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
#include <QDateTime>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,method(-1)

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
        if(method==0){
            fileName = QFileDialog::getOpenFileName(this, "选择图片", QString(), "Images (*.png *.jpg *.bmp)");
            QString mimeType = QFileInfo(fileName).suffix().toLower();
            QImage image(fileName);
            if(fileName!=NULL){
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
            }
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

    //监听数据存储请求
    QPushButton *inserbutton = ui->insertButton;
    connect(inserbutton,&QPushButton::clicked,this,[=]{
         // 链接数据库
         DBManager* dbmng = new DBManager();
         dbmng->open("D:\\qtProject\\TeamDemo\\project\\untitled\\sql\\demo.db");
         if(dbmng->open("D:\\qtProject\\TeamDemo\\project\\untitled\\sql\\demo.db"))
         this->logomanager = new logoManager(dbmng->getDb());
         logomanager->putLogoList(logolist);
         if(logolist.isEmpty()){
             QMessageBox::warning(this, "警告", "列表容器中无数据");
         }
         else{
             QMessageBox::information(this, "成功", "数据存储成功！");
         }
    });

    //监听页面跳转请求
    QPushButton *logoViewbutton = ui->logoViewButton;
    connect(logoViewbutton,&QPushButton::clicked,this,[=]{
        // 显示第二个窗口
        logoView* logoview = new logoView();
        logoview->setWindowTitle("dbPresentation");
        logoview->setFixedSize(1300,750);
        logoview->show();
        // 隐藏当前窗口
        this->close();
    });


}
//析构
MainWindow::~MainWindow()
{
    delete ui;
//    delete logoview;
}
//图片转base64
QString MainWindow::QImageToBase64(const QImage &image,QString mimeType) {
    // 创建一个QByteArray和QBuffer来存储图像的二进制数据
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    // 根据mimeType决定图像保存的格式
    QString format = mimeType.toLower();
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
    imagelabel->setScaledContents(true);
}

//时间戳处理
QString timestampToString(qint64 timestampSeconds) {
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestampSeconds);
    return dateTime.toString("yyyy-MM-dd HH:mm:ss");  // 可以自定义想要的日期时间格式
}


//三、处理响应数据
void MainWindow::onFinished()
{

    if (reply) {
        QByteArray responseData = reply->readAll();  // 获取响应内容
//        qDebug() << "Response:" << responseData;

        // 处理 JSON 响应（如果需要）
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull()) {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "JSON Response:" << jsonObj;
            // 获取result_num和result数组
            int resultNum = jsonObj.value("result_num").toInt();
            if (resultNum<=0){
                QMessageBox::warning(this, "输入错误", "未检测到图片中有logo信息", QMessageBox::Ok);
            }
            QJsonArray resultArray = jsonObj.value("result").toArray();

            // 清空当前的logoList
            logolist.clear();

            // 遍历result数组，解析每个logo信息并存储到logoList中
            for (int i = 0; i < resultNum; ++i) {
                QJsonObject logoObj = resultArray[i].toObject();
                QString name = logoObj.value("name").toString();
                int type = logoObj.value("type").toInt();
                double probability = logoObj.value("probability").toDouble();
                QJsonObject locationObj = logoObj.value("location").toObject();
                int left = locationObj.value("left").toInt();
                int top = locationObj.value("top").toInt();
                int width = locationObj.value("width").toInt();
                int height = locationObj.value("height").toInt();


                // 创建Logo对象并添加到logoList中
                logoModel logo;
                logo.setLogoName(name);
                logo.setType(type);
                logo.setProbability(probability);
                logo.setLeftPosition(left);
                logo.setTopPosition(top);
                logo.setWidth(width);
                logo.setHeight(height);
                logo.setImageOrigin(method==0?fileName:url);

                // 获取当前时间的QDateTime对象
                QDateTime currentDateTime = QDateTime::currentDateTime();
                // 获取当前时间的秒时间戳
                qint64 secondTimestamp = currentDateTime.toSecsSinceEpoch();
                logo.setRecognitionTime (timestampToString(secondTimestamp));

                //加入列表
                logolist.append(logo);
                this->standardItemModel = new QStandardItemModel(this);
                this->standardItemModel->setColumnCount(3);
                this->standardItemModel->setHorizontalHeaderLabels(QStringList()<<"logoName"<<"置信度"<<"查询时间"<<"查询来源");
                //将查询到的logo数据存进数据模型中
                foreach(logoModel model,logolist){
                     QStandardItem *item1 = new QStandardItem(model.getLogoName());
                     QStandardItem *item2 = new QStandardItem(QString::number( model.getProbability()));
                     QStandardItem *item3 = new QStandardItem(model.getRecognitionTime());
                     QStandardItem *item4 = new QStandardItem(model.getImageOrigin());
                     //将四个单元格的数据存入一行
                     standardItemModel->appendRow(QList<QStandardItem*>()<<item1<<item2<<item3<<item4);
                }
                QTableView *tableView =ui->tableView;
                //将standardItemModel渲染到tableView中
                tableView->setModel(standardItemModel);
                //让单元格自适应
                tableView->resizeColumnsToContents();
            }
        } else {
            qDebug() << "无效json响应";
            QMessageBox::warning(this, "输入错误", "获得了无效json响应，请检查图片是否满足接口规范", QMessageBox::Ok);
        }
        reply->deleteLater();  // 在不需要时删除reply对象
        reply = nullptr;  // 清空reply指针
    }
}

