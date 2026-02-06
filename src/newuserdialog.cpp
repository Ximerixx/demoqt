#include "newuserdialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

NewUserDialog::NewUserDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Первый пользователь"));
    setMinimumWidth(320);

    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText(tr("Логин"));

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(tr("Пароль"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_createButton = new QPushButton(tr("Создать"), this);
    QPushButton *cancelButton = new QPushButton(tr("Отмена"), this);

    connect(m_createButton, &QPushButton::clicked, this, &NewUserDialog::onCreateClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Логин:"), m_loginEdit);
    formLayout->addRow(tr("Пароль:"), m_passwordEdit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(m_createButton);
    layout->addWidget(cancelButton);
}

QString NewUserDialog::login() const
{
    return m_loginEdit->text().trimmed();
}

QString NewUserDialog::password() const
{
    return m_passwordEdit->text();
}

void NewUserDialog::onCreateClicked()
{
    if (login().isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите логин."));
        return;
    }
    if (password().isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите пароль."));
        return;
    }
    accept();
}
