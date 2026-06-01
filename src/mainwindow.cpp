#include "mainwindow.h"
#include "database.h"
#include "tabletab.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Главное окно"));
    setMinimumSize(800, 600);
    resize(1000, 700);

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    setupTabs();
}

void MainWindow::setupTabs()
{
    Database &db = Database::instance();
    if (!db.isOpen()) {
        qWarning("MainWindow::setupTabs: база данных не открыта");
        QWidget *placeholder = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(placeholder);
        layout->addWidget(new QLabel(tr("База данных не открыта."), placeholder));
        m_tabWidget->addTab(placeholder, tr("Нет данных"));
        return;
    }

    QStringList tables = db.tableNames();
    qDebug("MainWindow::setupTabs: таблиц в БД: %d", static_cast<int>(tables.size()));

    for (const QString &tableName : tables) {
        if (tableName.isEmpty())
            continue;
        TableTab *tab = new TableTab(tableName, m_tabWidget);
        if (tab->isValid()) {
            m_tabWidget->addTab(tab, tableName);
        } else {
            tab->deleteLater();
        }
    }

    if (m_tabWidget->count() == 0) {
        QWidget *placeholder = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(placeholder);
        layout->addWidget(new QLabel(tr("Нет таблиц в базе данных."), placeholder));
        m_tabWidget->addTab(placeholder, tr("Нет данных"));
    }
}
