#include "tcpclient.h"




/* ServiceHeader
 * Для работы с потоками наши данные необходимо сериализовать.
 * Поскольку типы данных не стандартные перегрузим оператор << Для работы с ServiceHeader
*/
QDataStream &operator >>(QDataStream &out, ServiceHeader &data){

    out >> data.id;
    out >> data.idData;
    out >> data.status;
    out >> data.len;
    return out;
};
QDataStream &operator <<(QDataStream &in, ServiceHeader &data){

    in << data.id;
    in << data.idData;
    in << data.status;
    in << data.len;

    return in;
};



/*
 * Поскольку мы являемся клиентом, инициализацию сокета
 * проведем в конструкторе. Также необходимо соединить
 * сокет со всеми необходимыми нам сигналами.
*/
TCPclient::TCPclient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &TCPclient::ReadyRead);

    connect(socket, &QTcpSocket::connected, this, [&]() {
        emit sig_connectStatus(STATUS_SUCCES);
    });

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, [&](QAbstractSocket::SocketError error) {
        emit sig_connectStatus(ERR_CONNECT_TO_HOST);
    });

    connect(socket, &QTcpSocket::disconnected, this, [&]() {
        //emit sig_connectStatus(ERR_CONNECT_TO_HOST);
        emit sig_Disconnected();
    });
}

/* write
 * Метод отправляет запрос на сервер. Сериализировать будем
 * при помощи QDataStream
*/
void TCPclient::SendRequest(ServiceHeader head)
{
    QByteArray sendHdr;
    QDataStream outStream(&sendHdr, QIODevice::WriteOnly);

    outStream << head;

    socket->write(sendHdr);
}

/* write
 * Такой же метод только передаем еще данные.
*/
void TCPclient::SendData(ServiceHeader head, QString str)
{
    QByteArray sendHdr;
    QDataStream outStream(&sendHdr, QIODevice::WriteOnly);

    outStream << head;
    outStream << str;

    socket->write(sendHdr);

}

/*
 * \brief Метод подключения к серверу
 */
void TCPclient::ConnectToHost(QHostAddress host, uint16_t port)
{
    socket->connectToHost(host, port);
}
/*
 * \brief Метод отключения от сервера
 */
void TCPclient::DisconnectFromHost()
{
    socket->disconnectFromHost();
}


void TCPclient::ReadyRead()
{

    QDataStream incStream(socket);

    if(incStream.status() != QDataStream::Ok){
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Ошибка открытия входящего потока для чтения данных!");
        msg.exec();
    }


    //Читаем до конца потока
    while(incStream.atEnd() == false){
        //Если мы обработали предыдущий пакет, мы скинули значение idData в 0
        if(servHeader.idData == 0){

            //Проверяем количество полученных байт. Если доступных байт меньше чем
            //заголовок, то выходим из обработчика и ждем новую посылку. Каждая новая
            //посылка дописывает данные в конец буфера
            if(socket->bytesAvailable() < sizeof(ServiceHeader)){
                return;
            }
            else{
                //Читаем заголовок
                incStream >> servHeader;
                //Проверяем на корректность данных. Принимаем решение по заранее известному ID
                //пакета. Если он "битый" отбрасываем все данные в поисках нового ID.
                if(servHeader.id != ID){
                    uint16_t hdr = 0;
                    while(!incStream.atEnd()){
                        incStream >> hdr;
                        if(hdr == ID){
                            incStream >> servHeader.idData;
                            incStream >> servHeader.status;
                            incStream >> servHeader.len;
                            break;
                        }
                    }
                }
            }
        }
        //Если получены не все данные, то выходим из обработчика. Ждем новую посылку
        if(socket->bytesAvailable() < servHeader.len){
            return;
        }
        else{
            //Обработка данных
            ProcessingData(servHeader, incStream);
            servHeader.idData = 0;
            servHeader.status = 0;
            servHeader.len = 0;
        }
    }
}


/*
 * Остался метод обработки полученных данных. Согласно протоколу
 * мы должны прочитать данные из сообщения и вывести их в ПИ.
 * Поскольку все типы сообщений нам известны реализуем выбор через
 * switch. Реализуем получение времени.
*/

void TCPclient::ProcessingData(ServiceHeader header, QDataStream &stream)
{
    switch (header.idData){

        case GET_TIME: {
            QDateTime time;
            stream >> time;
            emit sig_sendTime(time);
            break;
        }
        case GET_SIZE:
        case GET_STAT: {
            StatServer stat;
            stream >> stat.incBytes >> stat.sendBytes >> stat.revPck >> stat.sendPck >> stat.workTime >> stat.clients;
            emit sig_sendStat(stat);
            break;
        }
        case SET_DATA:
        case CLEAR_DATA:
        default:
            return;

        }

}
