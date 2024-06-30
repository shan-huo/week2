#include <QRegularExpression>
#include <QStringList>
#include <QProcess>
#include <QHostInfo>
#include <QTime>
#include <QNetworkInterface>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include "ipmsg.h"
#include "protocol.h"

Protocol::Protocol()
{
    port = IPMSG_DEFAULT_PORT;
    version = QString::number(IPMSG_VERSION, 10);

    QStringList envVariables;

    envVariables << "USER.*" << "HOSTNAME.*";

    //系统中关于环境变量的信息存在environment中
    QStringList environment = QProcess::systemEnvironment();

    QRegularExpression re("USER.*");
        int index = environment.indexOf(re);
    if (index != -1) {
        QStringList stringList = environment.at(index).split('=');
        if (stringList.size() == 2) {
            username = stringList.at(1);
        }
    }

    if (index != -1) {
        QStringList stringList = environment.at(index).split('=');
        if (stringList.size() == 2) {
            hostname = stringList.at(1);
        }
    }

    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces ();
        QNetworkInterface interface=list.at(1);
}

Protocol::~Protocol()
{

}

void Protocol::setAbsenceStatus(const quint32 &status)
{
    absenceStatus = status;
}

QByteArray Protocol::buildcmdNooperation()
{
    quint32 packetno;

    // Set seed value.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    packetno = rand() % 10240;

    quint64 cmd = 0;
    cmd = cmd | IPMSG_NOOPERATION;

    QByteArray cmd_nooperation;
    cmd_nooperation.append(QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_nooperation.append (":");
    cmd_nooperation.append (QString::number(packetno, 10).toUtf8());
    cmd_nooperation.append (":");
    cmd_nooperation.append ((username).toUtf8());
    cmd_nooperation.append (":");
    cmd_nooperation.append ((hostname).toUtf8());
    cmd_nooperation.append (":");
    cmd_nooperation.append (QString::number(cmd, 16).toUtf8());
    cmd_nooperation.append (":");
    cmd_nooperation.append ("8C:89:A5:BE:1E:F5");

    qDebug () << cmd_nooperation;
    return cmd_nooperation;
}

QByteArray Protocol::buildcmdBrEntry()
{

    // Set seed value.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ENTRY;
    qDebug() << "bcmd: " << cmd;

    // set IPMSG_ABSENCEOPT option
    if (absenceStatus == ABSENCEABSENCE ||
        absenceStatus == ABSENCEMEAL ||
        absenceStatus == ABSENCEMEETING ||
        absenceStatus == ABSENCEVISITOR ||
        absenceStatus == ABSENCEOUT ||
        absenceStatus == ABSENCEHOME ||
        absenceStatus == ABSENCEEDO ||
        absenceStatus == ABSENCEPRIEST) {
        cmd = cmd | IPMSG_ABSENCEOPT;
    }


    QByteArray cmd_br_entry;
    cmd_br_entry.append(QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_br_entry.append (":");
    cmd_br_entry.append (QString::number(packetno, 10).toUtf8());
    cmd_br_entry.append (":");
    cmd_br_entry.append ((username).toUtf8());
    cmd_br_entry.append (":");
    cmd_br_entry.append ((hostname).toUtf8());
    cmd_br_entry.append (":");
    cmd_br_entry.append (QString::number(cmd, 16).toUtf8());
    cmd_br_entry.append (":");
    cmd_br_entry.append ((username).toUtf8());

    return cmd_br_entry;
}


QByteArray Protocol::buildcmdBrExit()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_EXIT;

    QByteArray cmd_br_exit;
    cmd_br_exit.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_br_exit.append (":");
    cmd_br_exit.append (QString::number(packetno, 10).toUtf8());
    cmd_br_exit.append (":");
    cmd_br_exit.append ((username).toUtf8());
    cmd_br_exit.append (":");
    cmd_br_exit.append ((hostname).toUtf8());
    cmd_br_exit.append (":");
    cmd_br_exit.append (QString::number(cmd, 16).toUtf8());
    cmd_br_exit.append (":");
    cmd_br_exit.append ((username).toUtf8());

    qDebug () << cmd_br_exit;
    return cmd_br_exit;
}

QByteArray Protocol::buildcmdAnsentry()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_ANSENTRY;
    // set IPMSG_ABSENCEOPT option
    if (absenceStatus == ABSENCEABSENCE ||
        absenceStatus == ABSENCEMEAL ||
        absenceStatus == ABSENCEMEETING ||
        absenceStatus == ABSENCEVISITOR ||
        absenceStatus == ABSENCEOUT ||
        absenceStatus == ABSENCEHOME ||
        absenceStatus == ABSENCEEDO ||
        absenceStatus == ABSENCEPRIEST) {
        cmd = cmd | IPMSG_ABSENCEOPT;
    }

    QByteArray cmd_ansentry;
    cmd_ansentry.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_ansentry.append (":");
    cmd_ansentry.append (QString::number(packetno, 10).toUtf8());
    cmd_ansentry.append (":");
    cmd_ansentry.append ((username).toUtf8());
    cmd_ansentry.append (":");
    cmd_ansentry.append ((hostname).toUtf8());
    cmd_ansentry.append (":");
    cmd_ansentry.append (QString::number(cmd, 16).toUtf8());
    cmd_ansentry.append (":");
    cmd_ansentry.append ((username).toUtf8());

    qDebug () << "Ansentry" << cmd_ansentry;
    return cmd_ansentry;
}

