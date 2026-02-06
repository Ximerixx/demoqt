#ifndef PASSWORDHASH_H
#define PASSWORDHASH_H

#include <QByteArray>
#include <QString>

class PasswordHash
{
public:
    static QByteArray generateSalt();
    static QByteArray hashPassword(const QString &password, const QByteArray &salt);
    static bool verifyPassword(const QString &password, const QByteArray &salt, const QByteArray &storedHash);
};

#endif // PASSWORDHASH_H
