#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_start->setEnabled(true);
    ui->pb_stop->setEnabled(false);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendMsgToGUI, this, [output=ui->te_result](QString msg) {
        output->append(msg);
    });

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

//        QByteArray dataToSend;
//        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

//        outStr << dateTime;

        udpWorker->SendDatagram(dateTime);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    udpWorker->Rebind();
    timer->start(TIMER_DELAY);
    ui->pb_start->setEnabled(false);
    ui->pb_stop->setEnabled(true);
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
    ui->pb_start->setEnabled(true);
    ui->pb_stop->setEnabled(false);
    udpWorker->Close();
}


void MainWindow::on_pb_clean_clicked()
{
    ui->te_result->clear();
}

void MainWindow::on_pb_send_clicked()
{
    udpWorker->SendDatagram(ui->te_input->toPlainText());
    ui->te_input->clear();
}
