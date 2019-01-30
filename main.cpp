#include <QCoreApplication>
#include "tcpreciver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpReciver serv;

    return a.exec();
}
