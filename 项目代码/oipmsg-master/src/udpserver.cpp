#include "udpserver.h"

Udpserver::Udpserver(quint16 &udpport)
{
    port = udpport;
    udpSocket = new QUdpSocket();
    protocolObj = new Protocol();
    buildConnection();
}

Udpserver::~Udpserver()
{
    delete udpSocket;
    delete protocolObj;
}

void Udpserver::run()
{
    if (!bindPort()) {
        qDebug("Can't bind port!");

        // change to new port or exit
        exit(-2);
    }

    // broadcast entry
    sendcmdBrEntry();
}


bool Udpserver::bindPort()
{
    if (!udpSocket->bind(port)) {
        QMessageBox *msgBox = new QMessageBox();
        connect(msgBox, SIGNAL(buttonClicked ),
                this, SLOT(exit(-1)));
        msgBox->setText(QObject::tr("Bind UDP Port Error!"));
        msgBox->exec();
        return false;
    }
    return true;
}

void Udpserver::dataReceived()
{
    QByteArray datagram;
    QHostAddress senderAddress;

    while (udpSocket->hasPendingDatagrams()) {
         datagram.resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &port);
         qDebug () << "Recv:" << senderAddress << datagram.data();
         handleCmd (senderAddress, datagram);
    }
}


bool Udpserver::buildConnection()
{

    // socket ready to read
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    // no operation
    connect(this, SIGNAL(signalNooperation(const QHostAddress&)),
            this, SLOT(processNooperation(const QHostAddress&)));
    // broadcast new user online
    connect(this, SIGNAL(signalBrEntry(const QHostAddress&, const QByteArray&)),
            this, SLOT(processBrEntry(const QHostAddress&, const QByteArray&)));
    connect(this, SIGNAL(signalBrExit(const QString&)),
            this, SLOT(processBrExit(const QString&)));
    connect(this, SIGNAL(signalAnsentry(const QHostAddress&, const QByteArray&)),
            this, SLOT(processAnsentry(const QHostAddress&, const QByteArray&)));
    connect(this, SIGNAL(signalSendmsg(const QHostAddress&,
                                       const QByteArray&)),
            this, SLOT(processSendmsg(const QHostAddress&,
                                      const QByteArray&)));

    return true;
}

// the main function the handle different packet
bool Udpserver::handleCmd (const QHostAddress &ipaddr, const QByteArray &newPacket)
{
    QList<QByteArray> argumentList = newPacket.split (':');

    quint32 cmd = argumentList.at(4).toInt() & 0x000000FF;

    switch (cmd) {
        case IPMSG_BR_ENTRY:
            emit signalBrEntry(ipaddr, newPacket);
            break;
        case IPMSG_BR_EXIT:
            emit signalBrExit(argumentList.at(2).data());
            break;
        case IPMSG_ANSENTRY:
            emit signalAnsentry(ipaddr, newPacket);
            break;
        case IPMSG_BR_ABSENCE:
            emit signalBrAbsence();
            break;
        case IPMSG_BR_ISGETLIST:
            emit signalBrIsgetlist();
            break;
        case IPMSG_OKGETLIST:
            emit signalOkgetlist();
            break;
        case IPMSG_GETLIST:
            emit signalGetlist();
            break;
        case IPMSG_ANSLIST:
            emit signalAnslist();
            break;
        case IPMSG_BR_ISGETLIST2:
            emit signalBrIsgetlist2();
            break;
        case IPMSG_SENDMSG:
            emit signalSendmsg(ipaddr, newPacket);
            break;
        case IPMSG_RECVMSG:
            emit signalRecvmsg(ipaddr, newPacket);
            break;
        case IPMSG_READMSG:
            emit signalReadmsg();
            break;
        case IPMSG_DELMSG:
            emit signalDelmsg();
            break;
        case IPMSG_ANSREADMSG:
            emit signalAnsreadmsg();
            break;
        case IPMSG_GETINFO:
            emit signalGetinfo();
            break;
        case IPMSG_SENDINFO:
            emit signalSendinfo();
            break;
        case IPMSG_GETABSENCEINFO:
            emit signalGetabsenceinfo();
            break;
        case IPMSG_SENDABSENCEINFO:
            emit signalSendinfo();
            break;
        case IPMSG_GETFILEDATA:
            emit signalGetfiledata();
            break;
        case IPMSG_RELEASEFILES:
            emit signalReleasefiles();
            break;
        case IPMSG_GETDIRFILES:
            emit signalGetdirfiles();
            break;
        case IPMSG_GETPUBKEY:
            emit signalGetpubkey();
            break;
        case IPMSG_ANSPUBKEY:
            emit signalAnspubkey();
            break;
        case IPMSG_NOOPERATION:
        default:
            emit signalNooperation(ipaddr);
            break;

    }
    return true;
}

//-------------------- process slots ------------------------
bool Udpserver::processNooperation(const QHostAddress &ipaddr)
{
    qDebug() << "Process nooperation" << ipaddr;
    return true;
}

bool Udpserver::processBrEntry(const QHostAddress &ipaddr,
                               const QByteArray &packet)
{
    qDebug() << "Process br entry" << ipaddr << packet;
    sendcmdAnsentry (ipaddr);
    return true;
}

bool Udpserver::processBrExit(const QString &username)
{

    qDebug() << "Process br exit" << username;
    return true;
}

bool Udpserver::processAnsentry(const QHostAddress &ipaddr,
                                const QByteArray &packet)
{

    qDebug() << "Process ans entry" << ipaddr
             << packet;
    return true;
}

bool Udpserver::processBrAbsence()
{

    return true;

}

bool Udpserver::processBrIsgetlist()
{
    return true;
}

bool Udpserver::processOkgetlist()
{

    return true;
}

