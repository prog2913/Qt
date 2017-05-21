#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTcpSocket>
#include <QPlainTextEdit>
#include <QDebug>
#include <QScrollBar>

static QString chatMsg = "";
QString sendMsgCopy;
QString chatDelimit = "\r\n\r\n";
int clientMsgRet, serverMsgRet;
int servStop1, clientStop1;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setDisabled(true);
    ui->plainTextEdit->setCenterOnScroll(true);
    ui->lineEdit_3->setText("6789");

    //ui->plainTextEdit->centerCursor();
    //ui->plainTextEdit->ensureCursorVisible();
    //ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());

    // network section
    net = new networkFile(this);

    connect(net, &networkFile::dataRead, this, &MainWindow::addText);

    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(on_returnSend_text()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_returnSend_text()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_clearScreen_click()));

    // check and disable network conf if server enabled
    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(disNetConf()));
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(disNetConf()));

    // connect and terminate connection
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(nConnect()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(nStopConnect()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    w2 = new AboutWindow(this);
    w2->setWindowTitle("About QChat");
    w2->show();
}

void MainWindow::on_returnSend_text()
{
    if (!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked()){
        QMessageBox::information(this, "Invalid Options", "No valid Option selected");
    }
    if (ui->lineEdit_2->text() != ""){

        chatMsg += ui->lineEdit_2->text();
        chatMsg.append(chatDelimit);

        // send message to client
        QByteArray sendMsgCopy = ui->lineEdit_2->text().QString::toUtf8();
        if (ui->radioButton->isChecked()){
            clientMsgRet = net->sendClientMsg(sendMsgCopy);
            if (clientMsgRet == 0){
                QMessageBox::information(this, "Connection error", "No connection found!");
                ui->lineEdit_2->setText("");
                chatMsg = "";
                return;
            }
            ui->plainTextEdit->setPlainText(chatMsg);
            ui->lineEdit_2->setText("");
        } else if (ui->radioButton_2->isChecked()){
            serverMsgRet = net->sendServerMsg(sendMsgCopy);
            if (serverMsgRet == 0){
                QMessageBox::information(this, "Connection error", "No connection found!");
                ui->lineEdit_2->setText("");
                chatMsg = "";
                return;
            }
            ui->plainTextEdit->setPlainText(chatMsg);
            //QTextCursor c =  ui->plainTextEdit->textCursor();    //now
            //c.movePosition(QTextCursor::);
            //ui->plainTextEdit->setTextCursor(c);
            ui->lineEdit_2->setText("");
        }
    } else {
        ui->statusBar->showMessage("Nothing to send", 1000);
    }
}

void MainWindow::on_clearScreen_click()
{
    ui->plainTextEdit->setPlainText("");
}

void MainWindow::nConnect(){

    // check whether client or server selected
    if (!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked()){
        QMessageBox::information(this, "Invalid Actor", "Select a  communication type.");
        return;
    }

    // execute client or server as per the radio values
    if (ui->radioButton->isChecked()){
        if (ui->lineEdit->text() == "" || ui->lineEdit_3->text() == "" || ui->lineEdit_3->text().QString::toInt() == 0){
            QMessageBox::information(this, "Network Configuration", "valid IP and Port required.");
            return;
        }
        // code for client
        QString ipaddr = ui->lineEdit->text();
        quint16 portaddr_c = ui->lineEdit_3->text().QString::toUShort();

        QTcpSocket * conSock = net->doClientConnect(ipaddr, portaddr_c);

        if (conSock == NULL)
            QMessageBox::critical(this, "Connection error", "Unable to connect");
        else
            ui->statusBar->showMessage("Connected", 2000);

    } else if (ui->radioButton_2->isChecked()){
        on_clearScreen_click();
        ui->plainTextEdit->setPlainText("waiting for client to connect...");
        net->doServerListen();
    }

}

void MainWindow::nStopConnect(){
    QMessageBox::StandardButton repVal = QMessageBox::question(this, "Stop Connection", "Are you sure you want to terminate the connection?", QMessageBox::Yes | QMessageBox::No);
    if (repVal == QMessageBox::Yes && ui->radioButton_2->isChecked()){
        servStop1 = net->stopServerConn();
        if (servStop1 == 0){
            QMessageBox::information(this, "Connection Termination", "No connection found");
            return;
        }
        ui->statusBar->showMessage("Connection terminated", 2000);
        chatMsg = "";
        ui->plainTextEdit->setPlainText("");
    } else if (repVal == QMessageBox::Yes && ui->radioButton->isChecked()){
        clientStop1 = net->stopClientConn();
        if (clientStop1 == 0){
            QMessageBox::information(this, "Connection Termination", "No connection found");
            return;
        }
        ui->statusBar->showMessage("Connection terminated", 2000);
        chatMsg = "";
        ui->plainTextEdit->setPlainText("");
    }
}

void MainWindow::disNetConf(){
    // disable IP and port if server is checked
    if (ui->radioButton_2->isChecked()){
        ui->lineEdit->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
    } else {
        ui->lineEdit->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);
    }
}

////////////////////////////////
/* server write to plain edit */
////////////////////////////////
void MainWindow::addText(QString value1){
    chatMsg += value1;
    chatMsg.append(chatDelimit);
    ui->plainTextEdit->setPlainText(chatMsg);
}

void MainWindow::on_actionNew_triggered()
{
    if (ui->radioButton->isChecked())
        net->stopClientConn();
    else if (ui->radioButton_2->isChecked())
        net->stopServerConn();
    else {
        QMessageBox::information(this, "Invalid Option", "Check valid options");
    }
    ui->plainTextEdit->setPlainText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit->setText("");
}
