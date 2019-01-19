#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QDir>

#include "models/types/task_terminal_adapter/task_terminal_adapter.h"
#include "models/types/settings_manager/settings_manager.h"
#include "models/types/task/task.h"

class TaskManager : public QObject
{
    Q_OBJECT

public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager();

    /// setup filters
    void setTagFilter(const QString &tagFilter);
    void setUserFilter(const QString &userFilter);

protected:
    QScopedPointer<SettingsManager> m_settingsManager;
    QSharedPointer<TaskTerminalAdapter> m_adapter;

    QList<Task> m_tasks;

    QString m_tagFilter;
    QString m_userFilter;

signals:
    void directoryUpdated(QString directory);
    void taskListUpdated();
    void statusMessage(QString message);

public slots:

    /// working with adapter
    void runCommand(QString command);

    /// working with repositories
    void openRepository(QString directory);
    void initializeRepository(QString directory);
    void reopenRepository();

    /// update current directory
    void onCurrentDirectoryChanged(QString directory);

    /// handle todolist events
    void onTaskTerminalAdapter_TasksUpdated(QList<Task> tasks);

    /// working with tasks
    void changeTaskStatus(QString data, QString status);
    void changeTaskStatus(size_t index, QString status);
    void addTask(QString task);
    void deleteTask(QString index);
    void editTask(QString index, QString task);
    void lArchived();
    void archiveByStatus(QString status);
    void unarchive(QString index);
    void garbageCollection();

    /// get task information
    QList<Task> tasks();

    void setWorkingDirectory(QString directory);
    QString getWorkingDirectory();

    void setDefaultTasksPath(QString path);
    QString getDefaultTasksPath();

    /// read information from config file
    QStringList readStatuses();
    QStringList readTags();
    QStringList readUsers();

    SettingsManager& getSettingsManager();

protected slots:

    /// filter tasks
    QStringList filterByTagName(QStringList allTasks);
    QStringList filterByUserName(QStringList allTasks);
};

#endif // TASKMANAGER_H
