#ifndef CHECKOUTWINDOW_H
#define CHECKOUTWINDOW_H

#include <QMainWindow>
#include <QDateTime>

namespace Ui {
class CheckOutWindow;
}

class CheckOutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CheckOutWindow(QWidget *parent = nullptr, const QString &cashierName = QString());
    ~CheckOutWindow();

private slots:
    void on_pushButton_addItem_clicked();

    void on_pushButton_removeItem_clicked();

    void on_pushButton_generateReceipt_clicked();

private:
    Ui::CheckOutWindow *ui;
    QString cashierName;
    QDateTime currentDateTime;
    void updateTotals();
    void generatePDFReceipt();
    QSet<QString> dealSet; // Set of items eligible for the second type of deal
};

#endif // CHECKOUTWINDOW_H
