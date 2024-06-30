#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QSystemTrayIcon>

namespace Ui{
    class SysTray;
};

class QAction;
class QMenu;
class QActionGroup;
class QIcon;

//class MainWindow;

class Systray : public QObject
{
    Q_OBJECT

public:
    Systray(QObject *parent = 0);
    ~Systray();

    void show();

    //QList<MainWindow *> mainWindowList;

protected:
    //void timerEvent(QTimerEvent *event);

private slots:
    //void updateToolTip(int i);
    void updateToolTip();
    //void updateTransferCount(int);

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void setup();
    void about();
    void quit();

private:
    void createActions();
    void createLeaveMenu();
    void createMenus();
    void createIconSet();
    void createConnections();

    void createMainWindow();
    void destroyMainWindowList();
    void visibleAllMainWindow();

    //QAction *showTransferFileAction;
    //QAction *delAckWindowAction;
    QAction *topWindowAction;
    QAction *setupAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *readMsgLogAction;
    QMenu *leaveMenu;
    QAction *quitAction;

    QActionGroup *leaveActionGroup;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QString m_iconKey;
};

#endif // !SYSTRAY_H

