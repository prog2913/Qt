#include "networkfile.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

QString valClient;

networkFile::networkFile(QObject *parent) : QObject(parent)
{
}

QTcpSocket * networkFile::doClientConnect(QString client_ip, quint16 client_port){
    sock = new QTcpSocket(this);
    sock->connectToHost(client_ip, client_port, QIODevice::ReadWrite);
    connect(sock, SIGNAL(readyRead()), this, SLOT(readFromServer()));
    if (sock->waitForConnected()){
        sock->write("Connected.");       //new
        sock->waitForBytesWritten();
        return sock;
    } else {
        sock = NULL;
        return NULL;
    }
}

int networkFile::sendClientMsg(QByteArray sendMsgCopy_ins){
    if (sock != NULL){
        sock->write(sendMsgCopy_ins);
        sock->waitForBytesWritten();
        return 1;
    }
    else
        return 0;
}

void networkFile::readFromServer(){
    valClient = sock->readAll();
    valClient.prepend("Server : ");
    emit dataRead(valClient);
}

int networkFile::stopClientConn(){
    if (sock == NULL){
        return 0;
    } else {
        sock->deleteLater();
        sock = NULL;
        return 1;
    }
}

/********************************
 * SERVER SECTION
 ********************************
 */
int networkFile::doServerListen(){
    serv = new QTcpServer(this);
    connect(serv, SIGNAL(newConnection()), this, SLOT(newServerConn()));
    serv->listen(QHostAddress::LocalHost, 6789);
    serv->waitForNewConnection();
    /* test */
    if (serv == NULL)
        qDebug() <<"null";
    return 2;
}

void networkFile::newServerConn(){
    sock = serv->nextPendingConnection();
    connect(sock, SIGNAL(readyRead()), this, SLOT(readFromClient()));
    sock->write("Connected.");
    sock->waitForBytesWritten();
}

int networkFile::stopServerConn(){
    if (serv == NULL)
        return 0;
    else {
        serv->deleteLater();
        serv = NULL;
        return 1;
    }
}

int networkFile::sendServerMsg(QByteArray sendMsgCopy_ins){
    if (serv != NULL){
        sock->write(sendMsgCopy_ins);
        sock->waitForBytesWritten();
        return 1;
    }
    else
        return 0;
}

void networkFile::readFromClient(){
    valClient = sock->readAll();
    valClient.prepend("Client : ");
    emit dataRead(valClient);
}
