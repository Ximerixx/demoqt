#include "loginwindow.h"
#include "database.h"
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Вход"));
    setMinimumWidth(320);

    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText(tr("Логин"));

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(tr("Пароль"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton(tr("Войти"), this);
    m_quitButton = new QPushButton(tr("Выход"), this);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_quitButton, &QPushButton::clicked, this, &LoginWindow::onQuitClicked);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Логин:"), m_loginEdit);
    formLayout->addRow(tr("Пароль:"), m_passwordEdit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(m_loginButton);
    layout->addWidget(m_quitButton);
}

void LoginWindow::onLoginClicked()
{
    QString login = m_loginEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (login.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите логин."));
        return;
    }

    Database &db = Database::instance();
    if (!db.verifyLogin(login, password)) {
        QMessageBox::warning(this, tr("Ошибка входа"),
            tr("Неверный логин или пароль."));
        return;
    }

    emit loginSucceeded();
}

void LoginWindow::onQuitClicked()
{
    reject();
    QApplication::quit();
}
