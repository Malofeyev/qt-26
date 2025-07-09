#include "udpworker.h"

#include <QDataStream>

QString UDPworker::HandleDate(QDateTime date) {
    counterPck++;
//    if(counterPck % 20 == 0){
//        ui->te_result->clear();
//    }

    return QString("Текущее время: %1. Принято пакетов %2").arg(date.toString()).arg(counterPck);
}

QString UDPworker::HandleStringMsg(QHostAddress address, int port, int size) {
    return QString("Принято сообщение от %1:%2, размер сообщения(байт) %3").arg(address.toString()).arg(port).arg(size);
}

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{


}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    //serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    //connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    uint8_t int_type;
    inStr >> int_type;
    DataType type = static_cast<DataType>(int_type);

    QString msg;
    switch (type) {
        case DataType::DATE: {
            QDateTime date;
            inStr >> date;
            msg = HandleDate(date);
            break;
        }
        case DataType::STRING: {
            QString str_data;
            inStr >> str_data;
            msg = HandleStringMsg(datagram.senderAddress(), datagram.senderPort(), data.size());
            break;
        }
        default:
            msg = "Undefined input type";
    }
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendMsgToGUI(msg);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QDateTime date)
{
    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << static_cast<uint8_t>(DataType::DATE) << date;

    serviceUdpSocket->writeDatagram(dataToSend, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendDatagram(const QString& str)
{
    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << static_cast<uint8_t>(DataType::STRING) << str;
    if (serviceUdpSocket->isOpen()) {
        serviceUdpSocket->writeDatagram(dataToSend, QHostAddress::LocalHost, BIND_PORT);
    } else {
        QUdpSocket socket;
        socket.writeDatagram(dataToSend, QHostAddress::LocalHost, BIND_PORT);
    }
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::Close() {
    serviceUdpSocket->close();
}

void UDPworker::Rebind() {
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT, QUdpSocket::ReuseAddressHint);
    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}
