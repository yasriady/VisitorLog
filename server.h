#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "socketthread.h"
#include <QObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

signals:
    void newData(RFIData data);

public slots:
    void onNewData(RFIData data);

private:
    // void emitNewData(ResultTypes type, QString string = "");

};

#endif // SERVER_H
