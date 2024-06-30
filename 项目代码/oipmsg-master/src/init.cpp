#include <QDebug>
#include "init.h"

Init::Init()
{
    // 如果在 Windows 平台上编译
#ifdef Q_OS_WIN
   qDebug() << "Win64";
#endif

    // 如果在 Linux/Unix 平台上编译
#ifdef Q_OS_UNIX
   qDebug() << "Linux/Unix";
#endif

    // 如果在 Mac OS X 平台上编译
#ifdef  Q_OS_MAC
   qDebug() << "Mac OS X";
#endif
}

Init::~Init()
{

}
