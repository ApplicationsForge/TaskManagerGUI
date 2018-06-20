#include "showdialog.h"
#include "ui_showdialog.h"

ShowDialog::ShowDialog(TaskManager &taskManager, QMap<QString,QString> map, QWidget *parent) :
    QDialog(parent),
    m_taskManager(taskManager),
    m_map(map),
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
    m_index = m_map.value("index");
    ui->indexLineEdit->setText(m_index);

    m_title = m_map.value("title");
    ui->titleLineEdit->setText(m_title);

    QString tmp_date = m_map.value("date");
    m_date.fromString(tmp_date, "dd/MM/YYYY");
    ui->calendarWidget->setSelectedDate(m_date);

    m_tags = m_map.value("tags");
    ui->tagTextEdit->setPlainText(m_tags);

    m_users = m_map.value("users");
    ui->userTextEdit->setPlainText(m_users);

    m_subject = m_map.value("description");
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
}
