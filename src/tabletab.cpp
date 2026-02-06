#include "tabletab.h"
#include "database.h"
#include <QTableView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QLabel>

TableTab::TableTab(const QString &tableName, QWidget *parent)
    : QWidget(parent)
    , m_tableName(tableName)
{
    QSqlDatabase db = Database::instance().connection();

    m_tableModel = new QSqlTableModel(this, db);
    m_tableModel->setTable(m_tableName);
    m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_tableModel->select();

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_tableModel);
    m_tableView->setAlternatingRowColors(true);

    m_queryModel = new QSqlQueryModel(this);

    m_sqlEdit = new QPlainTextEdit(this);
    m_sqlEdit->setPlaceholderText(tr("Введите SQL запрос..."));
    m_sqlEdit->setMaximumHeight(120);

    QPushButton *executeButton = new QPushButton(tr("Выполнить"), this);
    connect(executeButton, &QPushButton::clicked, this, &TableTab::onExecuteSqlClicked);

    m_resultView = new QTableView(this);
    m_resultView->setModel(m_queryModel);
    m_resultView->setAlternatingRowColors(true);

    QVBoxLayout *sqlLayout = new QVBoxLayout;
    sqlLayout->addWidget(new QLabel(tr("Произвольный SQL:"), this));
    sqlLayout->addWidget(m_sqlEdit);
    sqlLayout->addWidget(executeButton);
    sqlLayout->addWidget(new QLabel(tr("Результат запроса:"), this));
    sqlLayout->addWidget(m_resultView);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel(tr("Таблица: %1").arg(m_tableName), this));
    mainLayout->addWidget(m_tableView);
    mainLayout->addLayout(sqlLayout);
}

void TableTab::onExecuteSqlClicked()
{
    QString sql = m_sqlEdit->toPlainText().trimmed();
    if (sql.isEmpty()) {
        QMessageBox::information(this, tr("Подсказка"), tr("Введите SQL запрос."));
        return;
    }

    m_queryModel->setQuery(sql, Database::instance().connection());
    if (m_queryModel->lastError().isValid()) {
        QMessageBox::critical(this, tr("Ошибка SQL"),
            tr("Ошибка выполнения запроса: %1").arg(m_queryModel->lastError().text()));
        return;
    }
}
