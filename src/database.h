#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QStringList>

class Database
{
public:
    static Database &instance();

    bool open(const QString &path);
    void close();
    bool isOpen() const { return m_db.isOpen(); }
    QString lastError() const { return m_lastError; }
    QSqlDatabase connection() const { return m_db; }
    QString currentPath() const { return m_dbPath; }

    bool createUser(const QString &login, const QString &password);
    bool verifyLogin(const QString &login, const QString &password);
    QStringList tableNames() const;

private:
    Database() = default;
    ~Database() = default;
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    bool ensureSchema();

    QSqlDatabase m_db;
    QString m_dbPath;
    QString m_lastError;
};

#endif // DATABASE_H
