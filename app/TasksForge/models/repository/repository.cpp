#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_taskTerminalBinPath(""),
    m_defaultTaskRepositoryPath(""),
    m_tasks(QList<Task>()),
    m_tagFilter(""),
    m_userFilter("")
{
    qDebug() << "new instance of task Manager";
    try
    {
        m_taskTerminalBinPath = m_settingsManager->get("General", "TodoListBinPath").toString();
        m_defaultTaskRepositoryPath = m_settingsManager->get("General", "DefaultTasksPath").toString();
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", e.what()).exec();
    }
}

void Repository::setTagFilter(const QString &tagFilter)
{
    m_tagFilter = tagFilter;
}

void Repository::setUserFilter(const QString &userFilter)
{
    m_userFilter = userFilter;
}

void Repository::setTaskTerminalBinPath(const QString path)
{
    m_settingsManager->set("General", "TodoListBinPath", path);
    m_settingsManager->saveSettings();

    m_taskTerminalBinPath = m_settingsManager->get("General", "TodoListBinPath").toString();
}

QString Repository::taskTerminalBinPath() const
{
    return m_taskTerminalBinPath;
}

QString Repository::defaultTaskRepositoryPath() const
{
    return m_defaultTaskRepositoryPath;
}

void Repository::setDefaultTaskRepositoryPath(const QString path)
{
    m_settingsManager->set("General", "DefaultTasksPath", path);
    m_settingsManager->saveSettings();

    m_defaultTaskRepositoryPath = m_settingsManager->get("General", "DefaultTasksPath").toString();
}

QList<Task> Repository::tasks() const
{
    QList<Task> filteredTasks = QList<Task>();

    for(auto task : m_tasks)
    {
        if((task.tags().join(" ").contains(m_tagFilter)) || m_tagFilter.isEmpty())
        {
            if((task.users().join(" ").contains(m_userFilter)) || m_userFilter.isEmpty())
            {
                filteredTasks.push_back(task);
            }
        }
    }

    std::reverse(filteredTasks.begin(), filteredTasks.end());
    return filteredTasks;
}

QStringList Repository::readStatuses()
{
    QStringList statuses;
    try
    {
        int statusesCount = m_settingsManager->get("Statuses", "Count").toInt();
        if(statusesCount > 0)
        {
            for(size_t i = 0; i < (size_t) statusesCount; i++)
            {
                QString key = QStringLiteral("Status") + QString::number(i);
                QString statusName = m_settingsManager->get("Statuses", key).toString();
                statuses.push_back(statusName);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "SettingsError", e.what());
    }
    return statuses;
}

QStringList Repository::readTags()
{
    QStringList tags;
    try
    {
        int tagsCount = m_settingsManager->get("Tags", "Count").toInt();
        if(tagsCount > 0)
        {
            for(int i = 0; i < tagsCount; i++)
            {
                QString key = QStringLiteral("Tag") + QString::number(i);
                QString tagName = m_settingsManager->get("Tags", key).toString();
                tags.push_back(tagName);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "SettingsError", e.what());
    }
    return tags;
}

QStringList Repository::readUsers()
{
    QStringList users;
    try
    {
        int usersCount = m_settingsManager->get("Users", "Count").toInt();
        if(usersCount > 0)
        {
            for(int i = 0; i < usersCount; i++)
            {
                QString key = QStringLiteral("User") + QString::number(i);
                QString userName = m_settingsManager->get("Users", key).toString();
                users.push_back(userName);
            }
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "SettingsError", e.what());
    }
    return users;
}

void Repository::setTagsCount(unsigned int count)
{
    m_settingsManager->set("Tags", "Count", count);
    m_settingsManager->saveSettings();
}

void Repository::addTag(QString key, QString value)
{
    m_settingsManager->set("Tags", key, value);
    m_settingsManager->saveSettings();
}

void Repository::setUsersCount(unsigned int count)
{
    m_settingsManager->set("Users", "Count", count);
    m_settingsManager->saveSettings();
}

void Repository::addUser(QString key, QString value)
{
    m_settingsManager->set("Users", key, value);
    m_settingsManager->saveSettings();
}

void Repository::setStatusesCount(unsigned int count)
{
    m_settingsManager->set("Statuses", "Count", count);
    m_settingsManager->saveSettings();
}

void Repository::addStatus(QString key, QString value)
{
    m_settingsManager->set("Statuses", key, value);
    m_settingsManager->saveSettings();
}
