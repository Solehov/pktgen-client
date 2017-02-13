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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connexionState = new QLabel;
    connexionState->setText("Disconnected");
    zoneCentrale = new QMdiArea;
    zoneCentrale->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    zoneCentrale->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(zoneCentrale);

    connect(zoneCentrale, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateDisplayMenu()));
    ui->statusBar->addPermanentWidget(connexionState);
    enDisMenu(false);
    ui->actionFrequency_mode->setEnabled(false);
    ui->actionSet_Frequency_mode->setEnabled(false);
    connect(ui->actionConnexion, SIGNAL(triggered()), this, SLOT(init_Connexion()));
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(actStart()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(actStop()));
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(actClear()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(actReset()));
    connect(ui->actionStart_all, SIGNAL(triggered()), this, SLOT(actStr()));
    connect(ui->actionStop_all, SIGNAL(triggered()), this, SLOT(actStp()));
    connect(ui->actionClear_all, SIGNAL(triggered()), this, SLOT(actClr()));
    connect(ui->actionReset_all, SIGNAL(triggered()), this, SLOT(actRst()));
    connect(ui->actionSet, SIGNAL(triggered()), this, SLOT(actSet()));
    connect(ui->actionSet_MAC_address, SIGNAL(triggered()), this, SLOT(actSetMac()));
    connect(ui->actionSet_protocol, SIGNAL(triggered()), this, SLOT(actSetProto()));
    connect(ui->actionSet_type, SIGNAL(triggered()), this, SLOT(actSetType()));
    connect(ui->actionSet_IP_address, SIGNAL(triggered()), this, SLOT(actSetIp()));
    connect(ui->actionSet_destination_MAC, SIGNAL(triggered()), this, SLOT(actSetDstMac()));
    connect(ui->actionSet_source_MAC, SIGNAL(triggered()), this, SLOT(actSetSrcMac()));
    connect(ui->actionSet_source_IP, SIGNAL(triggered()), this, SLOT(actSetSrcIp()));
    connect(ui->actionSet_destination_IP, SIGNAL(triggered()), this, SLOT(actSetDstIp()));
    connect(ui->actionSet_source_port, SIGNAL(triggered()), this, SLOT(actSetSrcPort()));
    connect(ui->actionSet_destination_port, SIGNAL(triggered()), this, SLOT(actSetDstPort()));
    connect(ui->actionSet_vlan_ID, SIGNAL(triggered()), this, SLOT(actSetWlanId()));
    connect(ui->actionSet_Frequency_mode, SIGNAL(triggered()), this, SLOT(setFrequencyMode()));
    connect(ui->actionFrequency_mode, SIGNAL(triggered()), this, SLOT(frequencyMode()));
    connect(ui->actionAbout_DPDK,SIGNAL(triggered()),this, SLOT(proposdpdk()));
    connect(ui->actionAbout_Pktgen, SIGNAL(triggered()), this, SLOT(propospktgen()));
    connect(ui->actionAbout_QT, SIGNAL(triggered()), this, SLOT(proposqt()));
    connect(ui->actionAbout_Pktgen_Client, SIGNAL(triggered()), this, SLOT(aboutPktgenClient()));
    init_Connexion();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePort(int nmb){
    this->nmbPortint = nmb;
    initSubWindows(nmb);
    emit portToSender(nmb);
}

void MainWindow::connectedProcess(){
    connexionState->setText("Connected");
    dataDispatcher = new DataDispatcher();
    senderThread = new SenderThread();
    connect(ui->actionQuit, SIGNAL(triggered()), dataDispatcher, SLOT(deleteLater()));
    connect(ui->actionQuit, SIGNAL(triggered()), senderThread, SLOT(deleteLater()));
    connect(dataDispatcher, SIGNAL(tinfoNmbPort(int)), this, SLOT(updatePort(int)));
    connect(this, SIGNAL(portToSender(int)), senderThread, SLOT(updatePort(int)), Qt::DirectConnection);
    connect(this, SIGNAL(sendOrder(QByteArray)), senderThread, SLOT(OneRequest(QByteArray)));
    dataDispatcher->start();
    senderThread->start(QThread::LowestPriority);
}

void MainWindow::init_Connexion(){
    Connexion *conWin = new Connexion(this);
    conWin->show();
}

void MainWindow::connexionDown(){
    ui->actionConnexion->setEnabled(true);
    enDisMenu(false);
    QMessageBox msgBox;
    msgBox.critical(this, "Error !", "Connection Failed !\nPlease check the IP address.");
}

