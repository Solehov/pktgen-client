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


#include "portwindows.h"
#include "ui_portwindows.h"

portWindows::portWindows(int port, DataDispatcher *thread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::portWindows)
{
    ui->setupUi(this);
    this->nport = port;
    this->CThread = thread;
    this->maxRxPkt = 0;
    this->maxTxPkt = 0;
    connect(thread, SIGNAL(updateInfoPortSize(int)), this, SLOT(updateportSizeValue(int)));
    connect(thread, SIGNAL(updateInfoPktStats(int)), this, SLOT(updatePktStatsValue(int)));
    connect(thread, SIGNAL(updateInfoPortStats(int)), this, SLOT(updatePortStatsValue(int)));
    connect(thread, SIGNAL(infoLinkState(int)), this, SLOT(updateLinkState(int)));
    setAttribute(Qt::WA_DeleteOnClose);
}

portWindows::~portWindows()
{
    delete ui;
    qDebug()<< "portWindows";
}

void portWindows::newPort()
{
    curPort = tr("Port %1").arg(this->nport);
    setWindowTitle(curPort);
}

void portWindows::updateLinkState(int port){
    if(port != nport) return;
    QString test;

    ui->linkState->setText((CThread->getLinkStateString()));
}

void portWindows::updateportSizeValue(int port)
{
    if(port != nport)return;
    QString test;

    ui->runts->setText(test.setNum(CThread->getPortSizeVal(0)));
    ui->_64bytes->setText(test.setNum(CThread->getPortSizeVal(1)));
    ui->_64127bytes->setText(test.setNum(CThread->getPortSizeVal(2)));
    ui->_128255bytes->setText(test.setNum(CThread->getPortSizeVal(3)));
    ui->_256511bytes->setText(test.setNum(CThread->getPortSizeVal(4)));
    ui->_5121023bytes->setText(test.setNum(CThread->getPortSizeVal(5)));
    ui->_10241518bytes->setText(test.setNum(CThread->getPortSizeVal(6)));
    ui->jumbos->setText(test.setNum(CThread->getPortSizeVal(7)));
    ui->broadcast->setText(test.setNum(CThread->getPortSizeVal(8)));
    ui->Multicast->setText(test.setNum(CThread->getPortSizeVal(9)));

}

void portWindows::updatePktStatsValue(int port)
{
    if(port != nport)return;
    QString test;

    ui->arp->setText(test.setNum(CThread->getPktStatsVal(0)));
    ui->icmp->setText(test.setNum(CThread->getPktStatsVal(1)));

}

void portWindows::updatePortStatsValue(int port)
{
    if(port != nport)return;

    quint64 temp;
    QString test;
    temp = CThread->getPortStatsVal(0);
    ui->rateipacket->setText(test.setNum(temp));
    if(temp >= this->maxRxPkt) this->maxRxPkt=temp;
    ui->maxipacket->setText(test.setNum(this->maxRxPkt));
    temp = CThread->getPortStatsVal(1);
    ui->rateopacket->setText(test.setNum(temp));
    if(temp >= this->maxTxPkt) this->maxTxPkt=temp;
    ui->maxopacket->setText(test.setNum(this->maxTxPkt));
    ui->mbitsRx->setText(test.setNum(CThread->getPortStatsVal(9)));
    ui->mbitsTx->setText(test.setNum(CThread->getPortStatsVal(10)));
    ui->errorrx->setText(test.setNum(CThread->getPortStatsVal(4)));
    ui->errortx->setText(test.setNum(CThread->getPortStatsVal(5)));

}

/* INPROGRESS
void portWindows::updateRangeValue(int code, int port, QString start, QString inc, QString min, QString max){
    if(port != nport)return;
    switch (code){
    case 0:
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    case 1:
        ui->sourcePortStart->setText(start);
        ui->sourcePortInc->setText(inc);
        ui->sourcePortMin->setText(min);
        ui->sourcePortMax->setText(max);
        break;
    case 2:
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    case 3:
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    case 4:
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    case 5:
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    default :
        ui->vlanIDStart->setText(start);
        ui->vlanIDInc->setText(inc);
        ui->vlanIDMin->setText(min);
        ui->vlanIDMax->setText(max);
        break;
    }

}
*/
