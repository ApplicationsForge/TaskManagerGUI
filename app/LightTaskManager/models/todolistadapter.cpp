#include "todolistadapter.h"

TodolistAdapter::TodolistAdapter(QString todolistBinPath, QObject *parent) :
    QObject(parent),
    m_todolistBinPath(todolistBinPath),
    m_directory(""),
    m_todolistProcess(new QProcess(this))
{
    //connect(m_todolistProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onMessage()));
    connect(m_todolistProcess.data(), SIGNAL(finished(int)), this, SLOT(onMessage()));
}

TodolistAdapter::~TodolistAdapter()
{

}

QString TodolistAdapter::currentDirectory() const
{
    return m_directory;
}

QString TodolistAdapter::currentTodoListBinPath() const
{
    return m_todolistBinPath;
}

void TodolistAdapter::initializeRepository(QString directory)
{
    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_initializeRepository;
    qDebug() << "initialize" << m_directory << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::openRepository(QString directory)
{
    m_directory = directory;
    m_todolistProcess->setWorkingDirectory(m_directory);

    QString args = m_todolistBinPath + " " + m_getTasks;
    qDebug() << "open" << m_directory << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::addTask(QString text)
{
    QString args = m_todolistBinPath + " " + m_addTask + " " + text;
    qDebug() << "add task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::changeTaskStatus(size_t index, QString status)
{
    QString args = m_todolistBinPath + " " + m_changeTaskStatus + " " + QString::number(index) + " " + status;
    qDebug() << "change task status" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::deleteTask(size_t index)
{
    QString args = m_todolistBinPath + " " + m_deleteTask + " " + QString::number(index);
    qDebug() << "delete task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::editTask(size_t index, QString text)
{
    QString args = m_todolistBinPath + " " + m_editTask + " " + QString::number(index) + " " + text;
    qDebug() << "edit task" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::archiveByStatus(QString text)
{
    QString args = m_todolistBinPath + " " + m_archiveByStatus + " " + text;
    qDebug() << "tasks archived by status" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::garbageCollection()
{
    QString args = m_todolistBinPath + " " + m_garbageCollection;
    qDebug() << "deleted all archived tasks" << args;
    m_todolistProcess->start(args);
}

void TodolistAdapter::onMessage()
{
    QByteArray message = m_todolistProcess->readAllStandardOutput();
    //qDebug() << "read message" << QString::fromUtf8(message);
    emit directoryUpdated(m_directory);
    emit tasksUpdated(message);
}

void TodolistAdapter::setBinPath(QString todolistPath)
{
    m_todolistBinPath.clear();
    m_todolistBinPath = todolistPath;
}

void TodolistAdapter::runCommand(QString command)
{
    QString args = m_todolistBinPath + " " + command;
    qDebug() << "run command:" << args;
    m_todolistProcess->start(args);
}
