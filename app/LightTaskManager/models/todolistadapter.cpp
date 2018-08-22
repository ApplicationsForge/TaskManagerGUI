#include "todolistadapter.h"

TaskTerminalAdapter::TaskTerminalAdapter(QString taskTerminalBinPath, QObject *parent) :
    QObject(parent),
    m_taskTerminalBinPath(taskTerminalBinPath),
    m_taskTerminalProcess(new QProcess(this)),
    m_directory("")
{
    //connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));
    connect(m_taskTerminalProcess.data(), SIGNAL(finished(int)), this, SLOT(onTaskTerminal_Finished()));
}

TaskTerminalAdapter::~TaskTerminalAdapter()
{

}

QString TaskTerminalAdapter::currentDirectory() const
{
    return m_directory;
}

QString TaskTerminalAdapter::currentTodoListBinPath() const
{
    return m_taskTerminalBinPath;
}

void TaskTerminalAdapter::initializeRepository(QString directory)
{
    m_directory = directory;
    m_taskTerminalProcess->setWorkingDirectory(m_directory);

    QString args = m_taskTerminalBinPath + " " + m_initializeRepositoryCommand;
    qDebug() << "initialize" << m_directory << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::openRepository(QString directory)
{
    m_directory = directory;
    m_taskTerminalProcess->setWorkingDirectory(m_directory);

    QString args = m_taskTerminalBinPath + " " + m_getTaskListComand;
    qDebug() << "open" << m_directory << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::addTask(QString text)
{
    QString args = m_taskTerminalBinPath + " " + m_addTaskCommand + " " + text;
    qDebug() << "add task" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::changeTaskStatus(size_t index, QString status)
{
    QString args = m_taskTerminalBinPath + " " + m_changeTaskStatusCommand + " " + QString::number(index) + " " + status;
    qDebug() << "change task status" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::deleteTask(size_t index)
{
    QString args = m_taskTerminalBinPath + " " + m_deleteTaskComamnd + " " + QString::number(index);
    qDebug() << "delete task" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::editTask(size_t index, QString text)
{
    QString args = m_taskTerminalBinPath + " " + m_editTaskCommand + " " + QString::number(index) + " " + text;
    qDebug() << "edit task" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::lArchived()
{
    QString args = m_taskTerminalBinPath + " " + m_lArchived;
    qDebug() << "l archived" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::archiveByStatus(QString text)
{
    QString args = m_taskTerminalBinPath + " " + m_archiveByStatus + " " + text;
    qDebug() << "tasks archived by status" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::unarchive(size_t index)
{
    QString args = m_taskTerminalBinPath + " " + m_unarchive + " " + QString::number(index);
    qDebug() << "unarchive" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::garbageCollection()
{
    QString args = m_taskTerminalBinPath + " " + m_garbageCollection;
    qDebug() << "deleted all archived tasks" << args;
    m_taskTerminalProcess->start(args);
}

void TaskTerminalAdapter::onTaskTerminal_Finished()
{
    QByteArray message = m_taskTerminalProcess->readAllStandardOutput();
    //qDebug() << "read message" << QString::fromUtf8(message);
    //emit directoryUpdated(m_directory);
    //emit tasksUpdated(parseTaskTerminalOutput(message));
    QStringList test = parseTaskTerminalOutput(message);
    try
    {
        QString testTask = test.first();
        qDebug() << testTask;

        qDebug() << getTaskIndex(testTask);
        qDebug() << getTaskStatus(testTask);
        qDebug() << getTaskTitle(testTask);
        qDebug() << getTaskTags(testTask);
        qDebug() << getTaskUsers(testTask);
        qDebug() << getTaskDate(testTask);
        qDebug() << getTaskDescription(testTask);
    }
    catch(std::invalid_argument e)
    {
        qDebug() << e.what();
    }
}

QStringList TaskTerminalAdapter::parseTaskTerminalOutput(QByteArray data)
{
    QStringList tasks = QStringList();
    QString str = QString::fromUtf8(data);
    if(str.contains("all"))
    {
        str.remove("\t"); //delete tabs
        str.remove("\n all\n");
        tasks = str.split(QRegExp("\n"), QString::SkipEmptyParts);
    }
    return tasks;
}

unsigned int TaskTerminalAdapter::getTaskIndex(QString task)
{
    bool ok = true;
    auto index = task.split("[").first().remove(" ").toUInt(&ok);
    if(ok)
    {
        return index;
    }
    else
    {
        throw std::invalid_argument("can not get task index" + task.toStdString());
    }
}

QString TaskTerminalAdapter::getTaskStatus(QString task)
{
    return task.split("]", QString::SkipEmptyParts).first().split("[", QString::SkipEmptyParts).last();
}

QString TaskTerminalAdapter::getTaskTitle(QString task)
{
    return task.split("#", QString::SkipEmptyParts)[1];
}

QStringList TaskTerminalAdapter::getTaskTags(QString task)
{
    QStringList tags = QStringList();

    QRegExp tagRegExp = QRegExp("\\+[\\p{L}\\d_-]+");
    tagRegExp.setPatternSyntax(QRegExp::PatternSyntax::W3CXmlSchema11);

    int lastPos = 0;
    while( (lastPos = tagRegExp.indexIn(task, lastPos)) != -1)
    {
        lastPos += tagRegExp.matchedLength();
        tags.push_back(tagRegExp.cap(0));
    }

    return tags;
}

QStringList TaskTerminalAdapter::getTaskUsers(QString task)
{
    QStringList tags = QStringList();

    QRegExp tagRegExp = QRegExp("\\@[\\p{L}\\d_]+");
    tagRegExp.setPatternSyntax(QRegExp::PatternSyntax::W3CXmlSchema11);

    int lastPos = 0;
    while( (lastPos = tagRegExp.indexIn(task, lastPos)) != -1)
    {
        lastPos += tagRegExp.matchedLength();
        tags.push_back(tagRegExp.cap(0));
    }

    return tags;
}

QDate TaskTerminalAdapter::getTaskDate(QString task)
{
    QString dateRaw = task.split("until [").last()
            .split("]").first();
    return QDate::fromString(dateRaw);
}

QString TaskTerminalAdapter::getTaskDescription(QString task)
{
    QString index = QString::number(getTaskIndex(task));
    QString status = getTaskStatus(task);
    QString title = getTaskTitle(task);
    QStringList tags = getTaskTags(task);
    QStringList users = getTaskUsers(task);
    QString date = getTaskDate(task).toString();

    task.remove(" " + index);
    task.remove("[" + status + "]");
    task.remove("#" + title + "#");
    task.remove("until [" + date + "]");

    for(auto tag : tags)
    {
        task.remove(tag);
    }

    for(auto user : users)
    {
        task.remove(user);
    }

    task = task.split(" ", QString::SkipEmptyParts).join(" ");

    return task;
}

void TaskTerminalAdapter::setBinPath(QString todolistPath)
{
    m_taskTerminalBinPath.clear();
    m_taskTerminalBinPath = todolistPath;
}

void TaskTerminalAdapter::runCommand(QString command)
{
    QString args = m_taskTerminalBinPath + " " + command;
    qDebug() << "run command:" << args;
    m_taskTerminalProcess->start(args);
}
