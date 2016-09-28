/*
Copyright (C) 2016 Sailley Thomas

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License v3 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "fensetmac.h"
#include "ui_fensetmac.h"

FenSetMac::FenSetMac(SenderThread *thread, int IDcode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenSetMac)
{
    ui->setupUi(this);
    this->code = IDcode;
    switch (code){
    case 0:
        setWindowTitle("Set type...");
        ui->labeldes->setText("Set the port packet types to IPv4 or v6");
        ui->labelAny->setText("Type :");
        break;
    case 1:
        setWindowTitle("Set protocol...");
        ui->labeldes->setText("Set the packet protocol type");
        ui->labelAny->setText("Protocol :");
        break;
    default:
        ui->macinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        break;
    }
    connect(this, SIGNAL(sendOrder(QByteArray)), thread, SLOT(OneRequest(QByteArray)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(createRequest()));
    setAttribute(Qt::WA_DeleteOnClose);
}

FenSetMac::~FenSetMac()
{
    delete ui;
    qDebug()<<"fensetmac";
}

void FenSetMac::createRequest(){
    QByteArray firstPartRequest;
    QString tempPort = ui->portinfo->text();
    QString tempMac = ui->macinfo->text();
    switch(code){
    case 0:
        firstPartRequest = "pktgen.set_type(";
        break;
    case 1:
        firstPartRequest = "pktgen.set_proto(";
        break;
    default:
        firstPartRequest = "pktgen.set_mac(";
        break;
    }

    QByteArray setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + "\"" + tempMac.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    this->close();
}
