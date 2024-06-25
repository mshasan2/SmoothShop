#ifndef VIEWITEMSWINDOW_H
#define VIEWITEMSWINDOW_H

#include <QMainWindow>

namespace Ui {
class ViewItemsWindow;
}

class ViewItemsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewItemsWindow(QWidget *parent = nullptr);
    ~ViewItemsWindow();

private:
    Ui::ViewItemsWindow *ui;
};

#endif // VIEWITEMSWINDOW_H
