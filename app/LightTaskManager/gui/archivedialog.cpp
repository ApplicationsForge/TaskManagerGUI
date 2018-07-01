#include "archivedialog.h"
#include "ui_archivedialog.h"

ArchiveDialog::ArchiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveDialog)
{
    ui->setupUi(this);
}

ArchiveDialog::~ArchiveDialog()
{
    delete ui;
}
