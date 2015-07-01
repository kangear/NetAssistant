#include "udpclient.h"
#include <QMessageBox>

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    qDebug("%s", __func__);
    udpSendSocket = NULL;
    // 这里一定不要时行bind，否则会造成数据发不到外网去
//    if(!socket->bind(QHostAddress::LocalHost, 1234)) {
//        qWarning("NULL");
//    }
}

/**
 * 处理网络连接错误
 * @brief MyUDP::connection_error
 * @param err
 */
void UDPClient::connection_error(QAbstractSocket::SocketError err)
{
    qDebug("%s", __func__);
    switch(err){
    case 0:QMessageBox::critical(0,"connection error","The connection was refused by the peer (or timed out).",QMessageBox::Ok);
           break;
    case 2:QMessageBox::critical(0,"connection error","The host address was not found.",QMessageBox::Ok);
           break;
    case QAbstractSocket::NetworkError:QMessageBox::critical(0,"connection error","An error occurred with the network .",QMessageBox::Ok);
           break;

    case QAbstractSocket::RemoteHostClosedError:
           QMessageBox::critical(0,"connection error","disconnect .",QMessageBox::Ok);
           break;
    default:QMessageBox::critical(0,"connection error","undefine error.",QMessageBox::Ok);
        qDebug()<<"error is ......."<<err;
        break;
    }
}
/**
 * @brief MyUDP::sendData
 * @param string   发送内容
 * @param remoteIp 目标IP地址
 * @param port     目标端口号
 */
void UDPClient::sendData(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s %d", __func__, __LINE__);
    if(udpSendSocket == NULL)
        return;
    qDebug("%s %d", __func__, __LINE__);
    QByteArray Data;
    Data.append(string);
    emit updateState(QString(), QVariant(QVariant::Int), udpSendSocket->writeDatagram(Data, QHostAddress(remoteIp), port));
}

void UDPClient::udpListnerStart(const QHostAddress ip, const int port)
{
    qDebug("%s", __func__);
    udpListnerSocket = new QUdpSocket(this);
    if(!udpListnerSocket->bind(ip, port)) {
        qWarning("NULL");
    }
    connect(udpListnerSocket, SIGNAL(readyRead()), this, SLOT(readyListnerRead()));
    connect(udpListnerSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connection_error(QAbstractSocket::SocketError)));
}

void UDPClient::udpListnerStop()
{
    qDebug("%s", __func__);
    udpListnerSocket->close();
}

void UDPClient::connectNet(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s", __func__);
    if(udpSendSocket == NULL) {
        udpSendSocket = new QUdpSocket(this);
        connect(udpSendSocket, SIGNAL(readyRead()), this, SLOT(readySendRead()));
        connect(udpSendSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connection_error(QAbstractSocket::SocketError)));
    }
}

void UDPClient::disconnectNet(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s", __func__);
    if(udpSendSocket != NULL) {
        udpSendSocket->close();
        udpSendSocket = NULL;
    }
}

void UDPClient::readyListnerRead()
{
    qDebug("%s", __func__);
    readyRead(udpListnerSocket);
}

void UDPClient::readyRead(QUdpSocket* socket)
{
    qDebug("%s", __func__);
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    qDebug() << "Message from:" << sender.toString();
    qDebug() << "Message port:" << senderPort;
    qDebug() << "Message: " << Buffer;
    emit valueChanged(Buffer);
    emit updateState(QString(), Buffer.size(), QVariant(QVariant::Int));
}

void UDPClient::readySendRead()
{
    qDebug("%s", __func__);
    readyRead(udpSendSocket);
}
