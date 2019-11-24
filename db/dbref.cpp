#include "dbref.h"

DbRef::DbRef(QObject *parent) : QObject(parent)
{
    initDb();
}

DbRef::~DbRef()
{
    if (mDb2.isOpen()) {
        mDb2.close();
    }
}

Member DbRef::getMember(const QString uid)
{
    initDb();   //?? REDUNDANT ??

    Member member;
    if (! mDb2.open()) {
        qDebug() << mDb2.lastError().text();
    }
    QSqlQuery query(mDb2);
    // query.prepare(" SELECT * FROM `t_members` WHERE `uid` = ? ");
    // query.addBindValue(uid);
    query.prepare(" SELECT * FROM `t_members` WHERE `uid`=:uid  ");
    query.bindValue(":uid", uid);

    // uid, nim, nama
    if (query.exec()) {
        if (query.first()) {
            QString uid = query.value("uid").toString();
            QString nim = query.value("nim").toString();
            QString nama = query.value("nama").toString();
            member.setUid(uid);
            member.setNim(nim);
            member.setNama(nama);
        }
    }
    mDb2.close();
    return member;
}

void DbRef::initDb()
{
    if (QSqlDatabase::isDriverAvailable(DRIVER)) {

        {
            mDb2 = QSqlDatabase::addDatabase(DRIVER, "mDb2");
            mDb2.setDatabaseName("ref.sqlite");
            if (!mDb2.open()) {
                qWarning() << "ERROR1: " << mDb2.lastError();
            } else {
                QString sql = " CREATE TABLE IF NOT EXISTS `t_members` (id INTEGER PRIMARY KEY, `uid` TEXT, `nim` TEXT, `nama` TEXT ); ";
                QSqlQuery query(mDb2);
                query.exec(sql);
                sql = " CREATE UNIQUE INDEX `idx_members_uid` ON `t_members` (`uid`); ";
                query.exec(sql);
                if (!query.isActive())
                    qWarning() << "ERROR2B: " << query.lastError().text();
            }
        }
    }

}
