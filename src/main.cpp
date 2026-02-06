#include <QApplication>
#include <QMessageBox>
#include "database.h"
#include "dbselectwindow.h"
#include "loginwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("DemoQt");

    DbSelectWindow dbSelectWindow;
    LoginWindow loginWindow;

    QObject::connect(&dbSelectWindow, &DbSelectWindow::openExistingRequested, [&]() {
        dbSelectWindow.close();
        loginWindow.show();
    });

    QObject::connect(&dbSelectWindow, &DbSelectWindow::newDatabaseCreated, [&]() {
        dbSelectWindow.close();
        MainWindow *mainWindow = new MainWindow();
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->show();
    });

    QObject::connect(&dbSelectWindow, &QDialog::finished, [&](int result) {
        if (result == QDialog::Rejected)
            app.quit();
    });

    QObject::connect(&loginWindow, &LoginWindow::loginSucceeded, [&]() {
        loginWindow.close();
        MainWindow *mainWindow = new MainWindow();
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->show();
    });

    QObject::connect(&loginWindow, &QDialog::finished, [&](int result) {
        if (result == QDialog::Rejected)
            app.quit();
    });

    dbSelectWindow.show();

    return app.exec();
}
