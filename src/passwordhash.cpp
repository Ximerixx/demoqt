#include "passwordhash.h"
#include <QCryptographicHash>
#include <QRandomGenerator>

QByteArray PasswordHash::generateSalt()
{
    constexpr int saltSize = 32;
    QByteArray salt(saltSize, 0);
    for (int i = 0; i < saltSize; ++i)
        salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    return salt.toHex();
}

QByteArray PasswordHash::hashPassword(const QString &password, const QByteArray &salt)
{
    QByteArray data = salt + password.toUtf8();
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
}

bool PasswordHash::verifyPassword(const QString &password, const QByteArray &salt, const QByteArray &storedHash)
{
    QByteArray computed = hashPassword(password, salt);
    return computed == storedHash;
}
