#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QScreen>
#include <QThread>

namespace Ui {
    class SetupWindow;
}

class SetupWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SetupWindow(QWidget *parent = 0);
    ~SetupWindow();

    void winDisplay();
    void winClose();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SetupWindow *ui;
};

#endif // SETUPWINDOW_H
