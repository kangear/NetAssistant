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
    void connectNet(const QString string, const QString remoteIp, const int port);
    void disconnectNet(const QString string, const QString remoteIp, const int port);
signals:
    void valueChanged(QString newValue);
    void updateState(QString state, QVariant inNum, QVariant outNum);
public slots:
    void readyRead();
    void connection_error(QAbstractSocket::SocketError err);
private:
    QUdpSocket *socket;
};

#endif // UDPCLIENT_H

