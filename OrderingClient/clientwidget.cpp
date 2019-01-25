#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent,  QHostAddress address, int port):
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    setWindowTitle("Collaborative Ordering - Client          by Haowei Cao & Ming Yan");

    orderList.clear();
    menuList.clear();
    selectedOrder=-1;

    menutype[0]=ui->verticalLayout_0;
    menutype[1]=ui->verticalLayout_1;
    menutype[2]=ui->verticalLayout_2;
    menutype[3]=ui->verticalLayout_3;

    menuPage[0]=ui->scrollArea_0;
    menuPage[1]=ui->scrollArea_1;
    menuPage[2]=ui->scrollArea_2;
    menuPage[3]=ui->scrollArea_3;

    for(int i=0;i<4;i++)
    {
        menuPage[i]->hide();
    }

    QFile file(":/xml/res/menu.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"error read xml";

    }
    reader=new QXmlStreamReader();
    reader->setDevice(&file);

    while(!reader->atEnd()){
        if(reader->isStartElement())
        {
            if(reader->name() == "menu")
            {
                qDebug()<<"read menu";
                readMenu();
            }
            else
            {
                Q_ASSERT(reader->isStartElement() && (reader->name() == "menu"));
                reader->raiseError(QObject::tr("Not a menu"));
            }
        }
        else
        {
            reader->readNext();
        }
    }
    if(reader->hasError())
    {
        qDebug()<<reader->errorString();
    }

    ui->scrollAreaWidgetContents_0->setMinimumHeight(menutype[0]->count()*Dish::minh);
    ui->scrollAreaWidgetContents_1->setMinimumHeight(menutype[1]->count()*Dish::minh);
    ui->scrollAreaWidgetContents_2->setMinimumHeight(menutype[2]->count()*Dish::minh);
    ui->scrollAreaWidgetContents_3->setMinimumHeight(menutype[3]->count()*Dish::minh);

    for(int i=0;i<4;i++)
    {
        QSpacerItem* si=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);
        menutype[i]->addSpacerItem(si);
    }

    showMenu(0);
    current=ui->pushButton_1;

    connectToServer(address,port);


}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::readMenu(){
    reader->readNext();
    while(!reader->atEnd()){
        if(reader->isEndElement()){
            reader->readNext();
            break;
        }
        if(reader->isStartElement()){
            if(reader->name() == "entry")
                readEntry();
            else
                reader->raiseError(QObject::tr("no entry")+reader->name().toString());
        }
        else
            reader->readNext();
    }
}

void ClientWidget::readEntry(){

    int type=reader->attributes().value("type").toString().toInt();
    if(type>3 || type<0)
    {
        qDebug()<<"error type";
        reader->raiseError("error type");
    }

    int price=reader->attributes().value("price").toString().toInt();
    QString name=reader->attributes().value("name").toString();
    int id=reader->attributes().value("id").toString().toInt();
    QString path=":/img/res/"+reader->readElementText();


    Dish * p=new Dish(this,name,path,price,id);
    menutype[type]->addWidget(p);
    menuList[p->id]=p;

    connect(p,SIGNAL(ordered(Dish*)),this,SLOT(addOrder(Dish*)));

    qDebug()<<"read dish: "<<p->name<<" at "<<p->imgpath<<" of type "<<type<<" of id "<<id;

    reader->readNext();
}