void MainWindow::transactionDown(){
    QMessageBox msgBox;
    connexionState->setText("Disconnected");
    senderThread->deleteLater();
    dataDispatcher->deleteLater();
    clientThread->deleteLater();
    ui->actionConnexion->setEnabled(true);
    enDisMenu(false);
    msgBox.critical(this, "Error !", "Transaction Failed !\nPlease check if server is still working.");
    zoneCentrale->closeAllSubWindows();
    qDebug()<< zoneCentrale->subWindowList();
}

portWindows *MainWindow::createportWindows(int port)
{
    portWindows *child = new portWindows(port, dataDispatcher);
    zoneCentrale->addSubWindow(child, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    return child;
}

void MainWindow::connexion(QString address){
    clientThread = new ClientThread(address);
    connect(clientThread, SIGNAL(connexionOk()), this, SLOT(connectedProcess()));
    connect(clientThread, SIGNAL(connexionNok()), this, SLOT(connexionDown()));
    connect(clientThread, SIGNAL(transactionNok()), this, SLOT(transactionDown()));
    connect(ui->actionQuit, SIGNAL(triggered()), clientThread, SLOT(deleteLater()));
    clientThread->start(QThread::HighestPriority);
    ui->actionConnexion->setEnabled(false);
    enDisMenu(true);
}

void MainWindow::actSet(){
    FenSet *setWin = new FenSet(senderThread);
    setWin->show();
}

void MainWindow::actSetMac(){
    FenSetMac *setMacWin = new FenSetMac(senderThread, 2);
    setMacWin->show();
}

void MainWindow::actSetProto(){
    FenSetMac *setMacWin = new FenSetMac(senderThread, 1);
    setMacWin->show();
}

void MainWindow::actSetType(){
    FenSetMac *setMacWin = new FenSetMac(senderThread, 0);
    setMacWin->show();
}

void MainWindow::actSetIp(){
    FenSetIp *setIpWin = new FenSetIp(senderThread);
    setIpWin->show();
}

void MainWindow::actSetDstMac(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 2);
    setRangeWin->show();
}

void MainWindow::actSetSrcMac(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 6);
    setRangeWin->show();
}

void MainWindow::actSetSrcIp(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 4);
    setRangeWin->show();
}

void MainWindow::actSetDstIp(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 5);
    setRangeWin->show();
}

void MainWindow::actSetSrcPort(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 1);
    setRangeWin->show();
}

void MainWindow::actSetDstPort(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 3);
    setRangeWin->show();
}

void MainWindow::actSetWlanId(){
    Fensetsrcmac *setRangeWin = new Fensetsrcmac(senderThread, 0);
    setRangeWin->show();
}

void MainWindow::actStp(){
    QByteArray stpRequest = "pktgen.stop(\"all\");\r\n";
    emit sendOrder(stpRequest);
    //senderThread->stopRefresh();
}

void MainWindow::actStr(){
    QByteArray strRequest = "pktgen.start(\"all\");\r\n";
    emit sendOrder(strRequest);
    //senderThread->startRefresh();
}

void MainWindow::actClr(){
    QByteArray clrRequest = "pktgen.clear(\"all\");\r\n";
    emit sendOrder(clrRequest);
    emit clearStats("all");
}

void MainWindow::actRst(){
    QByteArray clrRequest = "pktgen.reset(\"all\");\r\n";
    emit sendOrder(clrRequest);
}

void MainWindow::actStart(){
    QByteArray startRequest1 = "pktgen.start(\"";
    QByteArray startRequest2 = "\");\r\n";
    QByteArray startRequest;
    QString temp = QInputDialog::getText(this, "Start...", "Port ?");
    if (temp.size()>0){
    startRequest = (startRequest1 + temp.toUtf8() + startRequest2);
    emit sendOrder(startRequest);
    }
}

void MainWindow::actStop(){
    QByteArray stopRequest1 = "pktgen.stop(\"";
    QByteArray stopRequest2 = "\");\r\n";
    QByteArray stopRequest;
    QString temp = QInputDialog::getText(this, "Stop...", "Port ?");
    if (temp.size()>0){
    stopRequest = (stopRequest1 + temp.toUtf8() + stopRequest2);
    emit sendOrder(stopRequest);
    }
}

void MainWindow::actClear(){
    QByteArray stopRequest1 = "pktgen.clear(\"";
    QByteArray stopRequest2 = "\");\r\n";
    QByteArray stopRequest;
    QString temp = QInputDialog::getText(this, "Clear...", "Port ?");
    if (temp.size()>0){
    stopRequest = (stopRequest1 + temp.toUtf8() + stopRequest2);
    emit sendOrder(stopRequest);
    emit clearStats(temp);
    }
}

