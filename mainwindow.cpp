#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include "udpclient.h"
#include <QLabel>
#include <QDate>
#include "commonhelper.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug("%s", __func__);
    ui->setupUi(this);

    /* 读取配置文件 */
    doSettings(false);

    /* 设置默认通讯模式 */
    ui->tcpclient_radioButton->setChecked(true);
    /** 目前设置为UDP为默认方式 */
    ui->udp_radioButton->setChecked(true);
    /** 设置远程主机IP地址 获取本机IP */
    ui->remoteIP_lineEdit->setText(mRemoteIp);
    /* 设置远程端口号 */
    /* TODO: 将其设置为不能以0开头 */
    ui->remoteport_spinBox->setRange(1024,9999);
    ui->remoteport_spinBox->setValue(mRemotePort);
    /* 设置本地端口号 */
    ui->localport_spinBox->setRange(1024,9999);
    ui->localport_spinBox->setValue(mLocalPort);

    isConnect = false;

    // 状态栏
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(260, 20); // 设置标签最小大小
    statusLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    statusLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(statusLabel);
    statusLabel->setText("UDP通信停止");
    statusLabel->setAlignment(Qt::AlignHCenter);

    // 接收数量
    receiveLabel = new QLabel;
    receiveLabel->setMinimumSize(150, 20); // 设置标签最小大小
    receiveLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    receiveLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(receiveLabel);
    receiveLabel->setAlignment(Qt::AlignHCenter);

    // 发送数量
    sendLabel = new QLabel;
    sendLabel->setMinimumSize(150, 20); // 设置标签最小大小
    sendLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    sendLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(sendLabel);
    sendLabel->setAlignment(Qt::AlignHCenter);
    updateStateBar(QString(), 0, 0);

    // 计数器清零 button
    clearCounterButton = new QPushButton();
    ui->statusBar->addWidget(clearCounterButton);
    clearCounterButton->setText(tr("计数器清零"));
    connect(clearCounterButton, SIGNAL(released()), this, SLOT(on_clearCounter_pushButton_released()));

    // 时间 TODO:要进行更新
    timeLabel = new QLabel;
    timeLabel->setMinimumSize(90, 20); // 设置标签最小大小
    timeLabel->setMaximumWidth(90);
    timeLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    timeLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(timeLabel);
    timeLabel->setText(QDate::currentDate().toString("yyyy-MM-dd"));

    // 更新接收到的数据
    connect(&client, SIGNAL(valueChanged(QString)), this, SLOT(updateReceiveText(QString)));
    connect(&client,
            SIGNAL(updateState(QString, QVariant, QVariant)),
            this, SLOT(updateStateBar(QString, QVariant, QVariant)));

    init();

    mReceiveNum = mSendNum = 0;
}

void MainWindow::connectNet()
{
    qDebug("%s", __func__);

    mRemoteIp = ui->remoteIP_lineEdit->text();
    mRemotePort = ui->remoteport_spinBox->text().toInt();
    mLocalPort = ui->localport_spinBox->text().toInt();
    updateStateBar("UDP通信 " + mRemoteIp + ":" + QString().number(mRemotePort),
                   QVariant(QVariant::Int), QVariant(QVariant::Int));

    // No.1
    isConnect = true;
    // 将状态设置为 通
    ui->state_label->setText("通");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　断开网络
    ui->connect_pushButton->setText("断开网络");

    // 禁用远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(false);
    ui->remoteport_spinBox->setEnabled(false);
    ui->localport_spinBox->setEnabled(false);
    // 使能button
    ui->handSend_pushButton->setEnabled(true);

    client.udpStart(chelper.getLocalHostIP(), mLocalPort, QHostAddress(mRemoteIp), mRemotePort);
}

void MainWindow::updateReceiveText(const QString string)
{
    QString oldString = ui->receive_textBrowser->toPlainText();
    ui->receive_textBrowser->setText(oldString + string + "\n");

    // 将光标移动到最后位置
    QTextCursor tmpCursor = ui->receive_textBrowser->textCursor();
    tmpCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 4);
    ui->receive_textBrowser->setTextCursor(tmpCursor);
}

/**
 * @brief MainWindow::updateStateBar
 * @param state 状态
 * @param inNum 接收数值
 * @param outNum 发送数值
 */
