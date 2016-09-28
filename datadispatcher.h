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

#ifndef DATADISPATCHER_H
#define DATADISPATCHER_H
#include <QThread>
#include <QtCore>
#include <QtNetwork>

class DataDispatcher : public QThread
{
    Q_OBJECT

public:
    DataDispatcher(QObject *parent = 0);
    ~DataDispatcher();
    void run() Q_DECL_OVERRIDE;
    QByteArray getInfo(const int code);
    quint64 getPortSizeVal(int index);
    quint64 getPktStatsVal(int index);
    quint64 getPortStatsVal(int index);
    QString getLinkStateString();

signals:
    void tinfoNmbPort(int port);
    void updateInfoPortSize(int port);
    void updateInfoPktStats(int port);
    void updateInfoPortStats(int port);
    void infoLinkState(int port);

private slots:

private:
    bool abort;
    QMutex data;
    QByteArray Pktgen_Version;
    QByteArray Pktgen_Copyright;
    QByteArray Pktgen_Authors;
    QByteArray DPDK_Version;
    QByteArray DPDK_Copyright;
    QByteArray linkState;
    quint64 Port_Size [10];
    quint64 Pkt_Stats [8];
    quint64 Port_Stats [11];
    QMutex mutex;
};

#endif // DATADISPATCHER_H
