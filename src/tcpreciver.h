#ifndef TCPRECIVER_H
#define TCPRECIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QDataStream>
#include <QHostInfo>
#include <QProcess>
#include <QProcessEnvironment>

class TcpReciver : public QObject
{
    Q_OBJECT
public:
    explicit TcpReciver(QObject *parent = nullptr);
    ~TcpReciver();
private:
    QTcpServer *m_server;
    QTcpSocket *m_client;
    QUdpSocket *m_udp;
    quint16 m_nextBlockSize;

signals:

public slots:
    virtual void slotNewConnection();
    void checkDatagram();
    void sendDatagram();
    void ReadClient();
    void commandReboot();
    void commandShutdown();
    QString checkName();
};

#endif // TCPRECIVER_H
