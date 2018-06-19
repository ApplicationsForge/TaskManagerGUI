#ifndef ARCHIVEBYSTATUSDIALOG_H
#define ARCHIVEBYSTATUSDIALOG_H

#include <QDialog>

namespace Ui {
class ArchiveByStatusDialog;
}

class ArchiveByStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchiveByStatusDialog(QWidget *parent = 0);
    ~ArchiveByStatusDialog();

private:
    Ui::ArchiveByStatusDialog *ui;
};

#endif // ARCHIVEBYSTATUSDIALOG_H
