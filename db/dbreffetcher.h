#ifndef DBREFFETCHER_H
#define DBREFFETCHER_H

#include "member.h"
#include <QObject>
#include <QWidget>
#include "const.h"
#include <RestApi/myhttprequestinput.h>
#include <QJsonDocument>
#include <QJsonObject>
#include "db/dbref.h"

class DbRefFetcher : public QObject
{
    Q_OBJECT
public:
    DbRefFetcher(QObject *parent = nullptr);
    ~DbRefFetcher();
    void process(const QString uid);

signals:
    // void refData(Member member);
    void memberDataReady(Member member);

private slots:
    void handle_result(HttpRequestWorker *httpRequestWorker);

private:
    void getFromLocal(const QString uid);
    void getFromServer(const QString uid);
    DbRef *mDbRef;
    void memberDataAvailable(const Member member);

};

#endif // DBREFFETCHER_H
