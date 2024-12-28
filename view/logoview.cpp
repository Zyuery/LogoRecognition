#include "logoview.h"
#include "ui_logoview.h"
#include "dbmanager.h"
#include <QSqlDatabase>
#include <QDebug>

logoView::logoView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logoView),
    pageNow(1),
    pageSize(10)
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
// 给 logo 的管理对象数据库链接信息
    this->logomanager = new logoManager(dbmng->getDb());
    //使用logoModel对象加载logo数据
    QList<logoModel> logolist = logomanager->getAll();
    //获取总页数
    totalPage = (logolist.count() + pageSize - 1) / pageSize;
    this->standardItemModel = new QStandardItemModel(this);
    this->standardItemModel->setColumnCount(5);
    this->standardItemModel->setHorizontalHeaderLabels(QStringList() << "id" << "logoName" << "置信度" <<"查询来源" <<"查询时间");
    //设置pageLabel内容
    QLabel *pagelabel=ui->pageLabel;
    pagelabel->setText(QString::number(pageNow) + "/" + QString::number(totalPage));
//监听返回主页按钮
    QPushButton *homeButton=ui->homeButton;
    MainWindow* mainWindow =new MainWindow();
    connect(homeButton,&QPushButton::clicked,this,[=](){
        // 显示第二个窗口
        mainWindow->show();
        // 隐藏当前窗口
        this->hide();
    });

//监听输入框
    QLineEdit *keywordEdit = ui->keywordEdit;
    connect(keywordEdit, &QLineEdit::textChanged, this, [=] {
        keyword = keywordEdit->text();
    });

//监听查询按钮
    QPushButton * queryButton =ui->queryButton;
    connect(queryButton,&QPushButton::clicked,this,[=](){
        // 清空现有数据
        standardItemModel->removeRows(0, standardItemModel->rowCount());
        // 调用分页查询
        QList<logoModel> logolist = logomanager->getlogoList(keyword, pageNow, pageSize);
        // 将查询到的 logo 数据存进数据模型中
        foreach(logoModel model, logolist){
            QStandardItem *item1  = new QStandardItem(QString::number(model.getId()));
            QStandardItem *item2 = new QStandardItem(model.getLogoName());
            QStandardItem *item3 = new QStandardItem(QString::number( model.getProbability()));
            QStandardItem *item4 = new QStandardItem(model.getImageOrigin());
            QStandardItem *item5 = new QStandardItem(model.getRecognitionTime());
            // 将四个单元格的数据存入一行
            standardItemModel->appendRow(QList<QStandardItem*>() << item1 << item2 << item3 << item4 << item5);
        }
        // 将 standardItemModel 渲染到 tableView 中
        ui->tableView->setModel(standardItemModel);
        // 让单元格自适应
        ui->tableView->resizeColumnsToContents();
        qDebug() << "成功查询" << totalPage;
    });
//监听上一页按钮
    QPushButton * pageUpButton =ui->pageUpButton;
    connect(pageUpButton,&QPushButton::clicked,this,[=](){
           if(pageNow>1){
                pageNow--;
                pagelabel->setText(QString::number(pageNow) + "/" + QString::number(totalPage));
                // 清空现有数据
                standardItemModel->removeRows(0, standardItemModel->rowCount());
                // 调用分页查询
                QList<logoModel> logolist = logomanager->getlogoList(keyword, pageNow, pageSize);
                // 将查询到的 logo 数据存进数据模型中
                foreach(logoModel model, logolist){
                    QStandardItem *item1  = new QStandardItem(QString::number(model.getId()));
                    QStandardItem *item2 = new QStandardItem(model.getLogoName());
                    QStandardItem *item3 = new QStandardItem(QString::number( model.getProbability()));
                    QStandardItem *item4 = new QStandardItem(model.getImageOrigin());
                    QStandardItem *item5 = new QStandardItem(model.getRecognitionTime());
                    // 将四个单元格的数据存入一行
                    standardItemModel->appendRow(QList<QStandardItem*>() << item1 << item2 << item3 << item4 << item5);
                }
                // 将 standardItemModel 渲染到 tableView 中
                ui->tableView->setModel(standardItemModel);
                // 让单元格自适应
                ui->tableView->resizeColumnsToContents();
                qDebug() << "成功查询" << totalPage;
           }
    });

//监听下一页按钮
    QPushButton * pageDownButton =ui->pageDownButton;
    connect(pageDownButton,&QPushButton::clicked,this,[=](){
            if(pageNow<totalPage){
                pageNow++;
                pagelabel->setText(QString::number(pageNow) + "/" + QString::number(totalPage));
                // 清空现有数据
                standardItemModel->removeRows(0, standardItemModel->rowCount());
                // 调用分页查询
                QList<logoModel> logolist = logomanager->getlogoList(keyword, pageNow, pageSize);
                // 将查询到的 logo 数据存进数据模型中
                foreach(logoModel model, logolist){
                    QStandardItem *item1  = new QStandardItem(QString::number(model.getId()));
                    QStandardItem *item2 = new QStandardItem(model.getLogoName());
                    QStandardItem *item3 = new QStandardItem(QString::number( model.getProbability()));
                    QStandardItem *item4 = new QStandardItem(model.getImageOrigin());
                    QStandardItem *item5 = new QStandardItem(model.getRecognitionTime());
                    // 将四个单元格的数据存入一行
                    standardItemModel->appendRow(QList<QStandardItem*>() << item1 << item2 << item3 << item4 << item5);
                }
                // 将 standardItemModel 渲染到 tableView 中
                ui->tableView->setModel(standardItemModel);
                // 让单元格自适应
                ui->tableView->resizeColumnsToContents();
                qDebug() << "成功查询" << totalPage;
            }
    });
}


//析构
logoView::~logoView() {
    delete logomanager;
    delete ui;
}
