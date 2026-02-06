#include "mainwindow.h"
#include "database.h"
#include "tabletab.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

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
    QStringList tables = Database::instance().tableNames();
    for (const QString &tableName : tables) {
        TableTab *tab = new TableTab(tableName, this);
        m_tabWidget->addTab(tab, tableName);
    }

    if (tables.isEmpty()) {
        QWidget *placeholder = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(placeholder);
        layout->addWidget(new QLabel(tr("Нет таблиц в базе данных."), this));
        m_tabWidget->addTab(placeholder, tr("Нет данных"));
    }
}
