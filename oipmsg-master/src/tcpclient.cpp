#include "tcpclient.h"

//从本地文件读取数据并通过 TCP 连接发送
Tcpclient::Tcpclient(const User &userinfo, const QString &filename)
{
    userDataLocal = userinfo;

    protoobj = new Protocol();
    port = protoobj->port;
    destIP = userDataLocal.getHostAddress();

    tcpClient = new QTcpSocket();
    localFile = new QFile(fileName);

    loadSize = 4*1024;
    TotalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;

}

Tcpclient::~Tcpclient()
{
    delete protoobj;
    delete tcpClient;
    delete localFile;
}

void Tcpclient::run()
{
    tcpClient->connectToHost(destIP, port);
}

bool Tcpclient::buildConnection()
{
    return true;
}

bool Tcpclient::startTransfer()
{
    if (!localFile->open(QFile::ReadOnly )) {
        return false;
    }

    TotalBytes = localFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);

    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    TotalBytes +=  outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<TotalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    bytesToWrite = TotalBytes - tcpClient->write(outBlock);
    qDebug()<<currentFile<<TotalBytes;
    outBlock.resize(0);

    return true;
}
