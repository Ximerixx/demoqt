#ifndef DBSELECTWINDOW_H
#define DBSELECTWINDOW_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class DbSelectWindow : public QDialog
{
    Q_OBJECT
public:
    explicit DbSelectWindow(QWidget *parent = nullptr);

signals:
    void openExistingRequested();
    void newDatabaseCreated();

private slots:
    void onBrowseClicked();
    void onOpenClicked();
    void onNewDatabaseClicked();
    void onQuitClicked();

private:
    bool createNewDatabaseWithUser(const QString &path);

    QLineEdit *m_pathEdit = nullptr;
    QPushButton *m_browseButton = nullptr;
    QPushButton *m_openButton = nullptr;
    QPushButton *m_newDbButton = nullptr;
    QPushButton *m_quitButton = nullptr;
};

#endif // DBSELECTWINDOW_H
