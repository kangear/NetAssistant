#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);
    void sendData(const QString string, const QString remoteIp, const int port);
    void udpStart(const QHostAddress localIp, const int listnerPort, const QHostAddress remoteIp, const int remotePort);
    void udpStop(const QString string, const QString remoteIp, const int port);
    void readyRead(QUdpSocket* socket);

signals:
    void valueChanged(QString newValue);
    void updateState(QString state, QVariant inNum, QVariant outNum);
public slots:
    void readyListnerRead();
    void readySendRead();
    void connection_error(QAbstractSocket::SocketError err);
private:
    QUdpSocket *udpSendSocket = nullptr;
    QUdpSocket *udpListnerSocket = nullptr;
    void udpListnerStart(const QHostAddress ip, const int port);
    void udpListnerStop();
};

#endif // UDPCLIENT_H

