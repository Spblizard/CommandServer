#include "tcpreciver.h"

TcpReciver::TcpReciver(QObject *parent) : QObject(parent), m_nextBlockSize(0)
{
    m_server = new QTcpServer(this);
    m_udp = new QUdpSocket(this);
    m_udp->bind(QHostAddress::Any, 11002);
    m_server->listen(QHostAddress::Any, 11000);
    connect(m_server, &QTcpServer::newConnection, this, &TcpReciver::slotNewConnection);
    connect(m_udp, &QUdpSocket::readyRead, this, &TcpReciver::checkDatagram);
    qDebug() << QHostInfo::localHostName();
}

TcpReciver::~TcpReciver()
{
}

void TcpReciver::slotNewConnection()
{
    m_client = m_server->nextPendingConnection();
    qDebug() << "Connection Succes!!";
    connect(m_client, &QTcpSocket::disconnected, m_client, &QTcpSocket::deleteLater);
    connect(m_client, &QTcpSocket::disconnected, this, &TcpReciver::slotDisconnected);
    connect(m_client, &QTcpSocket::readyRead, this, &TcpReciver::readClient);
    for (int i = 0; i < mCommands.length(); i++)
        sendCommand(mCommands[i]);
}

void TcpReciver::slotDisconnected()
{
    qDebug() << "Client disconnected";
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

void TcpReciver::readClient()
{
    m_client = qobject_cast<QTcpSocket*>(sender()); 
    QDataStream in(m_client);
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
        qDebug() << "commands: " << command;
        m_nextBlockSize = 0;
    }
}

void TcpReciver::sendCommand(const QString &command)
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << quint16(0) << command;
    m_client->write(arr);
    m_client->flush();
}
