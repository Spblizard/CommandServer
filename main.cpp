#include <QCoreApplication>
#include "tcpreciver/tcpreciver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpReciver serv;

    QObject::connect(&a, SIGNAL (aboutToQuit()), &serv, SLOT (sendDatagram()));

    return a.exec();
}
