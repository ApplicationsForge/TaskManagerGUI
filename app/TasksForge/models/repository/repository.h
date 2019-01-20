#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QDir>

#include "models/types/task_terminal_adapter/task_terminal_adapter.h"
#include "models/types/settings_manager/settings_manager.h"
#include "models/types/task/task.h"

class Router;

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QObject *parent = nullptr);

    /// setup filters
    void setTagFilter(const QString &tagFilter);
    void setUserFilter(const QString &userFilter);

    void setTaskTerminalBinPath(const QString path);
    QString taskTerminalBinPath() const;

    QString defaultTaskRepositoryPath() const;
    void setDefaultTaskRepositoryPath(const QString path);

    QList<Task> tasks() const;

    QStringList readStatuses();
    QStringList readTags();
    QStringList readUsers();

    void setTagsCount(unsigned int count);
    void addTag(QString key, QString value);

    void setUsersCount(unsigned int count);
    void addUser(QString key, QString value);

    void setStatusesCount(unsigned int count);
    void addStatus(QString key, QString value);

private:
    QScopedPointer<SettingsManager> m_settingsManager;
    QString m_taskTerminalBinPath;
    QString m_defaultTaskRepositoryPath;

    QList<Task> m_tasks;

    QString m_tagFilter;
    QString m_userFilter;

    friend class Router;

signals:

public slots:

};

#endif // REPOSITORY_H
