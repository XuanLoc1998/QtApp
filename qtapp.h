#ifndef QTAPP_H
#define QTAPP_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtSerialPort>
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui { class QtApp; }
QT_END_NAMESPACE

class QtApp : public QMainWindow
{
    Q_OBJECT

public:
    QtApp(QWidget *parent = nullptr);
    ~QtApp();

    void initSerial();

private slots:
    // Serial tab
    void on_btnOpenClose_clicked();
    void serialReceived();
    void on_btn_send_clicked();
    void on_tabWidget_currentChanged(int index);
    // TCP Client tab
    void socketClientRead();
    void socketClientDisconnected();
    void on_btnSendTCPClient_clicked();
    void on_btnConnectTCPClient_clicked();

    //TCP server tab
    void on_btnSendTCPServer_clicked();
    void serverNewConnect();
    void socketServerRead();
    void socketServerDisconnected();

    void on_btnListenTCPServer_clicked();

private:
    Ui::QtApp *ui;
    QSerialPort *serial;
    QTcpSocket* socket;

    //TCP server
    QTcpServer* server;
    QTcpSocket* socketServer;


};
#endif // QTAPP_H
