#include "router.h"

Router::Router(QObject *parent) :
    QObject(parent),
    m_repository(new Repository(this)),
    m_adapter(QSharedPointer<TaskTerminalAdapter> (new TaskTerminalAdapter(
                                                       m_repository->taskTerminalBinPath(),
                                                       m_repository->defaultTaskRepositoryPath(),
                                                       this)))
{
    setupConnections();
}

Router::~Router()
{
    resetConnections();
}

Router &Router::getInstance()
{
    static QScopedPointer<Router> m_instance;
    if(m_instance.data() == nullptr)
    {
        m_instance.reset( new Router() );
    }
    return *m_instance;
}

Repository *Router::getRepository()
{
    return m_repository.data();
}

void Router::setupConnections()
{
    QObject::connect(m_adapter.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(onTaskTerminalAdapter_DirectoryUpdated(QString)));
    QObject::connect(m_adapter.data(), SIGNAL(tasksUpdated(QList<Task>)), this, SLOT(onTaskTerminalAdapter_TasksUpdated(QList<Task>)));
}

void Router::resetConnections()
{
    QObject::disconnect(m_adapter.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(onTaskTerminalAdapter_DirectoryUpdated(QString)));
    QObject::disconnect(m_adapter.data(), SIGNAL(tasksUpdated(QList<Task>)), this, SLOT(onTaskTerminalAdapter_TasksUpdated(QList<Task>)));
}

void Router::onTaskTerminalAdapter_DirectoryUpdated(QString directory)
{
    emit this->directoryUpdated(directory);
}

void Router::onTaskTerminalAdapter_TasksUpdated(QList<Task> tasks)
{
    m_repository->m_tasks = tasks;
    emit this->taskListUpdated();
}

void Router::executeTaskTerminalCommand(QString command)
{
    m_adapter->runCommand(command);
}

void Router::openRepository(QString directory)
{
    m_adapter->openRepository(directory);
}

void Router::openRepository()
{
    m_adapter->openRepository(m_adapter->currentDirectory());
}

void Router::initializeRepository(QString directory)
{
    m_adapter->initializeRepository(directory);
}

void Router::changeTaskStatus(QString task, QString status)
{
    QString index = task.split(" ", QString::SkipEmptyParts).first();
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

void Router::changeTaskStatus(size_t index, QString status)
{
    emit statusMessage("change task " + QString::number(index) + " status to " + status);
    m_adapter->changeTaskStatus(index, status);
}

void Router::addTask(QString task)
{
    emit statusMessage("add task " + task);
    m_adapter->addTask(task);
}

void Router::deleteTask(QString index)
{
    emit statusMessage("delete task " + index);
    m_adapter->deleteTask(index.toUInt());
}

void Router::editTask(QString index, QString task)
{
    emit statusMessage("edit task " + index);
    m_adapter->editTask(index.toUInt(), task);
}

void Router::archiveByStatus(QString status)
{
    emit statusMessage("archive_by_status" + status);
    m_adapter->archiveByStatus(status);
}

void Router::unarchive(QString index)
{
    emit statusMessage("unarchive " + index);
    m_adapter->unarchive(index.toUInt());
}

void Router::garbageCollection()
{
    emit statusMessage("garbage collection");
    m_adapter->garbageCollection();
}

QString Router::getTaskTerminalBinPath()
{
    return m_repository->taskTerminalBinPath();
}

void Router::setTaskTerminalBinPath(QString path)
{
    m_repository->setTaskTerminalBinPath(path);
    m_adapter->setBinPath(m_repository->taskTerminalBinPath());

    emit statusMessage("set TaskTerminalBinPath directory " + path);
}

QString Router::defaultTaskRepositoryPath() const
{
    return m_repository->defaultTaskRepositoryPath();
}

void Router::setDefaultTaskRepositoryPath(const QString path)
{
    m_repository->setDefaultTaskRepositoryPath(path);

    m_adapter->setDefaultPath(m_repository->defaultTaskRepositoryPath());

    emit statusMessage("set DefaultTaskRepository directory " + path);
}

QStringList Router::readStatuses()
{
    emit statusMessage("read statuses");
    return m_repository->readStatuses();
}

QStringList Router::readTags()
{
    emit statusMessage("read tags");
    return m_repository->readTags();
}

QStringList Router::readUsers()
{
    emit statusMessage("read users");
    return m_repository->readUsers();
}

