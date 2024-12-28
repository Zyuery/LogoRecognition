#include "logoview.h"
#include "ui_logoview.h"
#include "dbmanager.h"
#include <QSqlDatabase>
#include <QDebug>
logoView::logoView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logoView)
{
    ui->setupUi(this);
    // 链接数据库
    DBManager* dbmng = new DBManager();
    dbmng->open("D:\\qtProject\\TeamDemo\\project\\untitled\\sql\\demo.db");
    //给logo的管理对象数据库链接信息
    this->logomanager = new logoManager(dbmng->getDb());
    //使用logo对象加载logo数据
    QList<logoModel> logolist = logomanager->getlogoList("");
    this->standardItemModel = new QStandardItemModel(this);
    this->standardItemModel->setColumnCount(4);
    this->standardItemModel->setHorizontalHeaderLabels(QStringList()<<"id"<<"logoName"<<"置信度"<<"时间");
    //将查询到的logo数据存进数据模型中
    foreach(logoModel model,logolist){
         QStandardItem *item1  = new QStandardItem(model.getId());
         QStandardItem *item2 = new QStandardItem(model.getLogoName());
         QStandardItem *item3 = new QStandardItem(model.getProbability());
         QStandardItem *item4 = new QStandardItem(model.getRecognitionTime());
         //将四个单元格的数据存入一行
         standardItemModel->appendRow(QList<QStandardItem*>()<<item1<<item2<<item3<<item4);
    }
    //将standardItemModel渲染到tableView中
    ui->tableView->setModel(standardItemModel);
    //让单元格自适应
    ui->tableView->resizeColumnsToContents();
}


logoView::~logoView()
{
    delete ui;
}
