#include "delete_task_dialog.h"
#include "ui_delete_task_dialog.h"

DeleteTaskDialog::DeleteTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteTaskDialog)
{
    ui->setupUi(this);
}

DeleteTaskDialog::~DeleteTaskDialog()
{
    delete ui;
}

void DeleteTaskDialog::on_buttonBox_accepted()
{
    emit deleteTask(ui->taskIndexLineEdit->text());
}
