#include "server.h"

Server::Server(QObject *parent):
    QTcpServer(parent)
{
    if (this->listen(QHostAddress::Any, 2525)) {
        qDebug() << "Server start at port: " << this->serverPort();
    } else {
        qDebug() << "Start failure";
    }
}

void Server::incomingConnection(qintptr handle)
{
    // qDebug() << Q_FUNC_INFO << " new connection";
    SocketThread *socket = new SocketThread(handle);
    connect(socket, SIGNAL(finished()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(newData(RFIData)), this, SLOT(onNewData(RFIData)));
    socket->start();

    // emitNewData(ResultTypes::IncomingConnection, "IncomingConnection");
    RFIData data;
    data.type = ResultTypes::IncomingConnection;
    data.string = "IncomingConnection";
}

void Server::onNewData(RFIData data)
{
    emit newData(data);
}

//void Server::emitNewData(ResultTypes type, QString string)
//{
//    RFIData data;
//    data.type = type;
//    data.string = string;
//    emit newData(data);
//}

