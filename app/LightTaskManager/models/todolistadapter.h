#ifndef TODOLISTADAPTER_H
#define TODOLISTADAPTER_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDebug>

class TodolistAdapter : public QObject
{
    Q_OBJECT
public:
    explicit TodolistAdapter(QObject *parent = nullptr);
    ~TodolistAdapter();

    QString currentDirectory() const;

protected:
    QString m_directory;
    QByteArray m_data;

    const QString m_todolistBinPath = "/usr/local/opt/todolist/bin/todolist";
    const QString m_getTasks = "list";
    const QString m_addTask = "add";
    const QString m_deleteTask = "delete";
    const QString m_updateTask = "update";

    QProcess* m_todolistProcess;


signals:
    void dataUpdated(QByteArray data);
    void directoryUpdated(QString path);

public slots:
    void openRepository(QString directory);
    void readData();
};

#endif // TODOLISTADAPTER_H