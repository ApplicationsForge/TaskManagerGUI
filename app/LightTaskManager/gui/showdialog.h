#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QDialog>

#include "models/taskmanager.h"

namespace Ui {
class ShowDialog;
}

class ShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowDialog(TaskManager& taskManager, QMap<QString, QString> map, QWidget *parent = 0);
    ~ShowDialog();

signals:
    void saveTask(QString index, QString title, QDate date, QStringList tags, QStringList users, QString subject);

private slots:
    void on_showButtonBox_accepted();

private:
    Ui::ShowDialog *ui;
    TaskManager& m_taskManager;
    QMap<QString, QString> m_map;

    QString m_index;
    QString m_title;
    QDate m_date;
    QString m_tags;
    QString m_users;
    QString m_subject;

    void setup();
};

#endif // SHOWDIALOG_H
