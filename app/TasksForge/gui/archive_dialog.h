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
    void unarchive(QString index);
    void lArchived();

public:
    explicit ArchiveDialog(QWidget *parent = nullptr);
    ~ArchiveDialog();

private slots:
    void show();
    void on_archiveByStatusPushButton_clicked();

    void on_gcPushButton_clicked();

    void on_unarchivePushButton_clicked();

private:
    Ui::ArchiveDialog *ui;
};

#endif // ARCHIVEDIALOG_H
