#include "showdialog.h"
#include "ui_showdialog.h"

ShowDialog::ShowDialog(TaskManager &taskManager, QWidget *parent) :
    QDialog(parent),
    m_taskManager(taskManager),
    ui(new Ui::ShowDialog)
{
    ui->setupUi(this);
    setup();
}

ShowDialog::~ShowDialog()
{
    delete ui;
}

void ShowDialog::setup()
{
    ui->indexLineEdit->clear();
    m_index = m_taskManager.getTaskIndex("1 task1 +tag1 @user");
    ui->indexLineEdit->setText(m_index);
/*
    ui->currentTaskIndexLineEdit->setText(item->index());
    ui->currentTaskTitleLineEdit->setText(item->title());
    ui->currentTaskTagsListWidget->addItems(item->tags().split(" ", QString::SkipEmptyParts));
    ui->currentTaskDateLineEdit->setText(item->date());
    ui->currentTaskUsersListWidget->addItems(item->users().split(" ", QString::SkipEmptyParts));
    ui->currentTaskDescriptionPlainTextEdit->setPlainText(item->description());
    ui->editTaskPushButton->setEnabled(true);*/

    ui->tagTextEdit->clear();
    m_tags = m_taskManager.readTags();
    //ui->tagTextEdit->setText(m_tags);

    ui->userTextEdit->clear();
    m_users = m_taskManager.readUsers();
    //ui->userTextEdit->setText(m_users);
}
