#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QXmlStreamReader>
#include <QNetworkInterface>
#include "mytcpserver.h"

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();


private:
    Ui::ServerWidget *ui;
    MyTcpServer* tcpserver;
};

#endif // SERVERWIDGET_H
