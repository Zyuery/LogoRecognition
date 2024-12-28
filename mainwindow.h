#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "view/logoview.h"
#include "model/logomodel.h"
#include "dbmanager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSqlDatabase>
#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QObject>
#include <QDebug>
#include <QRadioButton>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QUrlQuery>
#include <QTextCodec>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString QImageToBase64(const QImage &image,QString mimeType);//image转base64
    void isSslSupported();//是否支持ssl链接
    void isFormatSupported(QString mimeType,QString fileName,QLabel* imagelabel);
public slots:
    // 处理请求响应的槽函数
    void onFinished();

private slots:
    void on_insertButton_clicked();

private:
    Ui::MainWindow *ui;
    //声明logolist
    QList<logoModel> logolist;
    logoView* logoview;
    logoManager* logomanager;


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
    QString fileName;

    //数据条目渲染的成员 qt
    QStandardItemModel *standardItemModel;
};
#endif // MAINWINDOW_H

