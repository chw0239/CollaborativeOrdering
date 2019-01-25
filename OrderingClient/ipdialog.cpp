#include "ipdialog.h"
#include "ui_ipdialog.h"

IpDialog::IpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpDialog)
{
    ui->setupUi(this);
}

IpDialog::~IpDialog()
{
    delete ui;
}

void IpDialog::on_buttonBox_accepted()
{
    QHostAddress add;
    QString ip=ui->lineEdit->text();
    if(ip.compare("localhost",Qt::CaseInsensitive)==0)
        add=QHostAddress::LocalHost;
    else
        add.setAddress(ip);
    if(add.isNull())
    {
        ui->label_3->setText("IP not valid");
        return;
    }

    bool ok=false;
    int port= ui->lineEdit_2->text().toInt(&ok);
    if(!ok){
        ui->label_3->setText("Port not valid");
        return;
    }

    ClientWidget* cw = new ClientWidget(nullptr,add,port);
    cw->show();
    this->hide();



}

void IpDialog::on_buttonBox_rejected()
{
    QApplication::quit();
}
