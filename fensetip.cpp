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

#include "fensetip.h"
#include "ui_fensetip.h"

FenSetIp::FenSetIp(SenderThread *thread, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenSetIp)
{
    ui->setupUi(this);
    ui->IPinfo->setInputMask("000.000.000.000;");
    connect(this, SIGNAL(sendOrder(QByteArray)), thread, SLOT(OneRequest(QByteArray)));
    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(createRequest()));
    setAttribute(Qt::WA_DeleteOnClose);
}

FenSetIp::~FenSetIp()
{
    delete ui;
    qDebug()<<"fensetip";
}

void FenSetIp::createRequest(){
    const QByteArray firstPartRequest = "pktgen.set_ipaddr(";
    QString tempPort = ui->portinfo->text();
    QString tempIp = ui->IPinfo->text();
    QByteArray tempOption;

    if (ui->Src->isChecked())
        tempOption = "\"src\"";
    else if (ui->Dst->isChecked())
        tempOption = "\"dst\"";

    QByteArray setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + tempOption + "," + "\"" + tempIp.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    this->close();
}
