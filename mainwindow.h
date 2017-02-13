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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QtWidgets>
#include "fenset.h"
#include "fensetmac.h"
#include "fensetip.h"
#include "fensetsrcmac.h"
#include "freqwindow.h"
#include "portwindows.h"
#include "clientthread.h"
#include "senderthread.h"
#include "datadispatcher.h"
#include "connexion.h"

class portWindows;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void clearStats(QString);
    void sendOrder(QByteArray);
    void portToSender(int);

private slots:
    portWindows *createportWindows(int port);
    void actStart();
    void actStr();
    void actStop();
    void actStp();
    void actClear();
    void actClr();
    void actReset();
    void actRst();
    void actSet();
    void actSetMac();
    void actSetProto();
    void actSetType();
    void actSetIp();
    void actSetDstMac();
    void actSetSrcMac();
    void actSetSrcIp();
    void actSetDstIp();
    void actSetSrcPort();
    void actSetDstPort();
    void actSetWlanId();
    void setFrequencyMode();
    void frequencyMode();
    void proposdpdk();
    void propospktgen();
    void proposqt();
    void connexionDown();
    void transactionDown();
    void aboutPktgenClient();
    void init_Connexion();
    void updateDisplayMenu();
    void connexion(QString address);
    void connectedProcess();
    void updatePort(int nmb);
    void initSubWindows(int port);

private:
    Ui::MainWindow *ui;
    QMdiArea *zoneCentrale;
    int nmbPortint;
    QThread *clientThread;
    portWindows *activeMdiChild() const;
    DataDispatcher *dataDispatcher;
    QLabel *connexionState;
    SenderThread *senderThread;
    void enDisMenu(const bool stat);
};

#endif // MAINWINDOW_H
