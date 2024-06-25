#include "viewitemswindow.h"
#include "ui_viewitemswindow.h"
#include "mainwindow.h"

ViewItemsWindow::ViewItemsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ViewItemsWindow)
{
    ui->setupUi(this);

    MainWindow conn;
    QSqlQueryModel *model = new QSqlQueryModel();
    conn.connOpen();
    QSqlQuery qry(conn.mydb);
    qry.prepare("SELECT * FROM iteminfo");
    qry.exec();
    model->setQuery(std::move(qry));

    // Set custom header names
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Item No."));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Price"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Discount"));

    ui->tableView_Items->setModel(model);

    // Set custom column widths
    ui->tableView_Items->setColumnWidth(0, 50); // Set width for first column
    ui->tableView_Items->setColumnWidth(1, 200); // Set width for second column
    ui->tableView_Items->setColumnWidth(2, 50); // Set width for third column
    ui->tableView_Items->setColumnWidth(3, 80); // Set width for fourth column


    conn.connClose();

}

ViewItemsWindow::~ViewItemsWindow()
{
    delete ui;
}
