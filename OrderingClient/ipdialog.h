#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include "clientwidget.h"


namespace Ui {
class IpDialog;
}

class IpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpDialog(QWidget *parent = nullptr);
    ~IpDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::IpDialog *ui;
};

#endif // IPDIALOG_H
