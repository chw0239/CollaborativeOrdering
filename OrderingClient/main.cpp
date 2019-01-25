#include "clientwidget.h"
#include "ipdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IpDialog w;
    w.show();

    return a.exec();
}
