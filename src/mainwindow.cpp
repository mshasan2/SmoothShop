#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("://resources/images/logo_csf.jpeg");
    ui->label_pic->setPixmap(pix);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_Login_clicked()
{
    QString username, password;
    username = ui->lineEdit_UserName->text();
    password = ui->lineEdit_Password->text();

    connOpen();

    QSqlQuery qry;
    qry.prepare("select * from employeeinfo where username= :username and password= :password");
    qry.bindValue(":username",  username);
    qry.bindValue(":password", password);
    if(qry.exec()){
        int count = 0;
        while (qry.next()) {
            count++;
        }
        if(count==1) {
            ui->label_status->setText("Username and password are correct");
            this->hide();
            landingPageWindow = new landingPage(this, username);
            landingPageWindow->show();

        }
        if(count>1) {
            ui->label_status->setText("Username and password are incorrect");
        }
        if(count<1) {
            ui->label_status->setText("Username and password are incorrect");
        }
    }
    qry.clear();
    connClose();
}

