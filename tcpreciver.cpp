#include "tcpreciver.h"

TcpReciver::TcpReciver(QObject *parent) : QObject(parent), m_nextBlockSize(0)
{
    m_server = new QTcpServer(this);
    m_udp = new QUdpSocket(this);
    m_udp->bind(QHostAddress::Any, 11002);
    m_server->listen(QHostAddress::Any, 11000);
    connect(m_server, SIGNAL (newConnection()), this, SLOT (slotNewConnection()));
    connect(m_udp, SIGNAL (readyRead()), SLOT (checkDatagram()));
    qDebug() << QHostInfo::localHostName();
}

TcpReciver::~TcpReciver()
{
}

void TcpReciver::slotNewConnection()
{
    QTcpSocket *clientTcp = m_server->nextPendingConnection();
    qDebug() << "Connection Succes!!";
    connect(clientTcp, SIGNAL (disconnected()), clientTcp, SLOT (deleteLater()));
    connect(clientTcp, SIGNAL (readyRead()), this, SLOT (ReadClient()));
}

void TcpReciver::checkDatagram()
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    QNetworkDatagram datagram = m_udp->receiveDatagram();
    qDebug() << datagram.senderAddress();
    QString str = QHostInfo::localHostName();
    stream << str;
    m_udp->writeDatagram(arr, datagram.senderAddress(), 11001);
}

void TcpReciver::sendDatagram()
{
   QByteArray arr("exit");
   QNetworkDatagram datagram(arr, QHostAddress::Broadcast, 11002);
   m_udp->writeDatagram(datagram);
}

void TcpReciver::ReadClient()
{
    m_client = (QTcpSocket*) sender();
    QDataStream in(m_client);
    in.setVersion(QDataStream::Qt_5_9);
    while (true) {
        if (!m_nextBlockSize) {
            if(m_client->bytesAvailable() < sizeof(quint16))
                break;
            in >> m_nextBlockSize;
        }
        if (m_client->bytesAvailable() < m_nextBlockSize)
            break;
        QString command;
        in >> command;
        if (command == "Reboot")
            commandReboot();
        else if (command == "Shutdown")
            commandShutdown();
        else if (command == "chromium") {
            if (!chromiumStarted)
                chromiumStart();
            else
                chromiumStop();
        }
        m_nextBlockSize = 0;
    }
}

void TcpReciver::commandReboot()
{
    QProcess *process = new QProcess();
    process->start("reboot");
}

void TcpReciver::commandShutdown()
{
    QProcess *process = new QProcess();
    process->start("shutdown -h now");
}

void TcpReciver::chromiumStart()
{
    QProcess *process = new QProcess();
    process->start("sudo -u " + checkName() + " -H env DISPLAY=:0.0 chromium");
    m_processId = process->processId();
    chromiumStarted = true;
}

void TcpReciver::chromiumStop()
{
    m_processId++;
    QProcess *process = new QProcess();
    process->start("kill " + QString::number (m_processId));
    chromiumStarted = false;
}

QString TcpReciver::checkName()
{
    QDir dir("/home");
    QStringList list = dir.entryList();
    return list[2];
}
