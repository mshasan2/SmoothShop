#include "checkoutwindow.h"
#include "ui_checkoutwindow.h"
#include "mainwindow.h"
#include <QPdfWriter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>


CheckOutWindow::CheckOutWindow(QWidget *parent, const QString &cashierName)
    : QMainWindow(parent)
    , ui(new Ui::CheckOutWindow)
    , cashierName(cashierName)
{
    ui->setupUi(this);
    currentDateTime = QDateTime::currentDateTime();
    ui->lineEdit_cashier->setText(cashierName);
    ui->lineEdit_date->setText(currentDateTime.date().toString("dd/MM/yyyy"));
    ui->lineEdit_time->setText(currentDateTime.time().toString("HH:mm:ss"));
    // Initialize dealSet with the items eligible for the second deal
    dealSet = {"1", "4"};
}

CheckOutWindow::~CheckOutWindow()
{
    delete ui;
}

void CheckOutWindow::on_pushButton_addItem_clicked()
{
    QString itemId = ui->lineEdit_addItem_iid->text();
    int quantity = ui->lineEdit_addItem_quantity->text().toInt();

    if (itemId.isEmpty() || quantity <= 0) {
        qDebug() << "Invalid item ID or quantity.";
        return;
    }

    MainWindow conn;
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM iteminfo WHERE iid = :iid");
    qry.bindValue(":iid", itemId);
    if (qry.exec() && qry.next()) {
        QString itemName = qry.value("item_name").toString();
        double itemPrice = qry.value("item_cost").toDouble();
        double itemDiscount = qry.value("item_discount").toDouble();

        int rowCount = ui->tableWidget_ItemsAdded->rowCount();
        ui->tableWidget_ItemsAdded->insertRow(rowCount);

        ui->tableWidget_ItemsAdded->setItem(rowCount, 0, new QTableWidgetItem(QString::number(rowCount + 1)));
        ui->tableWidget_ItemsAdded->setItem(rowCount, 1, new QTableWidgetItem(itemId));
        ui->tableWidget_ItemsAdded->setItem(rowCount, 2, new QTableWidgetItem(itemName));
        ui->tableWidget_ItemsAdded->setItem(rowCount, 3, new QTableWidgetItem(QString::number(itemPrice)));
        ui->tableWidget_ItemsAdded->setItem(rowCount, 4, new QTableWidgetItem(QString::number(itemDiscount)));
        ui->tableWidget_ItemsAdded->setItem(rowCount, 5, new QTableWidgetItem(QString::number(quantity)));

        updateTotals();
    } else {
        qDebug() << "Item not found or error: " << qry.lastError();
    }
    conn.connClose();

}

void CheckOutWindow::updateTotals()
{
    double priceSum = 0;
    double totalDiscount = 0;
    double finalTotal = 0;
    QMap<QString, int> itemCounts;
    QMap<QString, double> itemPrices;
    QMap<QString, double> itemDiscounts;
    QString dealsApplied = "";

    // Collect item data from the table
    for (int i = 0; i < ui->tableWidget_ItemsAdded->rowCount(); ++i) {
        QString itemId = ui->tableWidget_ItemsAdded->item(i, 1)->text();
        double itemPrice = ui->tableWidget_ItemsAdded->item(i, 3)->text().toDouble();
        double itemDiscount = ui->tableWidget_ItemsAdded->item(i, 4)->text().toDouble();
        int quantity = ui->tableWidget_ItemsAdded->item(i, 5)->text().toInt();

        itemCounts[itemId] += quantity;
        itemPrices[itemId] = itemPrice;
        itemDiscounts[itemId] = itemDiscount;

        priceSum += itemPrice * quantity;
    }

    // Apply "buy 3 identical items and pay for 2" deal
    double dealDiscount1 = 0;
    bool isDeal1Applied = false;
    QSet<QString> dealSet1 = {"1"}; //Set of item IDs for the deal
    for (auto it = itemCounts.constBegin(); it != itemCounts.constEnd(); ++it) {
        int quantity = it.value();
        if (dealSet1.contains(it.key())) {
            if (quantity >= 3) {
                int freeItems = quantity / 3;
                dealDiscount1 += freeItems * itemPrices[it.key()];

                // Discount should only be applied to the paid items
                totalDiscount += (quantity - freeItems) * itemDiscounts[it.key()];
                isDeal1Applied = true;
            }

        } else {
            totalDiscount += quantity * itemDiscounts[it.key()];
        }
    }

    // // Apply "buy 3 items from a set and get the cheapest free" deal
    QSet<QString> dealSet2 = {"3", "4"};  //Set of item IDs for the deal
    QVector<double> dealItemsPrices;
    for (auto it = itemCounts.constBegin(); it != itemCounts.constEnd(); ++it) {
        if (dealSet2.contains(it.key())) {
            for (int i = 0; i < it.value(); ++i) {
                dealItemsPrices.append((itemPrices[it.key()] - itemDiscounts[it.key()]));
            }
        }
    }

    double dealDiscount2 = 0;
    bool isDeal2Applied = false;
    if (dealItemsPrices.size() >= 3) {
        std::sort(dealItemsPrices.begin(), dealItemsPrices.end());
        int setsOfThree = dealItemsPrices.size() / 3;
        for (int i = 0; i < setsOfThree; ++i) {
            dealDiscount2 += dealItemsPrices[i];
            isDeal2Applied = true;
        }
    }

    // Calculate the final total
    finalTotal = priceSum - totalDiscount - dealDiscount1 - dealDiscount2;

    // Update the deals applied string
    if (isDeal1Applied) {
        dealsApplied += "Deal 1 ";
    }
    if (isDeal2Applied) {
        dealsApplied += "Deal 2";
    }

    // Update the UI with the calculated totals
    ui->lineEdit_priceSum->setText(QString::number(priceSum));
    ui->lineEdit_discountSum->setText(QString::number(totalDiscount));
    ui->lineEdit_dealDiscount->setText(QString::number(dealDiscount1 + dealDiscount2));
    ui->lineEdit_finalTotal->setText(QString::number(finalTotal));
    ui->lineEdit_DealApplied->setText(dealsApplied);
}



