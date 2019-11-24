#include "member.h"

Member::Member()
{
    // qRegisterMetaType<Member>("Member");
}

QString Member::getNama() const
{
    return nama;
}

void Member::setNama(const QString &value)
{
    nama = value;
}

QString Member::getNim() const
{
    return nim;
}

void Member::setNim(const QString &value)
{
    nim = value;
}

QString Member::getUid() const
{
    return uid;
}

void Member::setUid(const QString &value)
{
    uid = value;
}
