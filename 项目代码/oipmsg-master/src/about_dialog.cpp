#include "about_dialog.h"
#include "ui_about_dialog.h"

//对话框的实现
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();
        move((screenWidth - width()) / 2, (screenHeight - height()) / 2);
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::dlgDisplay()
{
    show();
}

void AboutDialog::dlgClose()
{
    close();
}

void AboutDialog::on_buttonBoxOk_clicked(QAbstractButton *button)
{
   close();
}
