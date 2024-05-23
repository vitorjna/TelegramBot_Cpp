#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <QAbstractSocket>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>

#include "botStuff/BotStuff.h"

void registerMetatypes();
extern "C" void __cxa_pure_virtual(void);

namespace __cxxabiv1
{
extern "C"
void __cxa_pure_virtual(void)
{
    char const msg[] = "pure virtual called\n";
    write(2, msg, sizeof msg - 1);
    std::terminate();
}
} // namespace __cxxabiv1

void registerMetatypes()
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
}


int main(int argc, char **argv)
{
    srand(QTime::currentTime().msec());

    registerMetatypes();

    QCoreApplication botStuff(argc, argv);

    BotStuff::start();

    return botStuff.exec();
}

