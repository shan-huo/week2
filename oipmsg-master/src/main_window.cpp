#include "main_window.h"
#include "ui_main_window.h"
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set initial position
    QScreen *screen=QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();
        move((screenWidth - width()) / 6*7, (screenHeight - height()) / 2);

    stdModel = new QStandardItemModel();

    winList.empty();
    buildConnection();
}

MainWindow::~MainWindow()
{
    delete stdModel;
    delete ui;
}

ChatWin *MainWindow::singleton(const User &userinfo)
{
    QString userid = userinfo.getHostName();
    if (winList.find(userid) != winList.end()) {
        qDebug () << "HasWin" << userinfo.getHostName();
        return winList[userid];
    } else {
        qDebug () << "NoWin"<< userinfo.getHostName();
        ChatWin *tmpWin = new ChatWin(userinfo);
        //QScopedPointer<ChatWin> tmpWin(new ChatWin(userinfo));

        winList.insert(userid, tmpWin);
        connect(winList[userid], SIGNAL(sendInfo(const User&, const QString&)),
                this, SLOT(sendMsg(const User&, const QString&)));
        return winList[userid];
    }
}

bool MainWindow::buildConnection()
{
    connect(ui->treeViewUser, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(userItemClicked(const QModelIndex&)));

    // on chat window closed

    //program quit
    connect(ui->toolButtonRefresh, SIGNAL(clicked()), this, SLOT(onToolButtonRefreshClicked()));
    connect(ui->toolButtonSetting, SIGNAL(clicked()), this, SLOT(onToolButtonSettingClicked()));
    connect(ui->toolButtonAbout, SIGNAL(clicked()), this, SLOT(onToolButtonAboutClicked()));
    connect(ui->toolButtonQuit, SIGNAL(clicked()), this, SLOT(onToolButtonQuitClicked()));
    return true;
}


void MainWindow::buildItems(const QHash <QString, User> &hostlist)
{
    // reset treeview
    ui->treeViewUser->reset();

    // hold a copy of userlist
    copyHostlist.empty();
    copyHostlist = hostlist;

    treeModelRow = 0;
    stdModel->clear();
    QString key;
    foreach (key, hostlist.keys()) {
        QStandardItem *userItem = new QStandardItem((QString("-%0       %1").
                                                    arg(hostlist[key].getNickName())).
                                                    arg(hostlist[key].getHostName()));
        stdModel->setItem(treeModelRow, userItem);
        treeModelRow++;
    }

    stdModel->setHorizontalHeaderItem(0, new QStandardItem("Username         Hostname"));
    ui->treeViewUser->setModel(stdModel);
}

void MainWindow::userItemClicked(const QModelIndex &index)
 {
    QStringList infolist = index.data().toString().split(' ');
    qDebug () << infolist;
    User tmpuser;
    //findUser(infolist.at(2), tmpuser);
    findUser(infolist.at(7), tmpuser);

    // new user windows...
    ChatWin *chatWin = singleton(tmpuser);
    chatWin->run();
}


bool MainWindow::findUser(const QString &hostname, User &user)
{
    QString key;
    foreach (key, copyHostlist.keys()) {
        if (!key.isEmpty() && hostname == copyHostlist[key].getHostName()) {
            user = copyHostlist[key];
            return true;
        }
    }
    return false;
}

void MainWindow::sendMsg(const User &userinfo, const QString &msg)
{
    emit sendInfo(userinfo.getHostAddress(), msg);
    User tmpuser;
    findUser(userinfo.getHostName(), tmpuser);

    ChatWin *chatWin = singleton(tmpuser);
    chatWin->userDlg->showMsg(msg);
}

void MainWindow::recvMsg(const QByteArray &packet)
{
    User tmpuser;

    // 从消息中提取用户标识符和消息内容
    QString message = QString::fromUtf8(packet);

    // 找到第一个冒号的位置
    int colonIndex = message.indexOf(':');
    if (colonIndex == -1) {
        // 如果没有找到冒号，则认为整个消息是用户标识符
        QString userIdentifier = message.trimmed();
        QString messageContent = ""; // 没有消息内容

        if (!findUser(userIdentifier.toUtf8(), tmpuser)) {
            // 未找到用户，忽略该消息
            return;
        }

        ChatWin *chatWin = singleton(tmpuser);
        chatWin->userDlg->showMsg(messageContent.toUtf8());
        chatWin->run();
    } else {
        // 如果找到了冒号，则分割出用户标识符和消息内容
        QString userIdentifier = message.left(colonIndex).trimmed();
        QString messageContent = message.mid(colonIndex + 1).trimmed();

        if (!findUser(userIdentifier.toUtf8(), tmpuser)) {
            // 未找到用户，忽略该消息
            return;
        }

        ChatWin *chatWin = singleton(tmpuser);
        chatWin->userDlg->showMsg(messageContent.toUtf8());
        chatWin->run();
    }
}



void MainWindow::onToolButtonRefreshClicked()
{
    //emit fresh
    qDebug () << "refresh()";
    emit refreshUser();
}

void MainWindow::onToolButtonSettingClicked()
{
    qDebug () << "about";
    //new setting windows
    SetupWindow *setupWin = new SetupWindow();
    setupWin->winDisplay();

}

void MainWindow::onToolButtonAboutClicked()
{
    //new about windows
    AboutDialog *aboutDlg = new AboutDialog();
    aboutDlg->dlgDisplay();
}

void MainWindow::onToolButtonQuitClicked()
{
    emit quitApp();
}
