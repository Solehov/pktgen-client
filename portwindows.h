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

#ifndef PORTWINDOWS_H
#define PORTWINDOWS_H
#include <QLineEdit>
#include <QtCore>
#include <QWidget>
#include "datadispatcher.h"

namespace Ui {
class portWindows;
}

class portWindows : public QWidget
{
    Q_OBJECT

public:
    explicit portWindows(int port, DataDispatcher *thread, QWidget *parent = 0);
    ~portWindows();

    void newPort();

private slots:
    void updateportSizeValue(int port);
    void updatePktStatsValue(int port);
    void updatePortStatsValue(int port);
    void updateLinkState(int port);
    //void updateRangeValue(int code, int port, QString start, QString inc, QString min, QString max);

private:
    Ui::portWindows *ui;
    int nport;
    QString curPort;
    DataDispatcher *CThread;
    quint64 maxRxPkt, maxTxPkt;
};

#endif // PORTWINDOWS_H
