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

#include "freqwindow.h"
#include "ui_freqwindow.h"

FreqWindow::FreqWindow(SenderThread *thread, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FreqWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(sendTime(int)), thread, SLOT(updateTime(int)));
    connect(this, SIGNAL(sendOrder(QByteArray)), thread, SLOT(OneRequest(QByteArray)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(createRequest()));
}

FreqWindow::~FreqWindow()
{
    delete ui;
}

void FreqWindow::createRequest(){
    const QByteArray firstPartRequest = "pktgen.set(";
    QString tempPort = ui->portNmb->text();
    QString tempValue = ui->nmbPacket->text();
    QString temp_msec = ui->time_ms->text();
    emit sendTime(temp_msec.toInt());

    QByteArray setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + ",\"count\"," + tempValue.toUtf8() + ");\r\n");
    emit sendOrder(setRequest);

    this->close();
}
