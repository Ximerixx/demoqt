#include "database.h"
#include "passwordhash.h"
#include <QSqlQuery>
#include <QSqlError>

Database &Database::instance()
{
    static Database inst;
    return inst;
}

bool Database::open(const QString &path)
{
    if (path.isEmpty()) {
        m_lastError = QObject::tr("Путь к базе данных не указан.");
        return false;
    }

    if (m_db.isOpen()) {
        QString connName = m_db.connectionName();
        m_db.close();
        m_db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connName);
    }

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }

    m_dbPath = path;

    if (!ensureSchema()) {
        m_db.close();
        return false;
    }

    return true;
}

void Database::close()
{
    if (!m_db.isOpen())
        return;
    QString connName = m_db.connectionName();
    m_db.close();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);
    m_dbPath.clear();
}

bool Database::ensureSchema()
{
    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "salt TEXT NOT NULL)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS documents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT,"
        "created_at TEXT)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS notes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "content TEXT,"
        "created_at TEXT)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    return true;
}

bool Database::createUser(const QString &login, const QString &password)
{
    QByteArray salt = PasswordHash::generateSalt();
    QByteArray hash = PasswordHash::hashPassword(password, salt);

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO users (login, password_hash, salt) VALUES (:login, :hash, :salt)"));
    q.bindValue(QStringLiteral(":login"), login);
    q.bindValue(QStringLiteral(":hash"), QString::fromUtf8(hash));
    q.bindValue(QStringLiteral(":salt"), QString::fromUtf8(salt));

    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return true;
}

bool Database::verifyLogin(const QString &login, const QString &password)
{
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("SELECT password_hash, salt FROM users WHERE login = :login"));
    q.bindValue(QStringLiteral(":login"), login);

    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    if (!q.next())
        return false;

    QByteArray storedHash = q.value(0).toString().toUtf8();
    QByteArray salt = q.value(1).toString().toUtf8();
    return PasswordHash::verifyPassword(password, salt, storedHash);
}

QStringList Database::tableNames() const
{
    QStringList names;
    QSqlQuery q(m_db);
    q.exec(QStringLiteral(
        "SELECT name FROM sqlite_master WHERE type='table' AND name NOT IN ('users') ORDER BY name"));
    while (q.next())
        names.append(q.value(0).toString());
    return names;
}