void MainWindow::actReset(){
    QByteArray stopRequest1 = "pktgen.reset(\"";
    QByteArray stopRequest2 = "\");\r\n";
    QByteArray stopRequest;
    QString temp = QInputDialog::getText(this, "Reset...", "Port ?");
    if (temp.size()>0){
    stopRequest = (stopRequest1 + temp.toUtf8() + stopRequest2);
    emit sendOrder(stopRequest);
    }
}

void MainWindow::setFrequencyMode(){
    FreqWindow *freqMacro = new FreqWindow(senderThread, this);
    freqMacro->show();
}

void MainWindow::frequencyMode(){
    static bool flag;
    if (!flag){
        senderThread->startFreqMode(true);
        ui->actionFrequency_mode->setChecked(true);
        flag = true;
    } else {
        senderThread->startFreqMode(false);
        ui->actionFrequency_mode->setChecked(false);
        flag = false;
    }

}

void MainWindow::initSubWindows(int port) {
    int k;
    this->nmbPortint = port;
    for (k=0 ; k<port; k++){
        portWindows *child = createportWindows(k);
        child->newPort();
        child->show();
    }
}

class ActiveMdiSubWindowFunctor {
public:
    explicit ActiveMdiSubWindowFunctor(QMdiArea *mdiArea, QMdiSubWindow *activeWindow) : m_mdiArea(mdiArea), m_activeWindow(activeWindow) {}
    void operator()() const { m_mdiArea->setActiveSubWindow(m_activeWindow); }

private:
    QMdiArea *m_mdiArea;
    QMdiSubWindow *m_activeWindow;
};

void MainWindow::updateDisplayMenu(){
    ui->menuDisplay->clear();
    QList<QMdiSubWindow *> windows = zoneCentrale->subWindowList();
    for (int i = 0; i < windows.size(); ++i) {
        QMdiSubWindow *mdiSubWindow = windows.at(i);
        portWindows *child = qobject_cast<portWindows *>(mdiSubWindow->widget());

        QString text;
        if (i < 9) {
            text = tr("Port &%1").arg(i + 1);
        } else {
            text = tr("Port %1").arg(i + 1);
        }
        QAction *action = ui->menuDisplay->addAction(text, mdiSubWindow, ActiveMdiSubWindowFunctor(zoneCentrale, mdiSubWindow));
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
    }

}

portWindows *MainWindow::activeMdiChild() const
{
    if (QMdiSubWindow *activeSubWindow = zoneCentrale->activeSubWindow())
        return qobject_cast<portWindows *>(activeSubWindow->widget());
    return 0;
}

void MainWindow::enDisMenu(const bool stat){
    ui->actionStart->setEnabled(stat);
    ui->actionStop->setEnabled(stat);
    ui->actionReset->setEnabled(stat);
    ui->actionClear->setEnabled(stat);
    ui->actionStart_all->setEnabled(stat);
    ui->actionStop_all->setEnabled(stat);
    ui->actionReset_all->setEnabled(stat);
    ui->actionClear_all->setEnabled(stat);
    ui->actionSet->setEnabled(stat);
    ui->actionSet_IP_address->setEnabled(stat);
    ui->actionSet_MAC_address->setEnabled(stat);
    ui->actionSet_protocol->setEnabled(stat);
    ui->actionSet_type->setEnabled(stat);
    //ui->actionSet_Frequency_mode->setEnabled(stat);
    ui->actionSet_destination_IP->setEnabled(stat);
    ui->actionSet_destination_MAC->setEnabled(stat);
    ui->actionSet_destination_port->setEnabled(stat);
    ui->actionSet_source_IP->setEnabled(stat);
    ui->actionSet_source_MAC->setEnabled(stat);
    ui->actionSet_source_port->setEnabled(stat);
    ui->actionSet_vlan_ID->setEnabled(stat);
    //ui->actionFrequency_mode->setEnabled(stat);
    ui->actionAbout_DPDK->setEnabled(stat);
    ui->actionAbout_Pktgen->setEnabled(stat);
}

void MainWindow::aboutPktgenClient(){
    QMessageBox::information(0, "About Pktgen Client", "App created for educational purpose\nThomas Sailley\n2016");
}

void MainWindow::proposqt()
{
    QMessageBox msgBox;
    msgBox.aboutQt(this);
}

void MainWindow::propospktgen()
{
    QByteArray reception1 = dataDispatcher->getInfo(0);
    QByteArray reception2 = dataDispatcher->getInfo(1);
    QByteArray reception3 = dataDispatcher->getInfo(2);
    QMessageBox::information(0, "About Pktgen", reception2 + "\n" + reception1 + "\n" + reception3);
}

void MainWindow::proposdpdk()
{
    QByteArray reception1 = dataDispatcher->getInfo(3);
    QByteArray reception2 = dataDispatcher->getInfo(4);
    QMessageBox::information(0, "About DPDK", reception1 + "\n" + reception2);
}
