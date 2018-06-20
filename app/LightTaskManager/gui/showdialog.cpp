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
    m_index = m_taskManager.getTaskIndex("task 1 #Title +tag1 @user");
    ui->indexLineEdit->setText(m_index);

    ui->titleLineEdit->clear();
    m_title = m_taskManager.getTitle("task 1 #Title +tag1 @user");
    ui->titleLineEdit->setText(m_title);

    QDate date = ui->calendarWidget->selectedDate();
    date.setDate(2018, 06, 03);
    ui->calendarWidget->setSelectedDate(date);
/*
    ui->currentTaskIndexLineEdit->setText(item->index());
    ui->currentTaskTitleLineEdit->setText(item->title());
    ui->currentTaskTagsListWidget->addItems(item->tags().split(" ", QString::SkipEmptyParts));
    ui->currentTaskDateLineEdit->setText(item->date());
    ui->currentTaskUsersListWidget->addItems(item->users().split(" ", QString::SkipEmptyParts));
    ui->currentTaskDescriptionPlainTextEdit->setPlainText(item->description());
    ui->editTaskPushButton->setEnabled(true);*/

    ui->tagTextEdit->clear();
    m_tags = m_taskManager.getTags("task 1 #Title +tag1 @user").split(" ", QString::SkipEmptyParts);
    //ui->tagTextEdit->setPlainText(m_tags[0]);

    ui->userTextEdit->clear();
    m_users = m_taskManager.getUsers("task 1 #Title +tag1 @user").split(" ", QString::SkipEmptyParts);
    //ui->userTextEdit->setPlainText(m_users[0]);

    ui->showTextEdit->clear();
    m_subject = m_taskManager.getDescription("task 1 #Title +tag1 @user");
    ui->showTextEdit->setText(m_subject);
}

void ShowDialog::on_showButtonBox_accepted()
{
    QString index = ui->indexLineEdit->text();
    QString title = ui->titleLineEdit->text();
    QDate date = ui->calendarWidget->selectedDate();
    QStringList tags = ui->tagTextEdit->toPlainText().split(" ", QString::SkipEmptyParts);
    QStringList users = ui->userTextEdit->toPlainText().split(" ", QString::SkipEmptyParts);
    QString subject = ui->showTextEdit->toPlainText();

    emit saveTask(index, title, date, tags, users, subject);
    ui->indexLineEdit->clear();
    ui->titleLineEdit->clear();
    ui->tagTextEdit->clear();
    ui->userTextEdit->clear();
    ui->showTextEdit->clear();
}
