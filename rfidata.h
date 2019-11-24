#ifndef RFIDATA_H
#define RFIDATA_H

#include <QString>
#include <QAbstractSocket>
#include <QDate>

enum class ResultTypes {
    IncomingConnection,
    Disconnected,
    NewData,
    Error
};

class RFIData
{
public:
    RFIData();
    ~RFIData();

public:
    ResultTypes type;
    QString uid;
    QString string;
    QString getDate();
    QString getTime();
    QString getTicketNo();
    void setTicketNo(QString ticketNo);

private:
    QDateTime mCurrentDt;
    QString mTicketNo;

};

#endif // RFIDATA_H
