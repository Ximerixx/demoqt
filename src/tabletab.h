#ifndef TABLETAB_H
#define TABLETAB_H

#include <QWidget>

class QTableView;
class QPlainTextEdit;
class QSqlTableModel;
class QSqlQueryModel;

class TableTab : public QWidget
{
    Q_OBJECT
public:
    explicit TableTab(const QString &tableName, QWidget *parent = nullptr);
    bool isValid() const { return m_valid; }

private slots:
    void onExecuteSqlClicked();

private:
    QString m_tableName;
    bool m_valid = false;
    QTableView *m_tableView = nullptr;
    QPlainTextEdit *m_sqlEdit = nullptr;
    QTableView *m_resultView = nullptr;
    QSqlTableModel *m_tableModel = nullptr;
    QSqlQueryModel *m_queryModel = nullptr;
};

#endif // TABLETAB_H
