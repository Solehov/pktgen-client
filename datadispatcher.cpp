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

#include "datadispatcher.h"
#include "pipe.h"
//#include <QtCore>

#define CASE_MASK 0x0F
#define PORT_MASK 0xF0

DataDispatcher::DataDispatcher(QObject *parent):QThread(parent)
{
    abort = false;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

DataDispatcher::~DataDispatcher(){
    data.lock();
    abort = true;
    data.unlock();
    qDebug()<<"datadispatch";
    wait();
}

QByteArray DataDispatcher::getInfo(const int code){
    QByteArray temp;
    switch (code)
    {
    case 0:
        data.lock();
        temp = this->Pktgen_Version;
        data.unlock();
        break;
    case 1:
        data.lock();
        temp = this->Pktgen_Copyright;
        data.unlock();
        break;
    case 2:
        data.lock();
        temp = this->Pktgen_Authors;
        data.unlock();
        break;
    case 3:
        data.lock();
        temp = this->DPDK_Version;
        data.unlock();
        break;
    default:
        data.lock();
        temp = this->DPDK_Copyright;
        data.unlock();
        break;
    }
    return temp;
}

quint64 DataDispatcher::getPortStatsVal(int index){
    quint64 temp;
    data.lock();
    temp = this->Port_Stats[index];
    data.unlock();
    return temp;
}

quint64 DataDispatcher::getPktStatsVal(int index){
    quint64 temp;
    data.lock();
    temp = this->Pkt_Stats[index];
    data.unlock();
    return temp;
}

quint64 DataDispatcher::getPortSizeVal(int index){
    quint64 temp;
    data.lock();
    temp = this->Port_Size[index];
    data.unlock();
    return temp;
}

QString DataDispatcher::getLinkStateString(){
    QString temp;
    data.lock();
    temp = QString::fromStdString(this->linkState.toStdString());
    this->linkState.clear();
    data.unlock();
    return temp;
}

void DataDispatcher::run(){
    QByteArray buffer, info;
    unsigned char converted_info;
    volatile unsigned int i = 0;
    volatile unsigned int index = 0;

    forever{
        if(abort){
            return;
        }
        buffer.clear();
        info.clear();
        converted_info = 255;
        if(i<DataSize){
            if(GetPipeUsed.tryAcquire(1,500)){
                buffer.append(GetPipe[i % BufferSize]);
                info.append(GetPipeStat[i % BufferSize]);
                GetPipeFree.release(1);
                i++;
            }
         }else{
            i=0;
            if(GetPipeUsed.tryAcquire(1,500)){
                buffer.append(GetPipe[i % BufferSize]);
                info.append(GetPipeStat[i % BufferSize]);
                GetPipeFree.release(1);
            }
         }
        if (!info.isEmpty()){
            converted_info = (unsigned char)info.toInt();
        }
        switch (converted_info & CASE_MASK){
        case 0x00:
            emit tinfoNmbPort(buffer.toInt());
            break;
        case 0x01:
            this->Pktgen_Version = buffer;
            break;
        case 0x02:
            this->Pktgen_Copyright = buffer;
            break;
        case 0x03:
            this->Pktgen_Authors = buffer;
            break;
        case 0x04:
            this->DPDK_Version = buffer;
            break;
        case 0x05:
            this->DPDK_Copyright = buffer;
            break;
        case 0x06:
            for (; index < 10; index++){
                data.lock();
                this->Port_Size[index] = buffer.toULongLong();
                data.unlock();
                index++;
                break;
            }
            if (index == 10){
                emit updateInfoPortSize((int)((converted_info & PORT_MASK) >> 4));
                index = 0;
            }
            break;
        case 0x07:
            for (; index < 8; index++){
                data.lock();
                this->Pkt_Stats[index] = buffer.toULongLong();
                data.unlock();
                index++;
                break;
            }
            if (index == 8){
                index = 0;
                emit updateInfoPktStats((int)((converted_info & PORT_MASK) >> 4));
            }
            break;
        case 0x08:
            for (; index < 11; ){
                data.lock();
                this->Port_Stats[index] = buffer.toULongLong();
                data.unlock();
                index++;
                break;
            }
            if (index == 11){
                emit updateInfoPortStats((int)((converted_info & PORT_MASK) >> 4));
                index = 0;
            }
            break;
        case 0x09:
            data.lock();
            this->linkState.append(buffer);
            data.unlock();
            emit infoLinkState((int)((converted_info & PORT_MASK) >> 4));
            break;
        default:
            break;
        }
    }
}
