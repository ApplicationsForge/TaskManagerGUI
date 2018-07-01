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

void ArchiveDialog::on_archiveByStatusPushButton_clicked()
{
    emit archiveByStatus(ui->archiveByStatusLineEdit->text());
    this->close();
}

void ArchiveDialog::on_gcPushButton_clicked()
{
    emit garbageCollection();
    this->close();
}
