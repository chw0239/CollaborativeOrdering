#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include "dish.h"
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

namespace Ui {
class ClientWidget;
}

struct OrderInfo{
    int DishId;
    int ClientId;
    OrderInfo(int did=-1, int cid=-1): DishId(did),ClientId(cid) {}
};

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr, QHostAddress address=QHostAddress::LocalHost, int port=22222);
    ~ClientWidget();

private:
    Ui::ClientWidget *ui;
    QTcpSocket* socket;
    int clientID=-1;

    void readMenu();
    void readEntry();
    void connectToServer(QHostAddress ip, quint16 port);
    void showMenu(int type);
    void updateListWidget();

    QVector<OrderInfo> orderList;
    QMap<int, Dish*> menuList;

    QXmlStreamReader *reader;
    QDataStream dataStream;

    QVBoxLayout * menutype[4];
    QScrollArea * menuPage[4];
    QPushButton* current;

    int total=0, yours=0;
    int selectedOrder;

    Qt::GlobalColor colorList[11] ={ Qt::black,Qt::red,Qt::blue,Qt::darkGreen,Qt::darkYellow,Qt::cyan,Qt::magenta,Qt::gray,Qt::darkBlue,Qt::green,Qt::darkRed};

public slots:
    void addOrder(Dish* p);
    void slotConnect();
    void slotErr(QAbstractSocket::SocketError err);
    void slotReadData();

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_confirm_clicked();

};

#endif // CLIENTWIDGET_H
