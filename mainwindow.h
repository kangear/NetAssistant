#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>
#include <QPushButton>
#include "udpclient.h"
#include "commonhelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_clearReceive_pushButton_released();
    void on_clearSend_pushButton_released();
    void on_connect_pushButton_released();
    void on_handSend_pushButton_released();
    void updateReceiveText(const QString string);
    void on_clearCounter_pushButton_released();
    void on_quit_pushButton_released();
    void updateStateBar(QString state, QVariant inNum, QVariant outNum);

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    void connectNet();
    void disConnectNet();
    bool isConnect;
    UDPClient client;
    /** 工具类 */
    CommonHelper chelper;
    /** 状态标签 */
    QLabel *statusLabel;
    /** 总接收数量显示标签 */
    QLabel *receiveLabel;
    /** 总发送数量显示标签 */
    QLabel *sendLabel;
    /** 状态栏 计数清零 按钮 */
    QPushButton *clearCounterButton;
    /** 状态栏 时间标签 */
    QLabel *timeLabel;
    /** 接收总数 */
    quint64 mReceiveNum;
    /** 发送总数 */
    quint64 mSendNum;
    /** 远程IP */
    QString mRemoteIp;
    /** 远程端口 */
    int mRemotePort;
    /** 本地端口 */
    int mLocalPort;

    void doSettings(bool isWrite);
    void init();
};

#endif // MAINWINDOW_H
