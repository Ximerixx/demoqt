#include "dbselectwindow.h"
#include "database.h"
#include "newuserdialog.h"
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QLabel>

DbSelectWindow::DbSelectWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Выбор базы данных"));
    setMinimumWidth(480);

    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setPlaceholderText(tr("Путь к файлу базы данных"));

    m_browseButton = new QPushButton(tr("Обзор"), this);
    m_openButton = new QPushButton(tr("Открыть"), this);
    m_newDbButton = new QPushButton(tr("Новая база данных"), this);
    m_quitButton = new QPushButton(tr("Выход"), this);

    connect(m_browseButton, &QPushButton::clicked, this, &DbSelectWindow::onBrowseClicked);
    connect(m_openButton, &QPushButton::clicked, this, &DbSelectWindow::onOpenClicked);
    connect(m_newDbButton, &QPushButton::clicked, this, &DbSelectWindow::onNewDatabaseClicked);
    connect(m_quitButton, &QPushButton::clicked, this, &DbSelectWindow::onQuitClicked);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(m_pathEdit);
    pathLayout->addWidget(m_browseButton);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Путь к базе данных:"), this));
    layout->addLayout(pathLayout);
    layout->addWidget(m_openButton);
    layout->addWidget(m_newDbButton);
    layout->addWidget(m_quitButton);
}

void DbSelectWindow::onBrowseClicked()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Выберите файл базы данных"),
        QString(),
        tr("База данных SQLite (*.db *.sqlite *.sqlite3);;Все файлы (*)"));
    if (!path.isEmpty())
        m_pathEdit->setText(path);
}

void DbSelectWindow::onOpenClicked()
{
    QString path = m_pathEdit->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите путь к файлу базы данных."));
        return;
    }
    if (!QFileInfo::exists(path)) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Файл не найден: %1").arg(path));
        return;
    }

    Database &db = Database::instance();
    if (!db.open(path)) {
        QMessageBox::critical(this, tr("Ошибка"), db.lastError());
        return;
    }

    emit openExistingRequested();
}

void DbSelectWindow::onNewDatabaseClicked()
{
    QString path = QFileDialog::getSaveFileName(this,
        tr("Новая база данных"),
        QString(),
        tr("База данных SQLite (*.db *.sqlite *.sqlite3);;Все файлы (*)"));
    if (path.isEmpty())
        return;

    if (!createNewDatabaseWithUser(path))
        return;

    emit newDatabaseCreated();
}

void DbSelectWindow::onQuitClicked()
{
    reject();
    QApplication::quit();
}

bool DbSelectWindow::createNewDatabaseWithUser(const QString &path)
{
    NewUserDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return false;

    Database &db = Database::instance();
    if (!db.open(path)) {
        QMessageBox::critical(this, tr("Ошибка"), db.lastError());
        return false;
    }

    if (!db.createUser(dlg.login(), dlg.password())) {
        QMessageBox::critical(this, tr("Ошибка"), db.lastError());
        db.close();
        return false;
    }

    return true;
}
