#include "archivebystatusdialog.h"
#include "ui_archivebystatusdialog.h"

ArchiveByStatusDialog::ArchiveByStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveByStatusDialog)
{
    ui->setupUi(this);
}

ArchiveByStatusDialog::~ArchiveByStatusDialog()
{
    delete ui;
}
