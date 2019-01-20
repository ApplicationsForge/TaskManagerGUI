#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setup();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    Router& router = Router::getInstance();
    emit applytodoDirectory(ui->todolistBinPathLineEdit->text());
    //QMessageBox(QMessageBox::Information, "Информация", "Перезагрузите приложение, чтобы настройки обновились.").exec();
    QStringList users = ui->usersTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    QStringList tags = ui->tagsTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    QStringList statuses = ui->statusesTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    int usersCount = users.size();
    router.getRepository()->setUsersCount(usersCount);
    for(int i = 0; i < usersCount; i++)
    {
       QString key = QStringLiteral("User") + QString::number(i);
       router.getRepository()->addUser(key, users[i]);
    }

    int tagsCount = tags.size();
    router.getRepository()->setTagsCount(tagsCount);
    for(int i = 0; i < tagsCount; i++)
    {
       QString key = QStringLiteral("Tag") + QString::number(i);
       router.getRepository()->addTag(key, tags[i]);
    }

    int statusesCount = statuses.size();
    router.getRepository()->setStatusesCount(statusesCount);
    for(size_t i = 0; i < (size_t) statusesCount; i++)
    {
       QString key = QStringLiteral("Status") + QString::number(i);
       router.getRepository()->addStatus(key, statuses[i]);
    }

    QString defaultPath = ui->defaultPathLineEdit->text();
    router.getRepository()->setDefaultTaskRepositoryPath(defaultPath);
    this->close();
}

void SettingsDialog::setup()
{
    Router& router = Router::getInstance();

    ui->todolistBinPathLineEdit->clear();
    ui->defaultPathLineEdit->clear();
    ui->usersTextEdit->clear();
    ui->tagsTextEdit->clear();
    ui->statusesTextEdit->clear();

    QString todoListBinPath = router.getRepository()->taskTerminalBinPath();
    QString defaultTasksPath = router.getRepository()->defaultTaskRepositoryPath();

    QStringList userList = router.getRepository()->readUsers();
    QStringList tagList = router.getRepository()->readTags();
    QStringList statusList = router.getRepository()->readStatuses();

    QString users = userList.join("\n");
    QString tags = tagList.join("\n");
    QString statuses = statusList.join("\n");

    ui->todolistBinPathLineEdit->setText(todoListBinPath);
    ui->defaultPathLineEdit->setText(defaultTasksPath);
    ui->usersTextEdit->setText(users);
    ui->tagsTextEdit->setText(tags);
    ui->statusesTextEdit->setText(statuses);
}

void SettingsDialog::on_todolistBinPathToolButton_clicked()
{
    //QString str = QFileDialog::getExistingDirectory(0, "", "");
    QString path = QFileDialog::getOpenFileName(this, "Выберите путь до программы Taskterminal", "");
    if(path.length() > 0)
    {
        ui->todolistBinPathLineEdit->clear();
        ui->todolistBinPathLineEdit->setText(path);
    }
}

void SettingsDialog::on_defaultPathToolButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "", "");
    //QString path = QFileDialog::getOpenFileName(this, "Выберите путь до файза с задачами по умолчанию", "*.json");
    if(path.length() > 0)
    {
        ui->defaultPathLineEdit->clear();
        ui->defaultPathLineEdit->setText(path);
    }
}
