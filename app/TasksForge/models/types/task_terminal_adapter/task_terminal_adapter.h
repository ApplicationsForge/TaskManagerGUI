#ifndef TASKTERMINALADAPTER_H
#define TASKTERMINALADAPTER_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QFile>

#include "models/types/task/task.h"

/**
 * @brief Класс для взаимодействия с TaskTerminal
 */
class TaskTerminalAdapter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса
     * @param путь до исполняемого файла TaskTerminal
     * @param parent родительский объект
     */
    explicit TaskTerminalAdapter(QString taskTerminalBinPath, QString defaultTasksPath, QObject *parent = nullptr);

    /**
     * @brief Деструктор класса
     */
    ~TaskTerminalAdapter();

    /**
     * @brief Возвращает текущую директорию
     * @return путь до текущей директории
     */
    QString currentDirectory() const;

    /**
     * @brief Возвращает установленный путь до исполняемого файла TaskTerminal
     * @return путь до исполняемого файла TaskTerminal
     */
    QString currentTodoListBinPath() const;

    QString getDefaultPath() const;
    void setDefaultPath(const QString &defaultPath);

protected:
    /// Путь до исполняемого файла TaskTerminal
    QString m_taskTerminalBinPath = "";

    /// Процесс TaskTerminal
    QScopedPointer<QProcess> m_taskTerminalProcess;

    /// Путь до текущей директории
    QString m_directory;
    QByteArray m_data;

    QString m_defaultPath;

    /// Команда для инициализации репозитория
    const QString m_initializeRepositoryCommand = "init";

    /// Команда для получения списка задач
    const QString m_getTaskListComand = "list";

    /// Команда для добавления задачи
    const QString m_addTaskCommand = "add";

    /// Команда для удаления задачи
    const QString m_deleteTaskComamnd = "delete";

    /// Команда для обновления параметров задачи
    const QString m_editTaskCommand = "edit";

    /// Команда для смены статуса задачи
    const QString m_changeTaskStatusCommand = "cs";

    // отрефакторить
    const QString m_lArchived = "l archived";
    const QString m_archiveByStatus = "as";
    const QString m_unarchive = "uar";
    const QString m_garbageCollection = "gc";

    QList<Task> parseRawTasks(QByteArray data);

    static unsigned int getTaskIndex(QString task);
    static QString getTaskStatus(QString task);
    static QString getTaskTitle(QString task);
    static QStringList getTaskTags(QString task);
    static QStringList getTaskUsers(QString task);
    static QDate getTaskDate(QString task);
    static QString getTaskDescription(QString task);
signals:
    /**
     * @brief Сигнал об обновлении списка задач
     * @param Список задач
     */
    void tasksUpdated(QList<Task> tasks);

    void lArchive(QByteArray tasks);

    /**
     * @brief Сигнал об обновлении текущей директории
     * @param path
     */
    void directoryUpdated(QString path);

    /**
     * @brief Сигнал о получении нового сообщения от TaskTerminal
     * @param message
     */
    void newMessage(QByteArray message);

public slots:

    /**
     * @brief Инициализирует новый репозиторий
     * @param путь к директории для инициализации
     */
    void initializeRepository(QString directory);

    /**
     * @brief Открывает существоующий репозиторий
     * @param путь до репозитория
     */
    void openRepository(QString directory);

    void addTask(QString text);
    void changeTaskStatus(size_t index, QString status);
    void deleteTask(size_t index);
    void editTask(size_t index, QString text);
    void lArchived();
    void archiveByStatus(QString text);
    void unarchive(size_t index);
    void garbageCollection();

    void onTaskTerminal_Finished();

    void setBinPath(QString todolistPath);

    void runCommand(QString command);
};

#endif // TASKTERMINALADAPTER_H
