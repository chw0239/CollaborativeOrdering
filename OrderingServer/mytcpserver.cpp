#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent): QTcpServer(parent)
{
    listen(QHostAddress::Any,22222);
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{

    qDebug()<<"in coming connection "<<socketDescriptor;
    MyTcpSocket* clientSocket = new MyTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    QDataStream* clientStream=new QDataStream(clientSocket);
    clientStream->setVersion(QDataStream::Qt_5_0);
    clientStreams[socketDescriptor]=clientStream;
    clientIndex[socketDescriptor]=clients.size()+1;

    connect(clientSocket,&MyTcpSocket::myDisconnectedSignal,this,&MyTcpServer::clientDisconnect);
    connect(clientSocket,&MyTcpSocket::myReadyReadSignal,this,&MyTcpServer::receiveOrder);

    mutex.lock();
    clients.append(clientSocket);

    *clientStream<< (int)0;
    *clientStream<< clients.size();


    *clientStream<< orderList.size();
    for(int j=0;j<orderList.size();++j){
        *(clientStream)<<orderList.at(j).ClientId<<orderList.at(j).DishId;

    }

    mutex.unlock();
}

void MyTcpServer::broadcastOrderList()
{
    for (int i = 0; i < clients.size(); ++i)
    {
        QDataStream* datastream=(clientStreams[clients.at(i)->socketDescriptor()]);
        *(datastream) << (int)1
                      << orderList.size();

        for(int j=0;j<orderList.size();++j){
            *(datastream)<<orderList.at(j).ClientId<<orderList.at(j).DishId;

        }

    }

}

void MyTcpServer::broadcastConfirm()
{
    for (int i = 0; i < clients.size(); ++i)
    {
        QDataStream* datastream=(clientStreams[clients.at(i)->socketDescriptor()]);
        *(datastream) << (int)2;

    }
}

void MyTcpServer::receiveOrder(qintptr sockfd)
{
    if(finished)
        return;
    mutex.lock();
    QDataStream* datastream= clientStreams[sockfd];

    int operationType;
    OrderInfo orderInfo;
    int deleteIndex=-1, deleteDishID=-1, deleteDishClientID=-1;

    datastream->startTransaction();

    *datastream>> operationType;
    if(operationType==1){ // add
        *datastream>>orderInfo.DishId;
        orderInfo.ClientId=clientIndex[sockfd];
    }
    else if (operationType==2){ // delete
        *datastream>> deleteIndex;
        *datastream>> deleteDishClientID;
        *datastream>> deleteDishID;
    }
    else{//confirm

    }
    if (!datastream->commitTransaction()){
        qDebug()<<"data not complet.";
        return;
    }
    if(operationType==1){
        qDebug()<<"receive "<<orderInfo.DishId;
        orderList.append(orderInfo);
        broadcastOrderList();
    }
    else if (operationType==2){
        qDebug()<<"delete "<<deleteIndex<<deleteDishClientID<<deleteDishID;
        if(orderList[deleteIndex].ClientId==deleteDishClientID &&
                orderList[deleteIndex].DishId==deleteDishID)
        {
            orderList.remove(deleteIndex);
        }
        else{
            for(int i=0;i<orderList.size();i++){
                if(orderList[i].ClientId==deleteDishClientID&&
                        orderList[i].DishId==deleteDishID){
                    orderList.remove(i);
                    break;
                }
            }
        }
        broadcastOrderList();
    }
    else{//confirm
        qDebug()<<"confirmed";
        broadcastConfirm();
        finished=true;
    }


    mutex.unlock();


}

void MyTcpServer::clientDisconnect(qintptr sockfd)
{
    qDebug()<<"delete client "<<sockfd;
    for (int i = 0; i < clients.size(); ++i)
    {
        if (clients.at(i)->socketDescriptor() == sockfd){
            clients.at(i)->deleteLater();
            clients.removeAt(i);

        }
    }
}

