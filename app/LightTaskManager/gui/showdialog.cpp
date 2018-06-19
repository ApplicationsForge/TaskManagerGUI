#include "showdialog.h"
#include "ui_showdialog.h"

ShowDialog::ShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowDialog)
{
    ui->setupUi(this);
}

ShowDialog::~ShowDialog()
{
    delete ui;
}
