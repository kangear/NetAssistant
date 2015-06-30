#include "myudp.h"
#include <QMessageBox>

MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{
    qDebug("%s", __func__);
    socket = NULL;
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
void MyUDP::connection_error(QAbstractSocket::SocketError err)
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
void MyUDP::sendData(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s %d", __func__, __LINE__);
    if(socket == NULL)
        return;
    qDebug("%s %d", __func__, __LINE__);
    QByteArray Data;
    Data.append(string);
    emit updateState(QString(), QVariant(QVariant::Int), socket->writeDatagram(Data, QHostAddress(remoteIp), port));
}


void MyUDP::connectNet(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s", __func__);
    if(socket == NULL) {
        socket = new QUdpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connection_error(QAbstractSocket::SocketError)));
    }
}

void MyUDP::disconnectNet(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s", __func__);
    if(socket != NULL) {
        socket->close();
        socket = NULL;
    }
}

void MyUDP::readyRead()
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
