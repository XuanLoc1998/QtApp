#pragma once

#include "qtapp.h"
#include "ui_qtapp.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include "QPixmap"


//Avoid addItems more than one time for Serial Tab
int iAddItems = 0;

QtApp::QtApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtApp)
{
    ui->setupUi(this);
    ui -> tabWidget -> setCurrentIndex(0);

#pragma region Load Pictures{
    QPixmap pSerial("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/ComPort.png");
    QPixmap pTcp("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/tcpip.jpg");
    QPixmap pUdp("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/udp.png");
    QPixmap pMultiThread("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/multithread.jpg");
    ui -> picSerial -> setPixmap(pSerial.scaled(200, 200, Qt::KeepAspectRatio));
    ui -> picTcp -> setPixmap(pTcp.scaled(200, 200, Qt::KeepAspectRatio));
    ui -> picUdp -> setPixmap(pUdp.scaled(200, 200, Qt::KeepAspectRatio));
    ui -> picMultiThread -> setPixmap(pMultiThread.scaled(200, 200, Qt::KeepAspectRatio));

    QPixmap pLogoVtx("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/logoVTX.jpg");
    ui -> picLogoVtx -> setPixmap(pLogoVtx.scaled(300, 300, Qt::KeepAspectRatio ));

    //About app
    QPixmap pQt("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/Qt.png");
    ui -> picQt -> setPixmap(pQt.scaled(100, 100, Qt::KeepAspectRatio ));
    QPixmap pCpp("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/logoCPP.png");
    ui -> picCpp -> setPixmap(pCpp.scaled(100, 100, Qt::KeepAspectRatio ));

    //About me
    QPixmap pAvt("D:/QT Tutorial/VisualQt/QtApp/QtApplicationForm/Pictures/XL.png");
    ui -> picAvatar -> setPixmap(pAvt.scaled(200, 200, Qt::KeepAspectRatio ));
#pragma endregion }

}

QtApp::~QtApp()
{
    serial -> close();
    //TCP Client
    delete this -> socket;
    //TCP Server
    server -> close();
    server -> deleteLater();

    delete ui;
}
// Select tabwidget and setting parameters
void QtApp::on_tabWidget_currentChanged(int index)
{

    //const QSerialPortInfo serial_info;

    //Get port name
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    int countCOM = portList.count();

    QStringList portListString = {};
    for(int i = 0; i< countCOM; i++){
      portListString.append(portList[i].portName());
    };

    // Get Pictures

    switch (ui -> tabWidget -> currentIndex()) {

    case 0:


        break;
    case 1: //Serial
        iAddItems += 1;
        if (iAddItems == 1){
            //Init Comboboxes
            ui -> cmb_COM -> addItems(portListString);
            //-------------------------------
            ui ->cmb_baudrate -> addItems({"9600", "19200", "38400", "115200"});
            ui->cmb_baudrate -> setCurrentIndex(0);

            ui -> cmb_databit -> addItems({"7 Bits", "8 Bits"});
            ui -> cmb_databit ->setCurrentIndex(1);

            ui -> cmb_parity -> addItems({"None", "Even", "Odd"});
            ui -> cmb_parity -> setCurrentIndex((0));

            ui -> cmb_stopbit -> addItems({"1", "1.5", "2"});
            ui -> cmb_stopbit -> setCurrentIndex(0);

            // Init a serial
            serial = new QSerialPort(this);
            //Init btnOpen text
            ui -> btnOpenClose -> setText("Open");
            ui -> btn_send -> setEnabled(false);
            break;
        } else{
            break;
        }


    case 2: // TCP Client
        ui -> txbIPClient -> setText("127.0.0.1");
        ui -> txbPortClient -> setText("3000");

        //Init Socket
        socket = new QTcpSocket();
        //connect signal slot
        QObject::connect(socket, &QTcpSocket::readyRead, this,
                         &QtApp::socketClientRead);
        QObject::connect(socket, &QTcpSocket::disconnected, this,
                         &QtApp::socketClientDisconnected);
        ui -> btnSendTCPClient ->setEnabled(false);
        break;
    case 3: // TCP Server
        ui -> txbIPServer -> setText("127.0.0.1");
        ui -> txbIPServer -> setEnabled(false);

        ui -> txbPortServer -> setText("3000");
        ui -> btnSendTCPServer -> setEnabled(false);

        ui -> btnListenTCPServer -> setText("Listen");
        //Init server
        server = new QTcpServer();
        //Signal - Slot
        QObject::connect(server, &QTcpServer::newConnection, this, &QtApp::serverNewConnect);
        break;

}
    return;
}

