#include "archive_dialog.h"
#include "ui_archive_dialog.h"

ArchiveDialog::ArchiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveDialog)
{
    ui->setupUi(this);
    ui->gcPushButton->hide();
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

void ArchiveDialog::on_unarchivePushButton_clicked()
{
    emit unarchive(ui->unarchiveLineEdit->text());
    this->close();
}
