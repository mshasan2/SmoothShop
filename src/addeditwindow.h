#ifndef ADDEDITWINDOW_H
#define ADDEDITWINDOW_H

#include <QMainWindow>

namespace Ui {
class AddEditWindow;
}

class AddEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddEditWindow(QWidget *parent = nullptr);
    ~AddEditWindow();

private slots:
    // Slot for adding an item
    void on_pushButton_addItem_clicked();

    // Slot for editing an item
    void on_pushButton_editItem_clicked();

private:
    Ui::AddEditWindow *ui;
};

#endif // ADDEDITWINDOW_H
