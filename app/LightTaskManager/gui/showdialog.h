#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QDialog>

#include "models/taskmanager.h"
#include "gui/widgets/mylistwidget.h"
#include "gui/widgets/mylistwidgetitem.h"

namespace Ui {
class ShowDialog;
}

class ShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowDialog(TaskManager& taskManager, QWidget *parent = 0);
    ~ShowDialog();

private:
    Ui::ShowDialog *ui;
    TaskManager& m_taskManager;

    QString m_index;
    QString m_title;
    QDate m_date;
    QStringList m_tags;
    QStringList m_users;

    void setup();
};

#endif // SHOWDIALOG_H
