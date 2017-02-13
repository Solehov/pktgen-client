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

#ifndef FENSETSRCMAC_H
#define FENSETSRCMAC_H

#include <QDialog>
#include <QtCore>
#include "senderthread.h"
#include "portwindows.h"


namespace Ui {
class Fensetsrcmac;
}

class Fensetsrcmac : public QDialog
{
    Q_OBJECT

public:
    explicit Fensetsrcmac(SenderThread *thread, int IDcode, QWidget *parent = 0);
    ~Fensetsrcmac();
signals:
    void sendOrder(QByteArray);
    void sendUpdateRange(int, int, QString, QString, QString, QString);
private slots:
    void createRequest();

private:
    int code;
    Ui::Fensetsrcmac *ui;
};

#endif // FENSETSRCMAC_H
