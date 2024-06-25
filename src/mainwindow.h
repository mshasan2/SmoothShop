#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/landingpage.h"
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include <QStandardPaths>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Database connection
    QSqlDatabase mydb;

    // Open database connection
    bool connOpen() {

        //mydb = QSqlDatabase::addDatabase("QSQLITE");
        //mydb.setDatabaseName("/Users/msadathhasan/Projects/qt/Supermarket_Checkout/src/db/SupermarketDB.db");
        //mydb.setDatabaseName("://src/db/SupermarketDB.db");

        mydb = QSqlDatabase::addDatabase("QSQLITE");

        QString currentPath = QDir::currentPath();
        QString basePath;
        int buildIndex = currentPath.indexOf("/build");
        basePath = currentPath.left(buildIndex);

        QString dbPath = basePath + "/src/db/SupermarketDB.db";
        mydb.setDatabaseName(dbPath);

        if(!mydb.open()) {
            qDebug() << "Error: Failed to open database." << mydb.lastError();
            return false;
        }
        else{
            return true;
        }
    };

    // Close database connection
    void connClose(){
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
        qDebug() << "Closed db";
    }



private slots:

    // Handle login button click
    void on_pushButton_Login_clicked();

private:
    Ui::MainWindow *ui;
    landingPage *landingPageWindow;


};
#endif // MAINWINDOW_H
