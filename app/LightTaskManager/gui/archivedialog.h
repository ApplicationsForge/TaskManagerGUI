#ifndef ARCHIVEDIALOG_H
#define ARCHIVEDIALOG_H

#include <QDialog>

namespace Ui {
class ArchiveDialog;
}

class ArchiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchiveDialog(QWidget *parent = 0);
    ~ArchiveDialog();

private:
    Ui::ArchiveDialog *ui;
};

#endif // ARCHIVEDIALOG_H
