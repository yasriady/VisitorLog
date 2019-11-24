#include "rfidata.h"

RFIData::RFIData()
{
    qRegisterMetaType<RFIData>("RFIData");
    type = ResultTypes::NewData;
    mCurrentDt = QDateTime::currentDateTime();
    mTicketNo = "VST" + mCurrentDt.toString("yyyyMMddhhmmss") ; //"VST2019112310240301";
}

RFIData::~RFIData()
{
}

QString RFIData::getDate()
{
    // QDateTime dt = QDateTime::currentDateTime();
    QString dateStr = mCurrentDt.toString("yyyy-MM-dd");
    return dateStr;
}

QString RFIData::getTime()
{
    // QDateTime dt = QDateTime::currentDateTime();
    QString timeStr = mCurrentDt.toString("hh:mm:ss");
    return timeStr;
}

QString RFIData::getTicketNo()
{
    return mTicketNo;
}

void RFIData::setTicketNo(QString ticketNo)
{
    mTicketNo = ticketNo;
}
