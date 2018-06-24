#ifndef ARCHIVEBYSTATUSDIALOG_H
#define ARCHIVEBYSTATUSDIALOG_H

#include <QDialog>

namespace Ui {
class ArchiveByStatusDialog;
}

class ArchiveByStatusDialog : public QDialog
{
    Q_OBJECT

signals:
    void archiveByStatus(QString status);

public:
    explicit ArchiveByStatusDialog(QWidget *parent = 0);
    ~ArchiveByStatusDialog();

private slots:
    void on_archiveByStatusButtonBox_accepted();

private:
    Ui::ArchiveByStatusDialog *ui;
};

#endif // ARCHIVEBYSTATUSDIALOG_H
