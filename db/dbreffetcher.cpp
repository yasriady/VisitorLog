#include "dbreffetcher.h"

DbRefFetcher::DbRefFetcher(QObject *parent)
    : QObject(parent)
{
    mDbRef = new DbRef(this);
}

DbRefFetcher::~DbRefFetcher()
{
    if (mDbRef != nullptr) {
        mDbRef = nullptr;
    }
}

void DbRefFetcher::process(const QString uid)
{
    Member member = mDbRef->getMember(uid);
    if (member.getNama().trimmed().isEmpty()) {
        // qDebug() << "No member in db";
        // getFromServer(uid);
    } else {
        // qDebug() << "Member available";
        memberDataAvailable(member);
    }
}

void DbRefFetcher::handle_result(HttpRequestWorker *httpRequestWorker)
{
    QString str = QString(httpRequestWorker->response);
    QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject resp = d.object();

    qDebug() << "----------- str --------------> " << str;

    if (!resp.isEmpty()) {
        QJsonValue v = resp.value(QString("ticket_no"));
        QString ticketNo = v.toString();
        // qDebug() << ticketNo;
        // updateRecord(ticketNo); // set flag new=0
    } else {
        // qDebug() << "EMPTY!";

    }
}

void DbRefFetcher::getFromLocal(const QString uid)
{

}

void DbRefFetcher::getFromServer(const QString uid)
{
    QString url_str = QString(BASEURL) + "index.php/api/member/member";
    MyHttpRequestInput input(url_str, "GET");

    input.add_var("uid", uid);

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker *)), this, SLOT(handle_result(HttpRequestWorker *)));
    worker->execute(&input);

}

void DbRefFetcher::memberDataAvailable(const Member member)
{
    emit memberDataReady(member);
}

