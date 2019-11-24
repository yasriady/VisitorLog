#include "database.h"

Database::Database()
{
    initDb();
}

Database::~Database()
{
    if (mDb1.isOpen()) {
        mDb1.close();
    }
    if (mDb2.isOpen()) {
        mDb2.close();
    }
}

void Database::process(RFIData data)
{
    insertNew(data);
}

void Database::insertNew(RFIData data)
{
    QString seq =  getNextTicketSequence();
    QString ticketNo  = data.getTicketNo() + seq;

    QSqlQuery query(mDb1);
    query.prepare("INSERT INTO `t_tickets`(`ticket_no`, `uid`, `date`, `time`, `new`) VALUES(:ticket_no, :uid, :date, :time, :new)");
    query.bindValue(":ticket_no", ticketNo);
    query.bindValue(":uid", data.uid);
    query.bindValue(":date", data.getDate());
    query.bindValue(":time", data.getTime());
    query.bindValue(":new", 1);

    query.exec();

    data.setTicketNo(ticketNo);
    postData(data);

    // if (!mDb1.open()) {
    //    qWarning() << "ERROR1: " << mDb1.lastError();
    // } else {
    // QSqlQuery query("INSERT INTO `t_tickets`(`uid`, `date`, `time`) VALUES('Dedy Yasriady', '019-11-22', '07:30:21')");
    // if (!query.isActive())
    //    qWarning() << "ERROR2: " << query.lastError().text();
    //}
    // mDb1.close();

    //QString sql = "INSERT INTO `log`(`uid`, `date`, `time`) VALUES('Dedy Yasriady', '019-11-22', '07:30:21')";
    //QSqlQuery query = mDb1.exec(sql);
    //if (!query.isActive())
    //    qWarning() << "ERROR0: " << query.lastError().text();

}

void Database::getPersonData(QString uid)
{

}

void Database::initDb()
{
    if (QSqlDatabase::isDriverAvailable(DRIVER)) {

        {
            mDb1 = QSqlDatabase::addDatabase(DRIVER, "mDb1");
            mDb1.setDatabaseName("data.sqlite");
            if (!mDb1.open()) {
                qWarning() << "ERROR1: " << mDb1.lastError();
            } else {
                QString sql = "CREATE TABLE IF NOT EXISTS `t_tickets` (id INTEGER PRIMARY KEY, `ticket_no` TEXT, `uid` TEXT, `date` DATE, `time` TIME, `new` INTEGER)";
                QSqlQuery query(mDb1);
                query.exec(sql);
                if (!query.isActive())
                    qWarning() << "ERROR2A: " << query.lastError().text();
            }
        }
        // QSqlDatabase::removeDatabase("mDb1"); // correct

        {
            mDb2 = QSqlDatabase::addDatabase(DRIVER, "mDb2");
            mDb2.setDatabaseName("ref.sqlite");
            if (!mDb2.open()) {
                qWarning() << "ERROR1: " << mDb2.lastError();
            } else {
                QString sql = "CREATE TABLE IF NOT EXISTS `t_members` (id INTEGER PRIMARY KEY, `uid` TEXT, `nim` TEXT, `nama` TEXT )";
                QSqlQuery query(mDb2);
                query.exec(sql);
                if (!query.isActive())
                    qWarning() << "ERROR2B: " << query.lastError().text();
            }
        }
        // QSqlDatabase::removeDatabase("mDb2");
    }

}

void Database::uploadToServer(RFIData data)
{
    QString sql = " SELECT * FROM `t_tickets` WHERE `new`=1; ";
    QSqlQuery query(mDb1);
    query.exec();
    query.first();
    while (query.next()) {
    }
}

void Database::postData(RFIData data)
{
    QString url_str = QString(DATAURL) + "visitor";

    MyHttpRequestInput input(url_str, "POST");

    // input.add_var("X-API-KEY", "12345"); // relocated into MyHttpRequestInput c'tor

    input.add_var("ticket_no", data.getTicketNo());
    input.add_var("uid", data.uid);
    input.add_var("date", data.getDate());
    input.add_var("time", data.getTime());
    input.add_var("new",  "0");

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker *)), this, SLOT(handle_result(HttpRequestWorker *)));
    worker->execute(&input);

}

QString Database::getNextTicketSequence()
{
    QString result;
    QSqlQuery query(mDb1);
    const QString &date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); // 2018-12-27 00:00:00
    QString sql = QString(" SELECT MAX(`rowid`) AS `rowid`, * FROM `t_tickets` WHERE `date` >= '%1'; ").arg(date);
    query.prepare(sql);
    int seq = 0;
    if (query.exec()) {
        query.first();
        QString ticketNo = query.value("ticket_no").toString();
        if (!ticketNo.trimmed().isEmpty()) {
            ticketNo = ticketNo.right(3);
            seq = ticketNo.toInt();
        }
    }
    seq++;
    result = QString("%1").arg(seq, 3, 10, QChar('0'));
    return result;
}

void Database::updateRecord(QString ticketNo)
{
    mDb1.open();
    QSqlQuery query(mDb1);
    query.prepare("UPDATE `t_tickets` SET `new`=0 WHERE `ticket_no`=:ticket_no ");
    query.bindValue(":ticket_no", ticketNo);
    query.exec();
}

Member Database::getPersonDataFromLocalDb(QString uid)
{
    Member member;
    QString sql =  " SELECT * FROM `t_members` WHERE `uid`=:uid " ;
    QSqlQuery query(mDb2);
    query.prepare(sql);
    query.bindValue(":uid", uid);
    query.exec();
    query.first();
    while (query.next()) {
        QString uid = query.value("uid").toString();
        QString nim = query.value("nim").toString();
        QString nama = query.value("nama").toString();
        // member.uid = uid;
        // member.nim = nim;
        // member.nama = nama;
    }
    return member;
}

void Database::handle_result(HttpRequestWorker *httpRequestWorker)
{
    QString str = QString(httpRequestWorker->response);
    QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject resp = d.object();

    if (!resp.isEmpty()) {
        QJsonValue v = resp.value(QString("ticket_no"));
        QString ticketNo = v.toString();
        // qDebug() << ticketNo;
        updateRecord(ticketNo); // set flag new=0
    } else {
        // qDebug() << "EMPTY!";

    }
}

