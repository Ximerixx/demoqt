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

    QSqlQuery pragma(m_db);
    pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON"));

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
        "CREATE TABLE IF NOT EXISTS employees ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "full_name TEXT NOT NULL,"
        "position TEXT NOT NULL,"
        "department TEXT NOT NULL)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS components ("
        "id TEXT PRIMARY KEY,"
        "name_gost TEXT NOT NULL,"
        "alternative_names TEXT,"
        "type TEXT NOT NULL,"
        "characteristics TEXT)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS clients ("
        "id TEXT PRIMARY KEY,"
        "phone TEXT,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "discount_percent REAL)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS sales ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "client_id TEXT NOT NULL REFERENCES clients(id),"
        "employee_id INTEGER NOT NULL REFERENCES employees(id),"
        "sale_datetime TEXT NOT NULL)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS sale_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sale_id INTEGER NOT NULL REFERENCES sales(id),"
        "component_id TEXT NOT NULL REFERENCES components(id),"
        "quantity INTEGER NOT NULL,"
        "unit_price REAL)"
    ))) {
        m_lastError = q.lastError().text();
        return false;
    }

    if (!q.exec(QStringLiteral(
        "CREATE VIEW IF NOT EXISTS v_sales_detail AS "
        "SELECT s.id AS sale_id, s.sale_datetime, s.client_id, "
        "(c.first_name || ' ' || c.last_name) AS client_name, "
        "s.employee_id, e.full_name AS employee_name, "
        "si.component_id, comp.name_gost AS component_name, "
        "si.quantity, si.unit_price, "
        "(si.quantity * COALESCE(si.unit_price, 0)) AS line_total "
        "FROM sales s "
        "JOIN sale_items si ON s.id = si.sale_id "
        "JOIN components comp ON si.component_id = comp.id "
        "JOIN clients c ON s.client_id = c.id "
        "JOIN employees e ON s.employee_id = e.id"))) {
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
    if (!m_db.isOpen())
        return names;
    QSqlQuery q(m_db);
    if (!q.exec(QStringLiteral(
        "SELECT name FROM sqlite_master WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%' ORDER BY name")))
        return names;
    while (q.next()) {
        QString name = q.value(0).toString();
        if (!name.isEmpty())
            names.append(name);
    }
    return names;
}
