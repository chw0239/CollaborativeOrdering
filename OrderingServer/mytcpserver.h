#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHostAddress>
#include <QDebug>
#include <QMap>
#include <QDataStream>
#include <QMutex>
#include "mytcpsocket.h"

struct OrderInfo{
    int DishId;
    int ClientId;
    OrderInfo(int did=-1, int cid=-1): DishId(did),ClientId(cid) {}

};

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QVector<MyTcpSocket*> clients;
    QMap<qintptr,QDataStream*> clientStreams;
    QMap<qintptr,int> clientIndex;
    QMutex mutex;
    QVector<OrderInfo> orderList;

    void broadcastOrderList();
    void broadcastConfirm();
    bool finished=false;

private slots:

    void receiveOrder(qintptr sockfd);

    void clientDisconnect(qintptr sockfd);

signals:
    void serverReadDataSignal(qintptr, QString);
};

#endif // MYTCPSERVER_H
