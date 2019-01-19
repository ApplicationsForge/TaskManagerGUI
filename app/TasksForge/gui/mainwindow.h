#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QLayout>

#include "models/taskmanager.h"
#include "gui/widgets/my_list_widget.h"
#include "gui/widgets/my_list_widget_item.h"

#include "add_dialog.h"
#include "delete_task_dialog.h"
#include "settings_dialog.h"
#include "archive_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDirectoryWidgets(QString filePath);

    void onTaskManager_TasksUpdated();

    void updateTaskWidgets();

    void showTask(QModelIndex index);

    void enableTasksActions();

    void showStatusMessage(QString message);

    void on_actionOpenRepository_triggered();

    void on_actionInitializeRepository_triggered();

    void changeTaskStatusAction(QString data);

    void on_actionAddTask_triggered();

    void on_actionDeleteTask_triggered();

    void on_editTaskPushButton_clicked();

    void on_saveTaskPushButton_clicked();

    void on_actionSettings_triggered();

    void on_acceptFiltersPushButton_clicked();

    void on_commandLineLineEdit_returnPressed();

    void on_actionArchive_Task_By_Status_triggered();

private:
    Ui::MainWindow *ui;
    QScopedPointer<TaskManager> m_taskManager;

    QList< QPair<QLabel*, MyListWidget*> > m_taskLists;

    //QList< QLabel* > m_statusesLabels;
    //QList< MyListWidget* > m_tasksLists;

    void setupWidgets();
    void setupModel();

    void updateTaskLists();
};

#endif // MAINWINDOW_H
