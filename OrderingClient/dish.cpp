#include "dish.h"

Dish::Dish(QWidget *parent, QString n, QString p, int pr, int idd) : QFrame(parent)
{
    name=n;
    imgpath=p;
    price=pr;
    id=idd;

    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
    this->setMinimumHeight(minh);
    this->setFrameShape(QFrame::Box);
    this->setFrameShadow(QFrame::Plain);

    img=new QLabel(this);
    img->setGeometry(5,10,200,200);
    img->setPixmap(QPixmap(imgpath));
    img->setScaledContents(true);

    label=new QLabel(this);
    label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
    label->setGeometry(210,50,180,50);
    label->setText(name+"\n"+QString::number(price)+QString::fromUtf8("€"));
    QFont f=label->font();
    f.setBold(false);
    f.setFamily("Cambria");
    f.setPointSize(14);
    label->setFont(f);
    label->setAlignment(Qt::AlignCenter);

    button=new QPushButton("Order it!",this);
    f.setPointSize(12);
    button->setFont(f);
    button->setGeometry(230,130,140,50);

    connect(button,SIGNAL(clicked()),this,SLOT(orderedslot()) );


}

void Dish::orderedslot()
{
    emit ordered(this);
}