#pragma region Serial Code {

void QtApp::on_btnOpenClose_clicked()
{
    if (ui -> btnOpenClose -> text() == tr("Open")){

        //Choose Port
        serial -> setPortName(ui -> cmb_COM -> currentText());

        //Baudrate
        switch (ui->cmb_baudrate->currentIndex()) {
        case 0:
            serial -> setBaudRate(QSerialPort::Baud9600);
            break;
        case 1:
            serial -> setBaudRate(QSerialPort::Baud19200);
            break;
        case 2:
            serial -> setBaudRate(QSerialPort::Baud38400);
            break;
        case 3:
            serial -> setBaudRate((QSerialPort::Baud115200));
            break;
        default:
            serial -> setBaudRate(QSerialPort::Baud9600);
            break;
        }

        //Data bit
        switch (ui -> cmb_databit -> currentIndex()) {
        case 0:
            serial -> setDataBits(QSerialPort::Data7);
            break;
        case 1:
            serial -> setDataBits(QSerialPort::Data8);
            break;
        }

        //Parity
        switch (ui -> cmb_parity -> currentIndex()) {
        case 0:
            serial -> setParity(QSerialPort::NoParity);
            break;
        case 1:
            serial -> setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serial -> setParity(QSerialPort::OddParity);
            break;
        }
        // Stop bit
        switch (ui -> cmb_stopbit -> currentIndex()){
        case 0:
            serial -> setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            serial -> setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            serial -> setStopBits(QSerialPort::TwoStop);
            break;
        }

        //Flow control
        serial-> setFlowControl(QSerialPort::NoFlowControl);

        //Open port
        if (serial-> open(QIODevice::ReadWrite)){
           ui -> statPort -> setText("Port is Opened!");
           ui -> statPort -> setStyleSheet("color: black");
           ui -> btnOpenClose -> setText("Close");
           //Set enable send
           ui -> btn_send -> setEnabled(true);
        }
        else{
            QString err = "Error:" + serial->errorString();
            ui -> statPort -> setText(err);
            ui -> statPort -> setStyleSheet("color: red");


        }

    }
    else{

        //Close port
        serial -> close();

           ui -> statPort -> setText("Port is Closed!");
           ui -> statPort -> setStyleSheet("color: black");
           ui -> btnOpenClose -> setText("Open");
           ui -> btn_send -> setEnabled(false);
    }

    //Create Signal-Slot
    connect(serial, SIGNAL(readyRead()), this, SLOT(serialReceived()));

}

void QtApp::serialReceived(){
    ui ->lb_data_received -> moveCursor(QTextCursor::End);
    ui->lb_data_received->insertPlainText(serial->readAll());
}

void QtApp::on_btn_send_clicked()
{
    serial -> write(ui -> txb_dataSending -> text().toStdString().data());
}
#pragma endregion}

#pragma region TCP Client Code {

void QtApp::on_btnSendTCPClient_clicked()
{

    //Get content of the text box and sent it as ASCII code
    socket -> write(ui -> txbSendDataTCPClient -> toPlainText().toLatin1());
    socket -> flush();
}


void QtApp::on_btnConnectTCPClient_clicked()
{
    if (ui -> btnConnectTCPClient -> text() == tr("Connect")){
        QString IP;
        int port;

        //Get IP
        IP = ui -> txbIPClient -> text();
        //Get Port number
        port = ui -> txbPortClient -> text().toInt();

        //Cancel existing connection:
        socket -> abort();
        //Connect to servers
        socket -> connectToHost(IP, port);

        //Waiting for the connection to succedd
        if (!socket -> waitForConnected(30000)){

            QString err = "Connection failed! \n Error: " + socket -> errorString();
            ui -> statTCPClient -> setText(err);
            ui -> statTCPClient -> setStyleSheet("Color: red");
            return;
        } else{
            ui -> statTCPClient -> setText("Connect successfully!");
            ui -> statTCPClient -> setStyleSheet("Color: black");

            //Send key enable:
            ui -> btnSendTCPClient -> setEnabled(true);
            // Modify keyword text
            ui -> btnConnectTCPClient -> setText("Disconnect");

        }


    }
    else{
        //Disconnect
        socket -> disconnectFromHost();

        //Modifiy keywork
        ui -> btnConnectTCPClient -> setText("Connect");
        ui -> btnSendTCPClient -> setEnabled(false);

    }
}

