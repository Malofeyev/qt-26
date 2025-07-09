#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345

class UDPworker : public QObject
{
private:
    enum class DataType : uint8_t{
        DATE = 0,
        STRING = 1
    };

    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram(QNetworkDatagram datagram);
    void SendDatagram(QDateTime date);
    void SendDatagram(const QString& str);
    void Close();
    void Rebind();


private slots:
    void readPendingDatagrams(void);

private:
    QString HandleDate(QDateTime date);
    QString HandleStringMsg(QHostAddress address, int port, int size);
    QUdpSocket* serviceUdpSocket;
    uint32_t counterPck = 0;
signals:
    void sig_sendMsgToGUI(QString data);

};

#endif // UDPWORKER_H