void CheckOutWindow::on_pushButton_removeItem_clicked()
{
    int index = ui->lineEdit_removeItem_index->text().toInt() - 1;
    if (index >= 0 && index < ui->tableWidget_ItemsAdded->rowCount()) {
        ui->tableWidget_ItemsAdded->removeRow(index);
        // Re-index remaining items
        for (int i = 0; i < ui->tableWidget_ItemsAdded->rowCount(); ++i) {
            ui->tableWidget_ItemsAdded->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        }
        updateTotals();
    } else {
        qDebug() << "Invalid item index.";
    }
}


void CheckOutWindow::on_pushButton_generateReceipt_clicked()
{
    generatePDFReceipt();
}

void CheckOutWindow::generatePDFReceipt()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Receipt", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QTextDocument document;
    QTextCursor cursor(&document);

    // Add the store name at the header of the receipt
    QTextCharFormat headerFormat;
    headerFormat.setFontPointSize(16);
    headerFormat.setFontWeight(QFont::Bold);
    cursor.insertText("Catapult Sports Food\n\n", headerFormat);

    // Add date and cashier info
    QTextCharFormat infoFormat;
    infoFormat.setFontPointSize(12);
    cursor.insertText("Date: " + QDateTime::currentDateTime().toString() + "\n", infoFormat);
    cursor.insertText("Cashier: " + cashierName + "\n\n", infoFormat);

    // Add items table header
    cursor.insertText("Items:\n", infoFormat);

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setCellPadding(4);
    tableFormat.setCellSpacing(0);

    QTextTable *table = cursor.insertTable(ui->tableWidget_ItemsAdded->rowCount() + 1, ui->tableWidget_ItemsAdded->columnCount(), tableFormat);

    // Insert header row
    for (int col = 0; col < ui->tableWidget_ItemsAdded->columnCount(); ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(ui->tableWidget_ItemsAdded->horizontalHeaderItem(col)->data(Qt::DisplayRole).toString());
    }

    // Insert items
    for (int row = 0; row < ui->tableWidget_ItemsAdded->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget_ItemsAdded->columnCount(); ++col) {
            QTextTableCell cell = table->cellAt(row + 1, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(ui->tableWidget_ItemsAdded->item(row, col)->text());
        }
    }

    // Move cursor outside the table
    cursor.movePosition(QTextCursor::End);

    // Add totals and deals applied at the end
    cursor.insertBlock();
    cursor.insertText("Total Price: " + ui->lineEdit_priceSum->text() + "\n", infoFormat);
    cursor.insertText("Total Discount: " + ui->lineEdit_discountSum->text() + "\n", infoFormat);
    cursor.insertText("Deals Applied: " + ui->lineEdit_DealApplied->text() + "\n", infoFormat);
    cursor.insertText("Deal Discount: " + ui->lineEdit_dealDiscount->text() + "\n", infoFormat);
    cursor.insertText("Final Total: " + ui->lineEdit_finalTotal->text() + "\n", infoFormat);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    document.print(&printer);

    QMessageBox::information(this, "Receipt Generated", "Receipt has been successfully generated as a PDF.");
}
