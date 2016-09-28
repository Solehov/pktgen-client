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

#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QtCore>
#include <QtNetwork>
#include <QMutex>
#include "mainwindow.h"
#include "pipe.h"
#define PORT 22022
#define NO_RECEPTION 0xFF



class ClientThread : public QThread
{
    Q_OBJECT

public:
    explicit ClientThread(QString hostname);
    void run() Q_DECL_OVERRIDE;
    ~ClientThread();

signals:
    void connexionOk();
    void connexionNok();
    void transactionNok();

private slots:

private:
    QMutex ctmutex;
    volatile bool abort;
    QString host;

};

#endif // CLIENTTHREAD_H
