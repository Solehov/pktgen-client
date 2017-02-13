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

#include "fensetsrcmac.h"
#include "ui_fensetsrcmac.h"

Fensetsrcmac::Fensetsrcmac(SenderThread *thread, int IDcode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fensetsrcmac)
{
    ui->setupUi(this);
    this->code = IDcode;
    switch (code){
    case 0:
        setWindowTitle("Set vlan id...");
        ui->labeldes->setText("Set the VLAN id in the range data");
        break;
    case 1:
        setWindowTitle("Set source port...");
        ui->labeldes->setText("Set the source port value in the range data");
        break;
    case 2:
        setWindowTitle("Set destination MAC...");
        ui->labeldes->setText("Set the destination MAC address for a given set of ports in the range data");
        ui->startinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->incinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->mininfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->maxinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        break;
    case 3:
        setWindowTitle("Set destination port...");
        ui->labeldes->setText("Set the destination port value in the range data");
        break;
    case 4:
        setWindowTitle("Set Source IP...");
        ui->labeldes->setText("Set the source IP address in the range data");
        ui->startinfo->setInputMask("000.000.000.000;");
        ui->incinfo->setInputMask("000.000.000.000;");
        ui->mininfo->setInputMask("000.000.000.000;");
        ui->maxinfo->setInputMask("000.000.000.000;");
        break;
    case 5:
        setWindowTitle("Set Destination IP...");
        ui->labeldes->setText("Set the destination IP address in the range data");
        ui->startinfo->setInputMask("000.000.000.000;");
        ui->incinfo->setInputMask("000.000.000.000;");
        ui->mininfo->setInputMask("000.000.000.000;");
        ui->maxinfo->setInputMask("000.000.000.000;");
        break;
    default :
        ui->startinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->incinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->mininfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        ui->maxinfo->setInputMask("HH:HH:HH:HH:HH:HH;");
        break;


    }
    //connect(this, SIGNAL(sendUpdateRange(int, int,QString,QString,QString,QString)), PortWindows, SLOT(updateRangeValue(int,int,QString,QString,QString,QString)));
    connect(this, SIGNAL(sendOrder(QByteArray)), thread, SLOT(RangeRequest(QByteArray)));
    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(createRequest()));
    setAttribute(Qt::WA_DeleteOnClose);
}

Fensetsrcmac::~Fensetsrcmac()
{
    delete ui;
    qDebug()<<"fensetsrcmac";
}

void Fensetsrcmac::createRequest(){

    QString tempPort = ui->portinfo->text();
    QString tempStart = ui->startinfo->text();
    QString tempInc = ui->incinfo->text();
    QString tempMin = ui->mininfo->text();
    QString tempMax = ui->maxinfo->text();
    QByteArray setRequest;
    QByteArray firstPartRequest;
    switch (code){
    case 0:     
        firstPartRequest = "pktgen.vlan_id(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    case 1:
        firstPartRequest = "pktgen.src_port(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    case 2:
        firstPartRequest = "pktgen.dst_mac(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    case 3:
        firstPartRequest = "pktgen.dst_port(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    case 4:
        firstPartRequest = "pktgen.src_ip(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    case 5:
        firstPartRequest = "pktgen.dst_ip(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;
    default :
        firstPartRequest = "pktgen.src_mac(";
        //sendUpdateRange(code, tempPort.toInt(), tempStart, tempInc, tempMin, tempMax);
        break;

    }
    setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + "\"start\"" + "," + "\"" +tempStart.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + "\"inc\"" + "," + "\"" +tempInc.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + "\"min\"" + "," + "\"" +tempMin.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    setRequest = (firstPartRequest + "\"" + tempPort.toUtf8() + "\"" + "," + "\"max\"" + "," + "\"" +tempMax.toUtf8() + "\"" + ");\r\n");
    emit sendOrder(setRequest);
    this->close();
}
