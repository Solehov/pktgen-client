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

#include "clientthread.h"


ClientThread::ClientThread(QString hostname)
{
    abort = false;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    this->host = hostname;
}

ClientThread::~ClientThread(){    
    ctmutex.lock();
    abort = true;
    ctmutex.unlock();
    wait();
    qDebug()<<"ClientThread deleted";
}

void ClientThread::run(){

    QTcpSocket socket;
    QString address;
    QByteArray buffer, bufferresult, info;
    int m, pos;
    unsigned int i =0;
    unsigned int position = 0;

    /**********************************************************************************************/
    /*                                  Connection to Pktgen                                      */
    /**********************************************************************************************/

    socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    address = this->host;
    socket.connectToHost(address, PORT, QIODevice::ReadWrite);
    if (!socket.waitForConnected(TIMEOUT(1))) {
        emit connexionNok();
        return;
    }
    emit connexionOk();

    forever {
        if(abort){
            socket.close();
            return;
        }
        buffer.clear();
        bufferresult.clear();
        info.clear();

        if(i<DataSize){
            if(ReqPipeUsed.tryAcquire(1,500)){

                buffer.append(RequestPipe[i % BufferSize]);
                qDebug()<< i << "RECEIVER"<< buffer;
                info.append(RequestPipeStat[i % BufferSize]);
                ReqPipeFree.release(1);
                i++;
            }
        }else{
            i=0;
            if(ReqPipeUsed.tryAcquire(1,500)){
                buffer.append(RequestPipe[i % BufferSize]);
                info.append(RequestPipeStat[i % BufferSize]);
                ReqPipeFree.release(1);
            }
        }
        if (!buffer.isEmpty()){
            qDebug()<<buffer;
            if(socket.write(buffer)<0){
                emit transactionNok();
                return;
            }
            qDebug()<<"sended";
            if (info.toInt() != 255){
                do {
                    if (socket.waitForReadyRead(500) != 1){
                        emit transactionNok();
                        return;
                    }
                    while(!socket.atEnd()){
                        bufferresult = socket.readAll();
                    }
                } while(bufferresult == "\n");
                for(m=0; m<2; m++){
                    pos = bufferresult.indexOf("\n");
                    if(bufferresult.indexOf("\n") < 0){}
                    else{
                        bufferresult.remove(pos,1);
                    }
                }
                if(position<DataSize){
                    if(GetPipeFree.tryAcquire(1,500)){
                        strcpy(&GetPipe[position % BufferSize][0], bufferresult.data());
                        strcpy(&GetPipeStat[position % BufferSize][0], info.data());
                        GetPipeUsed.release();
                        position++;
                    }
                }else{
                    position=0;
                    if(GetPipeFree.tryAcquire(1,500)){
                        strcpy(&GetPipe[position % BufferSize][0], bufferresult.data());
                        strcpy(&GetPipeStat[position % BufferSize][0], info.data());
                        GetPipeUsed.release();
                    }
                }
            }
        }           
    }
}


