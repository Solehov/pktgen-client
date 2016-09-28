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

#ifndef SENDERTHREAD_H
#define SENDERTHREAD_H
#include <QThread>
#include <QtCore>
#include <QMutex>
#include "pipe.h"

#define INIT 0x00
#define ONE_REQ 0x01
#define RANGE_REQ 0x02
#define REFRESH_VALUES 0x03
#define FREQUENCY_MACRO 0x04
#define NMB_INFO_REQ 6
#define NMB_PORTSIZE_REQ 10
#define NMB_PKTSTATS 8
#define NMB_PORTSTATS 11
#define DEFAULT_CASE 0xFF
#define NO_RECEPTION 0xFF

class SenderThread : public QThread
{
    Q_OBJECT

public:
    SenderThread();
    ~SenderThread();
    void run() Q_DECL_OVERRIDE;
    void startFreqMode(bool flag);

signals:


private slots:
    void OneRequest(QByteArray order);
    void RangeRequest(const QByteArray rangeOrders);
    void updateTime(int time);
    void updatePort(int nmb);



private:
    QMutex mutex;
    QMutex data;
    int nmbPort;
    int msTime;
    volatile bool abort;
    volatile unsigned char ReqID;
    QByteArray OneReq;
    QVector < QByteArray > infoReq;
    QVector < QByteArray > getRequestSecondLevel;
    QVector < QByteArray > portSizeReq;
    QVector < QByteArray > Pkt_StatsReq;
    QVector < QByteArray > Port_StatsReq;
    QVector < QByteArray > stack4Request;
    QByteArray linkStateReq;

};

#endif // SENDERTHREAD_H
