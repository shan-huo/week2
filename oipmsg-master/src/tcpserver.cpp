#include <QByteArray>
#include <QMessageBox>
#include "tcpserver.h"

// TCP 服务器
Tcpserver::Tcpserver()
{
    protoobj = new Protocol();
    port = protoobj->port;
    tcpServer = new QTcpServer(this);

    if (tcpServer->isListening()) {
        qDebug() << "port was listend!";
        exit(-4);
    }

    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug () << "unable listen tcpserver";
        tcpServer->close();
        exit(-3);
    }
}

Tcpserver::~Tcpserver()
{
    delete tcpServer;
}


void Tcpserver::run()
{

}


void Tcpserver::buildConnection()
{

}


void Tcpserver::recvFile()
{

}

void Tcpserver::recvDir()
{

}
