#include "addeditwindow.h"
#include "ui_addeditwindow.h"
#include "mainwindow.h"


AddEditWindow::AddEditWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddEditWindow)
{
    ui->setupUi(this);
}

AddEditWindow::~AddEditWindow()
{
    delete ui;
}

// Slot for adding an item
void AddEditWindow::on_pushButton_addItem_clicked()
{
    QString itemID = ui->lineEdit_iid->text();
    QString itemName = ui->lineEdit_name->text();
    QString itemPrice = ui->lineEdit_price->text();
    QString itemDiscount = ui->lineEdit_discount->text();

    // Check for null or empty values
    if (itemID.isEmpty() || itemName.isEmpty() || itemPrice.isEmpty() || itemDiscount.isEmpty()) {
        qDebug() << "Error: All fields are required.";
        ui->statusbar->showMessage("Error: All fields are required.", 5000); // Show message for 5 seconds
        return;
    }

    // Check for valid price and discount values
    bool priceOk, discountOk;
    double price = itemPrice.toDouble(&priceOk);
    double discount = itemDiscount.toDouble(&discountOk);

    if (!priceOk || !discountOk) {
        qDebug() << "Error: Price and Discount must be valid numbers.";
        ui->statusbar->showMessage("Error: Price and Discount must be valid numbers.", 5000); // Show message for 5 seconds
        return;
    }



    MainWindow conn;

    if (conn.connOpen()) {
        QSqlQuery qry(conn.mydb);

        qry.prepare("INSERT INTO iteminfo (iid, item_name, item_cost, item_discount) VALUES (:iid, :item_name, :item_cost, :item_discount)");
        qry.bindValue(":iid", itemID);
        qry.bindValue(":item_name", itemName);
        qry.bindValue(":item_cost", price);
        qry.bindValue(":item_discount", discount);

        if (qry.exec()) {
            qDebug() << "Item added successfully.";
            ui->statusbar->showMessage("Item added successfully.", 5000); // Show message for 5 seconds
        } else {
            qDebug() << "Error adding item: " << qry.lastError();
            ui->statusbar->showMessage("Error adding item.", 5000); // Show message for 5 seconds
        }
        conn.connClose();
    }


}

// Slot for editing an item
void AddEditWindow::on_pushButton_editItem_clicked()
{
    QString itemID = ui->lineEdit_iid->text();
    QString itemName = ui->lineEdit_name->text();
    QString itemPrice = ui->lineEdit_price->text();
    QString itemDiscount = ui->lineEdit_discount->text();

    // Check for null or empty itemID
    if (itemID.isEmpty()) {
        qDebug() << "Error: Item ID is required.";
        ui->statusbar->showMessage("Error: Item ID is required.", 5000); // Show message for 5 seconds
        return;
    }

    // Check that at least one of the other fields is provided
    if (itemName.isEmpty() && itemPrice.isEmpty() && itemDiscount.isEmpty()) {
        qDebug() << "Error: At least one of Name, Price, or Discount is required.";
        ui->statusbar->showMessage("Error: At least one of Name, Price, or Discount is required.", 5000); // Show message for 5 seconds
        return;
    }

    // Check for valid price and discount values if provided
    bool priceOk = true, discountOk = true;
    double price = 0.0, discount = 0.0;

    if (!itemPrice.isEmpty()) {
        price = itemPrice.toDouble(&priceOk);
        if (!priceOk) {
            qDebug() << "Error: Price must be a valid number.";
            ui->statusbar->showMessage("Error: Price must be a valid number.", 5000); // Show message for 5 seconds
            return;
        }
    }

    if (!itemDiscount.isEmpty()) {
        discount = itemDiscount.toDouble(&discountOk);
        if (!discountOk) {
            qDebug() << "Error: Discount must be a valid number.";
            ui->statusbar->showMessage("Error: Discount must be a valid number.", 5000); // Show message for 5 seconds
            return;
        }
    }

    MainWindow conn;

    if (conn.connOpen()) {
        QSqlQuery qry;
        QString queryString = "UPDATE iteminfo SET ";
        bool first = true;

        if (!itemName.isEmpty()) {
            if (!first) queryString += ", ";
            queryString += "item_name = :item_name";
            first = false;
        }

        if (!itemPrice.isEmpty()) {
            if (!first) queryString += ", ";
            queryString += "item_cost = :item_cost";
            first = false;
        }

        if (!itemDiscount.isEmpty()) {
            if (!first) queryString += ", ";
            queryString += "item_discount = :item_discount";
            first = false;
        }

        queryString += " WHERE iid = :iid";

        qry.prepare(queryString);

        if (!itemName.isEmpty()) qry.bindValue(":item_name", itemName);
        if (!itemPrice.isEmpty()) qry.bindValue(":item_cost", price);
        if (!itemDiscount.isEmpty()) qry.bindValue(":item_discount", discount);
        qry.bindValue(":iid", itemID);

        if (qry.exec()) {
            qDebug() << "Item updated successfully.";
            ui->statusbar->showMessage("Item updated successfully.", 5000); // Show message for 5 seconds
        } else {
            qDebug() << "Error updating item: " << qry.lastError();
            ui->statusbar->showMessage("Error updating item.", 5000); // Show message for 5 seconds
        }
        conn.connClose();
    }

}

