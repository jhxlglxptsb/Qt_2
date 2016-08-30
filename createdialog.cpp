#include "createdialog.h"
#include "ui_createdialog.h"
#include <QNetworkInterface>
#include <QtNetwork/QHostAddress>

CreateDialog::CreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDialog) {
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {

        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            qDebug()<< ipAddressesList.at(i).toString();
           // break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    ui->setupUi(this);
    ui->lineEdit->setText(ipAddress);
}

QString CreateDialog::getHostIpAddress() {
    return ui->lineEdit->text();
}

CreateDialog::~CreateDialog() {
    delete ui;
}
