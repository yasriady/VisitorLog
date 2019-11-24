#ifndef DBREF_H
#define DBREF_H

#include "member.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "const.h"

// const QString DRIVER("QSQLITE");

class DbRef : public QObject
{
    Q_OBJECT
public:
    explicit DbRef(QObject *parent = nullptr);
    ~DbRef();

    Member getMember(const QString uid);

signals:

public slots:


private:
    void initDb();
    QSqlDatabase mDb2;

};


#endif // DBREF_H
