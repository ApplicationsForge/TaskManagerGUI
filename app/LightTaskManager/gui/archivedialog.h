#ifndef ARCHIVEDIALOG_H
#define ARCHIVEDIALOG_H

#include <QDialog>

namespace Ui {
class ArchiveDialog;
}

class ArchiveDialog : public QDialog
{
    Q_OBJECT

signals:
    void archiveByStatus(QString status);
    void garbageCollection();

public:
    explicit ArchiveDialog(QWidget *parent = 0);
    ~ArchiveDialog();

private slots:
    void on_archiveByStatusPushButton_clicked();

    void on_gcPushButton_clicked();

private:
    Ui::ArchiveDialog *ui;
};

#endif // ARCHIVEDIALOG_H
