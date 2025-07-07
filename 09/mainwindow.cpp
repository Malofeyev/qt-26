#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , udpWorker(new UDPworker(this))
{
    ui->setupUi(this);
    udpWorker->InitSocket();
    connect(udpWorker, &UDPworker::sig_sendDatagramDataToGUI, this, [text = ui->tBrowser_receive](QString address ,int port, int size) {
        text->append(QString("Принято сообщение от %1:%2, размер сообщения(байт) %3").arg(address).arg(port).arg(size));
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_clear_clicked()
{
    ui->tBrowser_receive->clear();
}

void MainWindow::on_pb_send_clicked()
{
    QString text = ui->te_send->toPlainText();

    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << text;

    udpWorker->SendDatagram(dataToSend);
    ui->te_send->clear();
}
