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

#include "senderthread.h"
#include <string.h>

SenderThread::SenderThread()
{
    this->infoReq.append("print(pktgen.portCount());\r\n");
    this->infoReq.append("print(pktgen.info.Pktgen_Version);\r\n");
    this->infoReq.append("print(pktgen.info.Pktgen_Copyright);\r\n");
    this->infoReq.append("print(pktgen.info.Pktgen_Authors);\r\n");
    this->infoReq.append("print(pktgen.info.DPDK_Version);\r\n");
    this->infoReq.append("print(pktgen.info.DPDK_Copyright);\r\n");

    this->getRequestSecondLevel.append("print(pktgen.portSizes(\"all\")[");
    this->getRequestSecondLevel.append("print(pktgen.pktStats(\"all\")[");
    this->getRequestSecondLevel.append("print(pktgen.portStats(\"all\", \"rate\")[");
    this->getRequestSecondLevel.append("print(pktgen.linkState(\"all\")[");

    this->portSizeReq.append("].runt);\r\n");
    this->portSizeReq.append("]._64);\r\n");
    this->portSizeReq.append("]._65_127);\r\n");
    this->portSizeReq.append("]._128_255);\r\n");
    this->portSizeReq.append("]._256_511);\r\n");
    this->portSizeReq.append("]._512_1023);\r\n");
    this->portSizeReq.append("]._1024_1518);\r\n");
    this->portSizeReq.append("].jumbo);\r\n");
    this->portSizeReq.append("].broadcast);\r\n");
    this->portSizeReq.append("].multicast);\r\n");

    this->Pkt_StatsReq.append("].arp_pkts);\r\n");
    this->Pkt_StatsReq.append("].echo_pkts);\r\n");
    this->Pkt_StatsReq.append("].ip_pkts);\r\n");
    this->Pkt_StatsReq.append("].ipv6_pkts);\r\n");
    this->Pkt_StatsReq.append("].vlan_pkts);\r\n");
    this->Pkt_StatsReq.append("].dropped_pkts);\r\n");
    this->Pkt_StatsReq.append("].unknown_pkts);\r\n");
    this->Pkt_StatsReq.append("].tx_failed);\r\n");

    this->Port_StatsReq.append("].ipackets);\r\n");
    this->Port_StatsReq.append("].opackets);\r\n");
    this->Port_StatsReq.append("].ibytes);\r\n");
    this->Port_StatsReq.append("].obytes);\r\n");
    this->Port_StatsReq.append("].ierrors);\r\n");
    this->Port_StatsReq.append("].oerrors);\r\n");
    this->Port_StatsReq.append("].rx_nombuf);\r\n");
    this->Port_StatsReq.append("].pkts_rx);\r\n");
    this->Port_StatsReq.append("].pkts_tx);\r\n");
    this->Port_StatsReq.append("].mbits_rx);\r\n");
    this->Port_StatsReq.append("].mbits_tx);\r\n");

    this->linkStateReq = "]);\r\n";
    this->ReqID = 1;
    abort = false;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

SenderThread::~SenderThread(){
    qDebug()<<"SenderThread";
    mutex.lock();
    abort = true;
    mutex.unlock();
    wait();
}

void SenderThread::startFreqMode(bool flag){
    qDebug()<<"ok";
    if ( !flag ){
        data.lock();
        this->ReqID = 0;
        data.unlock();
    } else {
        data.lock();
        this->ReqID = 2;
        data.unlock();
        qDebug()<< ReqID;
    }
}

void SenderThread::updatePort(int nmb){
    data.lock();
    this->nmbPort = nmb;
    qDebug()<< nmbPort << "APPELLE";
    data.unlock();
}

void SenderThread::updateTime(int time){
    data.lock();
    this->msTime = time;
    data.unlock();
}

void SenderThread::OneRequest(const QByteArray order){
    this->OneReq = order;
    data.lock();
    this->ReqID = 3;
    data.unlock();
}

void SenderThread::RangeRequest(const QByteArray rangeOrders){
    static int k;
    data.lock();
    this->stack4Request.append(rangeOrders);
    data.unlock();
    k++;
    if( k<4 ){
        return;
    }else{
        k = 0;
        data.lock();
        this->ReqID = 4;
        data.unlock();
    }
}

void SenderThread::run(){
    volatile unsigned char prevMode;
    QByteArray buffer, info, portnmb, temp;
    volatile unsigned int k, l, m, port, mode, process;
    //volatile bool flagTimer = false;
    port = 0;
    k = 0;
    l = 0;
    m = 0;
    process = 0;
    volatile unsigned int i = 0;
    volatile unsigned int position = 0;
    QByteArray startreq = "pktgen.start(\"all\");\r\n";
    mode = 1;

    forever{
        if(abort){
            qDebug()<<"return from sender";
            return;
        }
        temp.clear();
        buffer.clear();
        info.clear();
        portnmb.clear();
        port = this->nmbPort;
        mode = ReqID;
        if ((prevMode == 2) && (mode != 2)){
            qDebug()<< "return";
            process = 2;
            mode = 2;
        }
        switch(mode){
        case 1:
            if (i != NMB_INFO_REQ){
                temp = this->infoReq.at(i);
                info.setNum(i);
                i++;
            }else{
                i = 0;
                ReqID = 0;
            }
            break;
        case 2:
            switch(process){
            case 1:
                msleep(msTime);
                process--;
                break;
            case 2:
                qDebug()<<"normalement ok";
                process = 0;
                mode = 0;
                ReqID = 0;
                break;
            default:
                temp = startreq;
                info.setNum(255);
                process++;
                break;
            }
            break;
        case 3:
            temp = OneReq;
            OneReq.clear();
            info.setNum(255);
            ReqID = 0;
            break;
        case 4:
            if ( m < 4 ){
                temp = this->stack4Request.at(m);
                info.setNum(255);
                m++;
            } else {
                m = 0;
                this->stack4Request.clear();
                ReqID = 0;
            }
            break;
        default:
            if (port == 0) break;
            if (k < port){
                if (l != 4){
                    switch(l)
                    {
                        /**********************************************************************************************/
                        /*                                  Port Size Refresh                                         */
                        /**********************************************************************************************/
                        case 0:
                        {
                            buffer = this->getRequestSecondLevel.at(l);
                            buffer = buffer + portnmb.setNum(k);
                            if (i != NMB_PORTSIZE_REQ){
                                temp = buffer + this->portSizeReq.at(i);
                                info.setNum(0x6|(k << 4));
                                i++;
                             }else{
                                i = 0;
                                l++;
                            }
                         break;
                         }
                             /**********************************************************************************************/
                             /*                                  Pkt Stats Refresh                                         */
                             /**********************************************************************************************/
                         case 1:
                         {
                            buffer = this->getRequestSecondLevel.at(l);
                            buffer = buffer + portnmb.setNum(k);
                            if (i != NMB_PKTSTATS){
                                temp = buffer + this->Pkt_StatsReq.at(i);
                                info.setNum(0x7|(k<<4));
                                i++;
                             }else{
                                i = 0;
                                l++;
                            }
                             break;
                          }
                                /**********************************************************************************************/
                                /*                                  Port Stats Refresh                                        */
                                /**********************************************************************************************/
                          case 2:
                          {
                            buffer = this->getRequestSecondLevel.at(l);
                            buffer = buffer + portnmb.setNum(k);
                            if (i != NMB_PORTSTATS){
                                temp = buffer + this->Port_StatsReq.at(i);
                                info.setNum(0x8|(k << 4));
                                i++;
                            }else{
                                i = 0;
                                l++;
                            }
                            break;
                           }
                                /**********************************************************************************************/
                                /*                                  LinkState Refresh                                         */
                                /**********************************************************************************************/
                           default:
                           {
                                buffer = this->getRequestSecondLevel.at(l);
                                buffer = buffer + portnmb.setNum(k);
                                temp = buffer + this->linkStateReq;
                                info.setNum(0x9|(k << 4));
                                l = 0;
                                k++;
                                if(abort)return;
                             break;
                           }
                       }
                   }
               }else{
                k = 0;
                }
            break;
        }
        if(temp.isEmpty()!=1){
            if(position<DataSize){
                if(ReqPipeFree.tryAcquire(1,500)){
                    qDebug()<< position << "SENDER"<< temp;
                    strcpy(&RequestPipe[position % BufferSize][0], temp.data());
                    strcpy(&RequestPipeStat[position % BufferSize][0], info.data());
                    ReqPipeUsed.release(1);
                    position++;
                }
            }else{
                position=0;
                if(ReqPipeFree.tryAcquire(1,500)){
                strcpy(&RequestPipe[position % BufferSize][0], temp.data());
                strcpy(&RequestPipeStat[position % BufferSize][0], info.data());
                ReqPipeUsed.release(1);
                }
            }
        }
        prevMode = mode;
        //prevId = ReqID;
        msleep(18);
    }
}
