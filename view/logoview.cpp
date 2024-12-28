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
    totalPage=(logolist.count()%10==0)?(logolist.count()/pageSize+0):(logolist.count()/pageSize+1);

    this->standardItemModel = new QStandardItemModel(this);
    this->standardItemModel->setColumnCount(4);
    this->standardItemModel->setHorizontalHeaderLabels(QStringList() << "id" << "logoName" << "置信度" << "查询时间");

    // 将查询到的 logo 数据存进数据模型中
    foreach(logoModel model, logolist) {
         QStandardItem *item1  = new QStandardItem(model.getId());
         QStandardItem *item2 = new QStandardItem(model.getLogoName());
         QStandardItem *item3 = new QStandardItem(model.getProbability());
         QStandardItem *item4 = new QStandardItem(model.getRecognitionTime());
         // 将四个单元格的数据存入一行
         standardItemModel->appendRow(QList<QStandardItem*>() << item1 << item2 << item3 << item4);
    }
    // 将 standardItemModel 渲染到 tableView 中
    ui->tableView->setModel(standardItemModel);
    // 让单元格自适应
    ui->tableView->resizeColumnsToContents();


//监听返回主页按钮
    QPushButton *homeButton=ui->homeButton;
    MainWindow* mainWindow =new MainWindow();
    connect(homeButton,&QPushButton::clicked,this,[=](){
        // 显示第二个窗口
        mainWindow->show();
        // 隐藏当前窗口
        this->hide();
    });

//监听查询按钮
    QPushButton * queryButton =ui->queryButton;
    connect(queryButton,&QPushButton::clicked,this,[=](){

    });
//监听上一页按钮
    QPushButton * pageUpButton =ui->pageUpButton;
    connect(pageUpButton,&QPushButton::clicked,this,[=](){
           if(pageNow>1){
                pageNow--;
           }
    });

//监听下一页按钮
    QPushButton * pageDownButton =ui->pageDownButton;
    connect(pageDownButton,&QPushButton::clicked,this,[=](){
//            if(pageNow)
    });
}


//析构
logoView::~logoView() {
    delete logomanager;
    delete ui;
}
