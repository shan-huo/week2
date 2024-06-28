#include "setup_window.h"
#include "ui_setup_window.h"

SetupWindow::SetupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SetupWindow)
{

    //set initial position
    QScreen *screen=QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();
        move((screenWidth - width()) / 2, (screenHeight - height()) / 2);
    ui->setupUi(this);
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::winDisplay()
{
    show();
}

void SetupWindow::winClose()
{
    show();
}

void SetupWindow::on_buttonBox_clicked(QAbstractButton *button)
{
   close();
}
