#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "rfidata.h"

#include <QObject>
#include <QThread>
#include <QtDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTextCodec>
#include "db/database.h"

/**
 *  7B94840D
 *  AA21FDEE
 *  89AB77BC
 */

class QTcpSocket;

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr descriptor, QObject *parent = nullptr);
    ~SocketThread();

signals:
    void newData(RFIData data);

protected:
    void run() Q_DECL_OVERRIDE;

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *m_socket;
    qintptr m_descriptor;
    // void emitNewData(ResultTypes type, QString uid = "", QString string = "");
    Database *mDb;

};

#endif // SOCKETTHREAD_H
