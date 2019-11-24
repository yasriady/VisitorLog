#ifndef DATABASE_H
#define DATABASE_H

#include "rfidata.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <RestApi/myhttprequestinput.h>
#include <QObject>
#include <db/member.h>
#include "const.h"
#include <QJsonDocument>
#include <QJsonObject>

// const QString DRIVER("QSQLITE");

class Database: public QObject
{
    Q_OBJECT
public:
    Database();
    ~Database();
    void process(RFIData data);
    void insertNew(RFIData data);
    void getPersonData(QString uid);

private slots:
    void handle_result(HttpRequestWorker *httpRequestWorker);

private:
    void initDb();
    QSqlDatabase mDb1, mDb2;
    void uploadToServer(RFIData data);
    void postData(RFIData data);
    QString getNextTicketSequence();
    void updateRecord(QString ticketNo);
    Member getPersonDataFromLocalDb(QString uid);
};

#endif // DATABASE_H
