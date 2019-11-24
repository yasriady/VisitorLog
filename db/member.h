#ifndef MEMBER_H
#define MEMBER_H
#include <QString>

class Member
{
public:
    Member();

    QString getUid() const;
    void setUid(const QString &value);
    QString getNim() const;
    void setNim(const QString &value);
    QString getNama() const;
    void setNama(const QString &value);

private:
    QString uid = "";
    QString nim = "";
    QString nama = "";
};

#endif // MEMBER_H
