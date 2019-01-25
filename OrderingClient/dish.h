#ifndef DISH_H
#define DISH_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

class Dish : public QFrame
{
    Q_OBJECT
public:
    explicit Dish(QWidget *parent , QString n, QString p, int pr, int idd);


    QLabel *img;
    QPushButton *button;
    QLabel *label;

    QString name;
    QString imgpath;
    int price;
    int id;

    const static int minh=220;

signals:
    void ordered(Dish* );

public slots:
    void orderedslot();
};

#endif // DISH_H