QByteArray Protocol::buildcmdBrAbsence(const quint32 &absenceStatus)
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ABSENCE;

    QByteArray cmd_absence;
    cmd_absence.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_absence.append (":");
    cmd_absence.append (QString::number(packetno, 10).toUtf8());
    cmd_absence.append (":");
    cmd_absence.append ((username).toUtf8());
    cmd_absence.append (":");
    cmd_absence.append ((hostname).toUtf8());
    cmd_absence.append (":");
    cmd_absence.append (QString::number(cmd, 16).toUtf8());
    cmd_absence.append (":");
    cmd_absence.append ((username).toUtf8());

    switch (absenceStatus) {
        case ABSENCEMEAL:
            cmd_absence.append("[meal]");
            break;
        case ABSENCEMEETING:
            cmd_absence.append("[meeting]");
            break;
        case ABSENCEVISITOR:
            cmd_absence.append("[visitor]");
            break;
        case ABSENCEOUT:
            cmd_absence.append("[out]");
            break;
        case ABSENCEHOME:
            cmd_absence.append("[home]");
            break;
        case ABSENCEEDO:
            cmd_absence.append("[edo]");
            break;
        case ABSENCEPRIEST:
            cmd_absence.append("[priest]");
            break;
        default:
            cmd_absence.append("[absence]");
            break;
    }
    qDebug () << cmd_absence;
    return cmd_absence;
}

QByteArray Protocol::buildcmdBrIsgetlist()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdOkgetlist()
{

    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_OKGETLIST;

    QByteArray cmd_okgetlist;
    cmd_okgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_okgetlist.append (":");
    cmd_okgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_okgetlist.append (":");
    cmd_okgetlist.append ((username).toUtf8());
    cmd_okgetlist.append (":");
    cmd_okgetlist.append ((hostname).toUtf8());
    cmd_okgetlist.append (":");
    cmd_okgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_okgetlist.append (":");
    cmd_okgetlist.append ((username).toUtf8());

    qDebug () << cmd_okgetlist;
    return cmd_okgetlist;
}

QByteArray Protocol::buildcmdGetlist()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_GETLIST;

    QByteArray cmd_getlist;
     cmd_getlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
     cmd_getlist.append (":");
     cmd_getlist.append (QString::number(packetno, 10).toUtf8());
     cmd_getlist.append (":");
     cmd_getlist.append ((username).toUtf8());
     cmd_getlist.append (":");
     cmd_getlist.append ((hostname).toUtf8());
     cmd_getlist.append (":");
     cmd_getlist.append (QString::number(cmd, 16).toUtf8());
     cmd_getlist.append (":");
     cmd_getlist.append ((username).toUtf8());

    qDebug () << cmd_getlist;
    return cmd_getlist;
}

QByteArray Protocol::buildcmdAnslist()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_ANSLIST;

    QByteArray cmd_anslist;
     cmd_anslist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
     cmd_anslist.append (":");
     cmd_anslist.append (QString::number(packetno, 10).toUtf8());
     cmd_anslist.append (":");
     cmd_anslist.append ((username).toUtf8());
     cmd_anslist.append (":");
     cmd_anslist.append ((hostname).toUtf8());
     cmd_anslist.append (":");
     cmd_anslist.append (QString::number(cmd, 16).toUtf8());
     cmd_anslist.append (":");
     cmd_anslist.append ((username).toUtf8());

    qDebug () <<  cmd_anslist;
    return cmd_anslist;
}

QByteArray Protocol::buildcmdBrIsgetlist2()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdSendmsg(const QString &msg)
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_SENDMSG;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((msg).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdRecvmsg(const QString &packetno)
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 pktno;
    pktno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_RECVMSG;

    QByteArray cmd_recvmsg;
    cmd_recvmsg.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_recvmsg.append (":");
    cmd_recvmsg.append (QString::number(pktno, 10).toUtf8());
    cmd_recvmsg.append (":");
    cmd_recvmsg.append ((username).toUtf8());
    cmd_recvmsg.append (":");
    cmd_recvmsg.append ((hostname).toUtf8());
    cmd_recvmsg.append (":");
    cmd_recvmsg.append (QString::number(cmd, 10).toUtf8());
    cmd_recvmsg.append (":");
    cmd_recvmsg.append ((packetno).toUtf8());

    qDebug () << cmd_recvmsg;
    return cmd_recvmsg;
}

QByteArray Protocol::buildcmdReadmsg()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdDelmsg()
{
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());

    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdAnsreadmsg()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdGetinfo()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdSendinfo()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdGetabsenceinfo()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdSendabsenceinfo()
{

    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdGetfiledata()
{

    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;


    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdReleasefiles()
{
    // Set init package number as random.
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdGetdirfiles()
{
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

QByteArray Protocol::buildcmdGetpubkey()
{
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug() << cmd_isgetlist;
    return cmd_isgetlist;
}


QByteArray Protocol::buildcmdAnspubkey()
{
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    quint64 packetno;
    packetno = rand() % 10240;

    quint32 cmd = 0;
    cmd = cmd | IPMSG_BR_ISGETLIST;

    QByteArray cmd_isgetlist;
    cmd_isgetlist.append (QString::number(IPMSG_VERSION, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(packetno, 10).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((hostname).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append (QString::number(cmd, 16).toUtf8());
    cmd_isgetlist.append (":");
    cmd_isgetlist.append ((username).toUtf8());

    qDebug () << cmd_isgetlist;
    return cmd_isgetlist;
}

