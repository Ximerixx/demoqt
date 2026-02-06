#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class NewUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewUserDialog(QWidget *parent = nullptr);

    QString login() const;
    QString password() const;

private slots:
    void onCreateClicked();

private:
    QLineEdit *m_loginEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QPushButton *m_createButton = nullptr;
};

#endif // NEWUSERDIALOG_H
