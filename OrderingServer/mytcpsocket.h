#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket(QObject *parent = nullptr);

signals:
    void myDisconnectedSignal(qintptr);
    void myReadyReadSignal(qintptr);

private slots:
    void readyReadSlot();
    void disconnectedSlot();
};

#endif // MYTCPSOCKET_H