void MainWindow::updateStateBar(QString state, QVariant inNum, QVariant outNum)
{
    if(!state.isNull())
        statusLabel->setText(state);

    if(!inNum.isNull()) {
        //累计
        if(inNum.toInt() == 0)
            mReceiveNum = 0;
        else
            mReceiveNum += inNum.toInt();
        receiveLabel->setText("接收:" + QString::number(mReceiveNum));
    }

    if(!outNum.isNull()) {
        //累计
        if(outNum.toInt() == 0)
            mSendNum = 0;
        else
            mSendNum += outNum.toInt();
        sendLabel->setText("发送:" + QString::number(mSendNum));
    }

}

/**
 * 断开UDP时调用该函数
 * @brief MainWindow::init
 */
void MainWindow::init()
{
    qDebug("%s", __func__);
    // No.1
    isConnect = false;
    // 将状态设置为 断
    ui->state_label->setText("断");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　连接网络
    ui->connect_pushButton->setText("连接网络");

    // 使能远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(true);
    ui->remoteport_spinBox->setEnabled(true);
    ui->localport_spinBox->setEnabled(true);
    // 禁用button
    ui->handSend_pushButton->setEnabled(false);
    //
    client.udpStop(NULL, NULL, NULL);

    updateStateBar("本地IP: " + chelper.getLocalHostIP().toString() + " 无连接",
                   QVariant(QVariant::Int), QVariant(QVariant::Int));
}

/**
 * 断开UDP时调用该函数
 * @brief MainWindow::disConnectNet
 */
void MainWindow::disConnectNet()
{
    qDebug("%s", __func__);
    // No.1
    isConnect = false;
    // 将状态设置为 断
    ui->state_label->setText("断");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　连接网络
    ui->connect_pushButton->setText("连接网络");

    // 使能远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(true);
    ui->remoteport_spinBox->setEnabled(true);
    ui->localport_spinBox->setEnabled(true);
    // 禁用button
    ui->handSend_pushButton->setEnabled(false);
    //
    client.udpStop(NULL, NULL, NULL);


    updateStateBar(tr("UDP通信停止"), QVariant(QVariant::Int), QVariant(QVariant::Int));
}

void MainWindow::doSettings(bool isWrite)
{
    QSettings settings("Yzs_think", "Application");
    const QString REMOTE_IP = "remoteip";
    const QString REMOTE_PORT = "remoteport";
    const QString LOCAL_PORT = "localport";
    if(isWrite) {
        settings.setValue(REMOTE_IP, mRemoteIp);
        settings.setValue(REMOTE_PORT, mRemotePort);
        settings.setValue(LOCAL_PORT, mLocalPort);
    } else {
        mRemoteIp = settings.value(REMOTE_IP, chelper.getLocalHostIP().toString()).toString();
        mRemotePort = settings.value(REMOTE_PORT, 1234).toInt();
        mLocalPort = settings.value(LOCAL_PORT, 2468).toInt();
    }
}

MainWindow::~MainWindow()
{
    doSettings(true);
    delete ui;
}

/**
 * @brief MainWindow::on_clearCounter_pushButton_released
 * 该函数是将计数器置零
 */
void MainWindow::on_clearCounter_pushButton_released()
{
    qDebug("%s", __func__);
    updateStateBar(QString(), 0, 0);
}

void MainWindow::on_clearReceive_pushButton_released()
{
    ui->receive_textBrowser->clear();
}

void MainWindow::on_clearSend_pushButton_released()
{
    ui->send_plainTextEdit->clear();
}

/**
 * @brief MainWindow::on_connect_pushButton_released
 * 该函数是在连接/断开网络时调用
 */
void MainWindow::on_connect_pushButton_released()
{
    qDebug("%s", __func__);
    // 如果当前网络是连接状态　调用断开连接函数
    if(isConnect) {
        disConnectNet();
    } else { // 否则调用连接函数
        connectNet();
    }
}

/**
 * 当用户点击 发送 时调用该函数
 * @brief MainWindow::on_handSend_pushButton_released
 */
void MainWindow::on_handSend_pushButton_released()
{
    // 获取 rmeote ip/ port 和内容
    QString string = ui->send_plainTextEdit->toPlainText();
    if(string.length() != 0) {
        client.sendData(string, mRemoteIp, mRemotePort);
    }
}

void MainWindow::on_quit_pushButton_released()
{
    QApplication::quit();
}
