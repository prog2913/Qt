#ifndef NETWORKFILE_H
#define NETWORKFILE_H
#define SERVERCONNCLOSE 99

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "mainwindow.h"

class networkFile : public QObject
{
    Q_OBJECT
public:
    explicit networkFile(QObject *parent = 0);
    QTcpSocket * doClientConnect(QString client_ip, quint16 client_port);
    int doServerListen();
    int sendClientMsg(QByteArray sendMsgCopy_ins);
    int sendServerMsg(QByteArray sendMsgCopy_ins);
    int stopClientConn();
    int stopServerConn();

signals:
    void dataRead(QString);

public slots:
    void newServerConn();
    void readFromClient();
    void readFromServer();

private:
    QTcpServer * serv = NULL;
    QTcpSocket * sock = NULL;
};

#endif // NETWORKFILE_H
