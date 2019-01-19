#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(SettingsManager settingsManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_settingsManager(settingsManager)
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
    emit applytodoDirectory(ui->todolistBinPathLineEdit->text());
    //QMessageBox(QMessageBox::Information, "Информация", "Перезагрузите приложение, чтобы настройки обновились.").exec();
    QStringList users = ui->usersTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    QStringList tags = ui->tagsTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    QStringList statuses = ui->statusesTextEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    int usersCount = users.size();
    m_settingsManager.set("Users", "Count", usersCount);
    for(size_t i = 0; i < (size_t) usersCount; i++)
    {
       QString key = QStringLiteral("User") + QString::number(i);
       m_settingsManager.set("Users", key, users[i]);
    }

    int tagsCount = tags.size();
    m_settingsManager.set("Tags", "Count", tagsCount);
    for(size_t i = 0; i < (size_t) tagsCount; i++)
    {
       QString key = QStringLiteral("Tag") + QString::number(i);
       m_settingsManager.set("Tags", key, tags[i]);
    }

    int statusesCount = statuses.size();
    m_settingsManager.set("Statuses", "Count", statusesCount);
    for(size_t i = 0; i < (size_t) statusesCount; i++)
    {
       QString key = QStringLiteral("Status") + QString::number(i);
       m_settingsManager.set("Statuses", key, statuses[i]);
    }

    QString defaultPath = ui->defaultPathLineEdit->text();
    m_settingsManager.set("General", "DefaultTasksPath", defaultPath);

    m_settingsManager.saveSettings();
    this->close();
}

void SettingsDialog::setup()
{
    ui->todolistBinPathLineEdit->clear();
    ui->defaultPathLineEdit->clear();
    ui->usersTextEdit->clear();
    ui->tagsTextEdit->clear();
    ui->statusesTextEdit->clear();

    QString todoListBinPath = "";
    QString defaultTasksPath = "";
    QString users;
    QString tags;
    QString statuses;

    try
    {
        todoListBinPath = m_settingsManager.get("General", "TodoListBinPath").toString();

        defaultTasksPath = m_settingsManager.get("General", "DefaultTasksPath").toString();

        int userCount = m_settingsManager.get("Users", "Count").toInt();
        if(userCount > 0)
        {
            for(size_t i = 0; i < (size_t) userCount; i++)
            {
                QString key = QStringLiteral("User") + QString::number(i);
                QString user = m_settingsManager.get("Users", key).toString() + "\n";
                users.push_back(user);
            }
        }

        int tagsCount = m_settingsManager.get("Tags", "Count").toInt();
        if(tagsCount > 0)
        {
            for(size_t i = 0; i < (size_t) tagsCount; i++)
            {
                QString key = QStringLiteral("Tag") + QString::number(i);
                QString tag = m_settingsManager.get("Tags", key).toString() + "\n";
                tags.push_back(tag);
            }
        }

        int statusesCount = m_settingsManager.get("Statuses", "Count").toInt();
        if(statusesCount > 0)
        {
            for(size_t i = 0; i < (size_t) statusesCount; i++)
            {
                QString key = QStringLiteral("Status") + QString::number(i);
                QString status = m_settingsManager.get("Statuses", key).toString() + "\n";
                statuses.push_back(status);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "SettingsError", e.what()).exec();
    }

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
