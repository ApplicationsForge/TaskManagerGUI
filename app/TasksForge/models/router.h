#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>

#include "models/repository/repository.h"

// Router& router = Router::getInstance();

class Router : public QObject
{
    Q_OBJECT
public:
    static Router& getInstance();
    ~Router();

     Repository* getRepository();

     /// working with adapter
     void executeTaskTerminalCommand(QString command);

     void openRepository(QString directory);
     void openRepository();
     void initializeRepository(QString directory);

     void changeTaskStatus(QString task, QString status);
     void changeTaskStatus(size_t index, QString status);

     QString getTaskTerminalBinPath();

     QString defaultTaskRepositoryPath() const;
     void setDefaultTaskRepositoryPath(const QString path);

     QStringList readStatuses();
     QStringList readTags();
     QStringList readUsers();


private:
    explicit Router(QObject *parent = nullptr);

    /// Репозиторий, хранящий текущее состояние
    QScopedPointer<Repository> m_repository;

    QSharedPointer<TaskTerminalAdapter> m_adapter;

    /**
     * @brief Подключет слоты к сигналам полей класса
     */
    void setupConnections();

    /**
     * @brief Отключает слоты от сигналов полей класса
     */
    void resetConnections();

signals:
    void directoryUpdated(QString directory);
    void taskListUpdated();
    void statusMessage(QString message);

public slots:
    void addTask(QString task);
    void deleteTask(QString index);
    void editTask(QString index, QString task);

    void archiveByStatus(QString status);
    void unarchive(QString index);
    void garbageCollection();

    void setTaskTerminalBinPath(QString path);

private slots:
    void onTaskTerminalAdapter_DirectoryUpdated(QString directory);
    void onTaskTerminalAdapter_TasksUpdated(QList<Task> tasks);
};

#endif // ROUTER_H
