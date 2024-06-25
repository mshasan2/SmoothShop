#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include "src/viewitemswindow.h"
#include "src/addeditwindow.h"
#include "src/checkoutwindow.h"


namespace Ui {
class landingPage;
}



class landingPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit landingPage(QWidget *parent = nullptr, const QString &cashierName = QString());
    ~landingPage();

private slots:

    void on_viewProductsButton_clicked();

    void on_addEditButton_clicked();

    void on_checkOutButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::landingPage *ui;
    QString cashierName;
    ViewItemsWindow *viewItemsWindow;
    AddEditWindow *addEditWindow;
    CheckOutWindow *checkOutWindow;

};

#endif // LANDINGPAGE_H
