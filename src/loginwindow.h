#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSucceeded();

private slots:
    void onLoginClicked();
    void onQuitClicked();

private:
    QLineEdit *m_loginEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QPushButton *m_loginButton = nullptr;
    QPushButton *m_quitButton = nullptr;
};

#endif // LOGINWINDOW_H
