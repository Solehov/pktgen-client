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

#include "connexion.h"
#include "ui_connexion.h"

Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion)
{
    ui->setupUi(this);
    ui->IP_Addr->setInputMask("000.000.000.000;");
    connect(this, SIGNAL(connectSignal(QString)), parent, SLOT(connexion(QString)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendAddress()));
    setAttribute(Qt::WA_DeleteOnClose);
}

Connexion::~Connexion()
{
    delete ui;
}

void Connexion::sendAddress(){
    emit connectSignal(ui->IP_Addr->text());
    this->close();
}
