#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent) :
    QObject(parent),  
    m_settingsManager(new SettingsManager()),
    m_tasks(QList<Task>()),
    m_tagFilter(""),
    m_userFilter("")
{
    qDebug() << "new instance of task Manager";
    try
    {
        QString todolistPath = m_settingsManager->get("General", "TodoListBinPath").toString();
        QString defaultTasksPath = m_settingsManager->get("General", "DefaultTasksPath").toString();
        m_adapter = QSharedPointer<TaskTerminalAdapter> (new TaskTerminalAdapter(todolistPath, defaultTasksPath, this));
        connect(m_adapter.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(onCurrentDirectoryChanged(QString)));
        connect(m_adapter.data(), SIGNAL(tasksUpdated(QList<Task>)), this, SLOT(onTaskTerminalAdapter_TasksUpdated(QList<Task>)));
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", e.what()).exec();
    }
}

TaskManager::~TaskManager()
{

}

void TaskManager::setTagFilter(const QString &tagFilter)
{
    m_tagFilter = tagFilter;
}

void TaskManager::setUserFilter(const QString &userFilter)
{
    m_userFilter = userFilter;
}

void TaskManager::runCommand(QString command)
{
    m_adapter->runCommand(command);
}

void TaskManager::openRepository(QString directory)
{
    m_adapter->openRepository(directory);
}

void TaskManager::initializeRepository(QString directory)
{
    m_adapter->initializeRepository(directory);
}

void TaskManager::reopenRepository()
{
    openRepository(m_adapter->currentDirectory());
}

void TaskManager::onCurrentDirectoryChanged(QString directory)
{
    emit directoryUpdated(directory);
}

void TaskManager::onTaskTerminalAdapter_TasksUpdated(QList<Task> tasks)
{
    m_tasks = tasks;
    emit taskListUpdated();
}

void TaskManager::changeTaskStatus(QString data, QString status)
{
    QString index = data.split(" ", QString::SkipEmptyParts).first();
    bool isCorrect;
    size_t number = index.toUInt(&isCorrect);
    if(isCorrect)
    {
        changeTaskStatus(number, status);
    }
    else
    {
        QMessageBox(QMessageBox::Warning, "Ошибка", "Невозможно сменить статус задачи №" + index).exec();
    }
}

void TaskManager::changeTaskStatus(size_t index, QString status)
{
    emit statusMessage("change task " + QString::number(index) + " status to " + status);
    m_adapter->changeTaskStatus(index, status);
}

void TaskManager::addTask(QString task)
{
    emit statusMessage("add task " + task);
    m_adapter->addTask(task);
}

void TaskManager::deleteTask(QString index)
{
    emit statusMessage("delete task " + index);
    m_adapter->deleteTask(index.toUInt());
}

void TaskManager::editTask(QString index, QString task)
{
    emit statusMessage("edit task " + index);
    m_adapter->editTask(index.toUInt(), task);
}

void TaskManager::lArchived()
{
    emit statusMessage("l archived");
    m_adapter->lArchived();
    connect(m_adapter.data(), SIGNAL(lArchive(QByteArray)), this, SLOT(onTaskTerminalAdapter_TasksUpdated(QByteArray)));
}

void TaskManager::archiveByStatus(QString status)
{
    emit statusMessage("archive_by_status" + status);
    m_adapter->archiveByStatus(status);
}

void TaskManager::unarchive(QString index)
{
    emit statusMessage("unarchive " + index);
    m_adapter->unarchive(index.toUInt());
}

void TaskManager::garbageCollection()
{
    emit statusMessage("garbage collection");
    m_adapter->garbageCollection();
}

QList<Task> TaskManager::tasks()
{
    qDebug() << m_tagFilter << m_userFilter;
    QList<Task> filteredTasks = QList<Task>();

    for(auto task : m_tasks)
    {
        qDebug() << task.tags().join(" ");
        if((task.tags().join(" ").contains(m_tagFilter)) || m_tagFilter.isEmpty())
        {
            qDebug() << "passed";
            if((task.users().join(" ").contains(m_userFilter)) || m_userFilter.isEmpty())
            {
                qDebug() << "passed";
                filteredTasks.push_back(task);
            }
        }
    }
    std::reverse(filteredTasks.begin(), filteredTasks.end());
    return filteredTasks;
}

void TaskManager::setWorkingDirectory(QString directory)
{
    m_settingsManager->set("General", "TodoListBinPath", directory);
    m_settingsManager->saveSettings();

    QString todolistPath = m_settingsManager->get("General", "TodoListBinPath").toString();
    m_adapter->setBinPath(todolistPath);

    emit statusMessage("set Todolist directory " + directory);
}

QString TaskManager::getWorkingDirectory()
{
    return m_adapter->currentTodoListBinPath();
}

void TaskManager::setDefaultTasksPath(QString path)
{
    m_adapter->setDefaultPath(path);
}

QString TaskManager::getDefaultTasksPath()
{
    return m_adapter->getDefaultPath();
}

QStringList TaskManager::readStatuses()
{
    emit statusMessage("read statuses");
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

QStringList TaskManager::readTags()
{
    emit statusMessage("read tags");
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

QStringList TaskManager::readUsers()
{
    emit statusMessage("read users");
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

SettingsManager &TaskManager::getSettingsManager()
{
    return (*(m_settingsManager.data()));
}

QStringList TaskManager::filterByTagName(QStringList allTasks)
{
    QString tagTemplate = QString(QString("+") + m_tagFilter);
    QStringList filteredTasks;
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == tagTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}

QStringList TaskManager::filterByUserName(QStringList allTasks)
{
    QStringList filteredTasks;
    QString userTemplate = QString(QString("@") + m_userFilter);
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == userTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}