void ClientWidget::connectToServer(QHostAddress ip, quint16 port)
{
    socket=new QTcpSocket(this);
    dataStream.setDevice(socket);
    dataStream.setVersion(QDataStream::Qt_5_0);


    connect(socket,SIGNAL(connected()),this,SLOT(slotConnect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotErr(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotReadData()));

    socket->connectToHost(ip,port);




}

void ClientWidget::showMenu(int type)
{
    for(int i=0;i<4;i++)
    {
        if(i!=type)
            menuPage[i]->hide();

        else
            menuPage[i]->show();
    }
}

void ClientWidget::updateListWidget()
{
    ui->listWidget->clear();
    total=0;
    yours=0;

    for(int i =0;i<orderList.size();i++){
        Dish* dish=menuList[orderList.at(i).DishId];
        total+=dish->price;
        yours+= orderList.at(i).ClientId==clientID? dish->price : 0;
        QString itemstring = dish->name.leftJustified(19,' ')+ QString::number(dish->price)+
                QString::fromUtf8("€")+"  client "+QString::number(orderList.at(i).ClientId);

        qDebug()<<"    client "<<orderList.at(i).ClientId<<"  dish "<<orderList.at(i).DishId;

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(itemstring);
        newItem->setTextColor(colorList[orderList.at(i).ClientId]);


        ui->listWidget->addItem(newItem);
    }

    ui->label_2->setText(QString("Total: ")+ QString::number(total).rightJustified(3)+
                         QString::fromUtf8("€") );
    ui->label_3->setText(QString("Yours: ")+ QString::number(yours).rightJustified(3)+
                         QString::fromUtf8("€") );
}

void ClientWidget::addOrder(Dish *p)
{
    dataStream<< 1;
    dataStream<< (p->id);
    qDebug()<<"send "<<p->id;
}

void ClientWidget::slotConnect()
{
    ui->label->setText("Server connected.");
}

void ClientWidget::slotErr(QAbstractSocket::SocketError err)
{
    ui->label->setText("Socket error.");
    switch (err) {
    case QAbstractSocket::RemoteHostClosedError:
        ui->label->setText("RemoteHostClosedError.");
        break;
    case QAbstractSocket::HostNotFoundError:
        ui->label->setText("HostNotFoundError.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        ui->label->setText("ConnectionRefusedError.");
        break;
    default:
        ui->label->setText(socket->errorString());
    }
}


void ClientWidget::slotReadData()
{
    dataStream.startTransaction();

    int flag;
    dataStream>>flag;
    if(flag==0){
        dataStream >> clientID;
        ui->label->setText("Client ID: "+QString::number(clientID));
        int count;
        dataStream>>count;
        qDebug()<<count;
        orderList.clear();
        for(int i=0;i<count;i++){
            int clientid,dishid;
            dataStream>>clientid;
            dataStream>>dishid;
            orderList.append(OrderInfo(dishid,clientid));
        }
    }
    else if(flag==1){
        int count;
        dataStream>>count;
        qDebug()<<count;
        orderList.clear();
        for(int i=0;i<count;i++){
            int clientid,dishid;
            dataStream>>clientid;
            dataStream>>dishid;
            orderList.append(OrderInfo(dishid,clientid));
        }
    }
    else{

    }

    if (!dataStream.commitTransaction()){
        qDebug()<<"data not complet.";
        return;
    }

    if(flag==0){
        qDebug()<<"greeting received";
        qDebug()<<orderList.size();
        updateListWidget();

    }
    else if(flag==1){
        qDebug()<<"data received";
        qDebug()<<orderList.size();
        updateListWidget();
    }
    else{
        qDebug()<<"confirmation received";
        QString info= "Your orders are received!\n\n";
        for(int i =0;i<orderList.size();i++){
            info+= (menuList[ orderList.at(i).DishId]->name+"\n");
        }

        QMessageBox::question(this, tr("Confirmation"),
                              info,
                              QMessageBox::Ok);
        QApplication::quit();

    }



}

void ClientWidget::on_pushButton_1_clicked()
{
    current->setDefault(false);
    showMenu(0);
    ui->pushButton_1->setDefault(true);
    current=ui->pushButton_1;
}

void ClientWidget::on_pushButton_2_clicked()
{
    current->setDefault(false);
    showMenu(1);
    ui->pushButton_2->setDefault(true);
    current=ui->pushButton_2;
}

void ClientWidget::on_pushButton_3_clicked()
{
    current->setDefault(false);
    showMenu(2);
    ui->pushButton_3->setDefault(true);
    current=ui->pushButton_3;
}

void ClientWidget::on_pushButton_4_clicked()
{
    current->setDefault(false);
    showMenu(3);
    ui->pushButton_4->setDefault(true);
    current=ui->pushButton_4;
}

void ClientWidget::on_pushButton_delete_clicked()
{


    int curRow=ui->listWidget->currentRow();
    if(curRow==-1)
        return;

    Dish* dish=menuList[orderList.at(curRow).DishId];
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("Delete order"),
                                                  QString("Are you sure to delete the order?\n")+dish->name+" ordered by Client "+QString::number(orderList.at(curRow).ClientId),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::No)) {

        dataStream<<2;
        dataStream<< curRow << orderList.at(curRow).ClientId << orderList.at(curRow).DishId;
        qDebug()<<"delete "<<curRow << orderList.at(curRow).ClientId << orderList.at(curRow).DishId;
    }
}

void ClientWidget::on_pushButton_confirm_clicked()
{
    if(total==0)
    {
        QMessageBox::question(this, tr(" "),
                              tr("Please order something!"),
                              QMessageBox::Ok);
        return;
    }

    if (QMessageBox::Yes ==QMessageBox::question(this, tr("Confirmation"),
                          tr("Are you sure to confirm your orders?"),
                          QMessageBox::Yes | QMessageBox::No,
                                                 QMessageBox::Yes)){

        dataStream<<3;
    }
}