bool Udpserver::processGetlist()
{

    return true;
}

bool Udpserver::processAnslist()
{

    return true;
}

bool Udpserver::processBrIsgetlist2()
{

    return true;
}

bool Udpserver::processSendmsg(const QHostAddress &ipaddr,
                               const QByteArray &packet)
{
    QList<QByteArray> argumentList = packet.split(':');
    sendcmdRecvmsg(ipaddr, argumentList.at(1));

    //emit a signal
    emit gotMsg (packet);
    return true;
}

bool Udpserver::processRecvmsg(const QHostAddress &ipaddr,
                               const QByteArray &packet)
{

    //sendcmdRecvmsg(ipaddr, packet);
    return true;
}

bool Udpserver::processReadmsg()
{

    return true;
}


bool Udpserver::processDelmsg()
{

    return true;
}

bool Udpserver::processAnsreadmsg()
{

    return true;
}

bool Udpserver::processGetinfo()
{

    return true;
}

bool Udpserver::processSendinfo()
{

    return true;
}

bool Udpserver::processGetabsenceinfo()
{

    return true;
}

bool Udpserver::processSendabsenceinfo()
{

    return true;
}

bool Udpserver::processGetfiledata()
{

    return true;
}

bool Udpserver::processReleasefiles()
{

    return true;
}

bool Udpserver::processGetdirfiles()
{

    return true;
}

bool Udpserver::processGetpubkey()
{

    return true;
}

bool Udpserver::processAnspubkey()
{

    return true;
}




// sokcet communication
bool Udpserver::sendcmdNooperation(const QHostAddress &ipaddr)
{
    qDebug() << "broad Nooperation";
    QByteArray datagram = protocolObj->buildcmdNooperation();
    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 ipaddr,
                                 port) != datagram.size()) {
        qDebug() << "Broad Entry Error!";
    }

    return true;
}

bool Udpserver::sendcmdBrEntry()
{
    qDebug() << "broad entry";
    QByteArray datagram = protocolObj->buildcmdBrEntry();
    qDebug () << datagram;
    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 port) != datagram.size()) {
        qDebug() << "Broad Entry Error!";
    }

    return true;
}

bool Udpserver::sendcmdBrExit()
{
    //sendcmdNooperation(protocolObj->absence_absence);
    sendcmdNooperation(QHostAddress::Broadcast);
    qDebug() << "broad exit";
    QByteArray datagram = protocolObj->buildcmdBrExit();
    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 port) != datagram.size()) {
        qDebug() << "Broad Exit Error!";
    }

    return true;
}

bool Udpserver::sendcmdAnsentry(const QHostAddress& ipaddr)
{
    qDebug() << "ans entry";
    QByteArray datagram = protocolObj->buildcmdAnsentry();
    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 ipaddr,
                                 port) != datagram.size()) {
        qDebug() << "Answer Entry Error!";
    }

    return true;
}

bool Udpserver::sendcmdBrAbsence()
{
    QByteArray cmdAbsence;
    return true;
}

bool Udpserver::sendcmdBrIsgetlist()
{
    QByteArray cmdIsgetlist;
    return true;
}

bool Udpserver::sendcmdOkgetlist()

{
    QByteArray cmdOkgetlist;
    return true;
}
bool Udpserver::sendcmdGetlist()
{
    QByteArray cmdGetlist;
    return true;
}

bool Udpserver::sendcmdAnslist()
{
    QByteArray cmdAnslist;
    return true;
}

bool Udpserver::sendcmdBrIsgetlist2()
{
    QByteArray cmdIsgetlist2;
    return true;
}

bool Udpserver::sendcmdSendmsg(const QHostAddress &ipaddr,
                               const QString &msg)
        //const User &userinfo,
{
    QByteArray datagram = protocolObj->buildcmdSendmsg(msg);
    qDebug () << "SendMsg" << datagram;

    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 ipaddr,
                                 2425) != datagram.size()) {
        qDebug() << "Send Msg Error!";
    }

    return true;
}

bool Udpserver::sendcmdRecvmsg(const QHostAddress &ipaddr,
                               const QString &packetno)
{
    QByteArray datagram = protocolObj->buildcmdRecvmsg(packetno);
    qDebug () << "SendRecvMsg" << datagram;

    if (udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 ipaddr,
                                 2425) != datagram.size()) {

    }
    return true;
}

bool Udpserver::sendcmdReadmsg()
{
    QByteArray cmdReadmsg;
    return true;
}

bool Udpserver::sendcmdDelmsg()
{
    QByteArray cmdDelmsg;
    return true;
}

bool Udpserver::sendcmdAnsreadmsg()
{
    QByteArray cmdAnsreadmsg;
    return true;
}

bool Udpserver::sendcmdGetinfo()
{
    QByteArray cmdGetinfo;
    return true;
}

bool Udpserver::sendcmdSendinfo()
{
    QByteArray cmdSendinfo;
    return true;
}

bool Udpserver::sendcmdGetabsenceinfo()
{
    QByteArray cmdGetabsenceinfo;
    return true;
}

bool Udpserver::sendcmdSendabsenceinfo()
{
    QByteArray cmdSendabsenceinfo;
    return true;
}

bool Udpserver::sendcmdGetfiledata()
{
    QByteArray cmdGetfiledata;
    return true;
}

bool Udpserver::sendcmdReleasefiles()
{
    QByteArray cmdReleasefiles;
    return true;
}

bool Udpserver::sendcmdGetdirfiles()
{
    QByteArray cmdGetdirfiles;
    return true;
}

bool Udpserver::sendcmdGetpubkey()
{
    QByteArray cmdGetpubkey;
    return true;
}

bool Udpserver::sendcmdAnspubkey()
{
    QByteArray cmdAspubkey();
    return true;
}
