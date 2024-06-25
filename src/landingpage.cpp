#include "landingpage.h"
#include "ui_landingpage.h"


landingPage::landingPage(QWidget *parent, const QString &cashierName)
    : QMainWindow(parent)
    , ui(new Ui::landingPage)
    , cashierName(cashierName)
{
    ui->setupUi(this);
}

landingPage::~landingPage()
{
    delete ui;
}


void landingPage::on_viewProductsButton_clicked()
{
    viewItemsWindow = new ViewItemsWindow(this);
    viewItemsWindow->show();

}


void landingPage::on_addEditButton_clicked()
{
    addEditWindow = new AddEditWindow(this);
    addEditWindow->show();

}


void landingPage::on_checkOutButton_clicked()
{
    checkOutWindow = new CheckOutWindow(this, cashierName);
    checkOutWindow->show();
}


void landingPage::on_quitButton_clicked()
{
    this->close();
}

