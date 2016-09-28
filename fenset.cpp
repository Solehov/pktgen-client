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

#include "fenset.h"
#include "ui_fenset.h"

FenSet::FenSet(SenderThread *thread, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenSet)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(sendOrder(QByteArray)), thread, SLOT(OneRequest(QByteArray)));
    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(createRequest()));
}

FenSet::~FenSet()
{
    delete ui;
    qDebug()<<"fenset";
}

void FenSet::createRequest(){
    const QByteArray firstPartRequest = "pktgen.set(";
    QByteArray tempOption;
    QString tempPort = ui->Port->text();
    if (ui->burst->isChecked())
        tempOption = "\"burst\"";
    else if (ui->count->isChecked())
        tempOption = "\"count\"";
    else if (ui->dport->isChecked())
        tempOption = "\"dport\"";
    else if (ui->dump->isChecked())
        tempOption = "\"dump\"";
    else if (ui->prime->isChecked())
        tempOption = "\"prime\"";
    else if (ui->rate->isChecked())
        tempOption = "\"rate\"";
    else if (ui->seqCnt->isChecked())
        tempOption = "\"seqcnt\"";
    else if (ui->size->isChecked())
        tempOption = "\"size\"";
    else if (ui->sport->isChecked())
        tempOption = "\"sport\"";

    QString tempValue = ui->Valeur->text();
    QByteArray setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + tempOption + "," + tempValue.toUtf8() + ");\r\n");
    emit sendOrder(setRequest);
    this->close();
}