void QtApp::socketClientRead(){
    QByteArray buffer;
    //Read Buffer Data
    buffer = socket -> readAll();
    if(!buffer.isEmpty()){
        QString str = ui -> txbRecvDataTCPClient -> toPlainText();
        str += tr(buffer);
        // Refress display
        ui -> txbRecvDataTCPClient -> setText(str);

    }
}

void QtApp::socketClientDisconnected(){
    //Send button failure
    ui -> btnSendTCPClient -> setEnabled(false);
    //Modify kewword
    ui -> btnConnectTCPClient -> setText("Connect");
    //Status
     ui -> statTCPClient -> setText("Disconnect");
     ui -> statTCPClient -> setStyleSheet("Color: red");
}
#pragma endregion TCPClient}

#pragma region TCP Server Code{
void QtApp::on_btnListenTCPServer_clicked()
{
    if (ui -> btnListenTCPServer -> text() == tr("Listen")){
        //Get the port number
        int port = ui -> txbPortServer -> text().toInt();
        // Listen on the specified port
        if (!server -> listen(QHostAddress::Any, port)){
            //if an error occurs
            QString err = "Error: " + server -> errorString();
            ui -> statTCPServer -> setText(err);
            ui -> statTCPServer -> setStyleSheet("Color: red");
            return;
        }

            //Modify keyword text
            ui -> btnListenTCPServer -> setText("Cancel");

            QString text = "Listening on port " + ui -> txbPortServer -> text();
            text += " ...  ";
            ui -> statTCPServer -> setText(text);
            ui -> statTCPServer -> setStyleSheet("Color: black");

    }
    else{

        //cancel listen
//        try {
            //If you are connecting
            if (socketServer -> state() == QAbstractSocket::ConnectedState){
                //Close connection
                socketServer -> disconnectFromHost();
            }

            server -> close();

            //Modify keyword

            ui -> btnListenTCPServer -> setText("Listen");
            //Send button failure
            ui -> btnSendTCPServer -> setEnabled(false);

//        }  catch (QString* ex) {
//            QString textErr = "Error when cancel listen on port " + ui -> txbPortServer -> text();
//            textErr += "\n";
//            textErr += ex;
//            ui -> statTCPServer -> setText(textErr);
//            ui -> statTCPServer -> setStyleSheet("Color: red");
//        }



    }
}

void QtApp::on_btnSendTCPServer_clicked()
{
    //Get the content and sent it as ASCII Code
    socketServer -> write(ui -> txbSendDataTCPServer -> toPlainText().toLatin1());
    socketServer -> flush();
}


void QtApp::serverNewConnect(){
    //Getting Client Connection
    socketServer = server->nextPendingConnection();
    //Connect the signal slot of the QTcpSocket to read the new data
    QObject::connect(socketServer, &QTcpSocket::readyRead, this, &QtApp::socketServerRead);
    QObject::connect(socketServer, &QTcpSocket::disconnected, this, &QtApp::socketServerDisconnected);
    //Send key enable
    ui->btnSendTCPServer->setEnabled(true);
    QString text = "A client connected on port " + ui -> txbPortServer ->text();
    text += " ! ";
    ui -> statTCPServer -> setText(text);

}

void QtApp::socketServerRead(){
    QByteArray buffer;
    //Read buffer data
    buffer = socketServer -> readAll();
    if (!buffer.isEmpty()){
        QString str = ui -> txbRecvDataTCPServer -> toPlainText();
        str += tr(buffer);
        //Refress display
        ui -> txbRecvDataTCPServer -> setText(str);
    }
}
void QtApp::socketServerDisconnected(){
    //Send failure
    ui -> btnSendTCPServer -> setEnabled(false);
    ui -> statTCPServer -> setText("Connection closed!  ");

}

#pragma endregion}






