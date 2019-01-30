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
private:
    QTcpServer *m_server;
    QTcpSocket *m_client;
    QUdpSocket *m_udp;
    quint16 m_nextBlockSize;
    quint64 m_processId;
    bool chromiumStarted = false;

signals:

public slots:
    virtual void slotNewConnection();
    void checkDatagram();
    void ReadClient();
    void commandReboot();
    void commandShutdown();
    void chromiumStart();
    void chromiumStop();
    QString checkName();
};

#endif // TCPRECIVER_H