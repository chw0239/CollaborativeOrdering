#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent): QTcpSocket (parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
}

void MyTcpSocket::readyReadSlot()
{
    emit myReadyReadSignal(this->socketDescriptor());
}

void MyTcpSocket::disconnectedSlot()
{
    emit myDisconnectedSignal(this->socketDescriptor());
}
