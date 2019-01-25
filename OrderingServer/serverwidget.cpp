#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    setWindowTitle("Collaborative Ordering - Server          by Haowei Cao & Ming Yan");

    tcpserver=new MyTcpServer(this);

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress += (" "+ipAddressesList.at(i).toString()+"\n");
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->label->setText(tr("The server is running on\n\nIP:\n%1\nport:\n %2\n\n")
                         .arg(ipAddress).arg(tcpserver->serverPort()));


}

ServerWidget::~ServerWidget()
{
    delete ui;
}
