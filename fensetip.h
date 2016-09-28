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

#ifndef FENSETIP_H
#define FENSETIP_H

#include <QDialog>
#include <QtCore>
#include "senderthread.h"


namespace Ui {
class FenSetIp;
}

class FenSetIp : public QDialog
{
    Q_OBJECT

public:
    explicit FenSetIp(SenderThread *thread, QWidget *parent = 0);
    ~FenSetIp();
signals:
    void sendOrder(QByteArray);

private slots:
    void createRequest();

private:
    Ui::FenSetIp *ui;
};

#endif // FENSETIP_H
