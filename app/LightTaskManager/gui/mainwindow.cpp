#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_taskManager(new TaskManager(this))
{
    ui->setupUi(this);
    setupWidgets();
    setupModel();
    this->showMaximized();

    auto defaultRepository = m_taskManager->getDefaultTasksPath();
    if(!defaultRepository.isEmpty())
    {
        m_taskManager->openRepository(defaultRepository);
    }
}

MainWindow::~MainWindow()
{
    for(auto list : m_taskLists)
    {
        delete list.first;
        delete list.second;
    }

    //qDeleteAll(m_taskLists.begin(), m_taskLists.end());
    delete ui;
}

void MainWindow::setupWidgets()
{
    auto toolbar = ui->mainToolBar;

    toolbar->addAction(ui->actionInitializeRepository);
    toolbar->addAction(ui->actionOpenRepository);
    toolbar->addAction(ui->actionAddTask);
    toolbar->addAction(ui->actionDeleteTask);
    toolbar->addAction(ui->actionSettings);
    //toolbar->addAction(ui->actionArchive_Task_By_Status);

    ui->statusBar->setStyleSheet("background-color:#333; color: #55bb55");
    ui->statusBar->showMessage("Ready");

    ui->actionAddTask->setEnabled(false);
    ui->actionDeleteTask->setEnabled(false);
    ui->editTaskPushButton->setEnabled(false);
    ui->saveTaskPushButton->setEnabled(false);
    ui->acceptFiltersPushButton->setEnabled(false);
    ui->currentTaskTagsLineEdit->setReadOnly(true);
    ui->currentTaskUsersLineEdit->setReadOnly(true);
    ui->currentTaskDateCalendarWidget->setEnabled(false);
    ui->actionArchive_Task_By_Status->setEnabled(false);

    updateTaskLists();
}

void MainWindow::setupModel()
{
    connect(m_taskManager.data(), SIGNAL(directoryUpdated(QString)), this, SLOT(updateDirectoryWidgets(QString)));
    connect(m_taskManager.data(), SIGNAL(taskListUpdated()), this, SLOT(onTaskManager_TasksUpdated()));
    connect(m_taskManager.data(), SIGNAL(statusMessage(QString)), this, SLOT(showStatusMessage(QString)));
}

void MainWindow::updateTaskLists()
{
    for(auto list : m_taskLists)
    {
        list.first->deleteLater();
        list.second->deleteLater();
    }

    m_taskLists.clear();

    QStringList statuses = m_taskManager->readStatuses();

    for(auto status : statuses)
    {
        QLabel* statusLabel = new QLabel(ui->tasksContainerWidget);
        QString objectName = status + QStringLiteral("Label");
        statusLabel->setText(status);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setObjectName(objectName);

        MyListWidget* taskList = new MyListWidget(ui->tasksContainerWidget);
        objectName = status + QStringLiteral("ListWidget");
        taskList->setObjectName(objectName);
        taskList->setDragEnabled(true);
        taskList->setDropIndicatorShown(true);
        taskList->setDragDropMode(QAbstractItemView::DragDrop);
        connect(taskList, SIGNAL(dropAction(QString)), this, SLOT(changeTaskStatusAction(QString)));
        connect(taskList, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));

        m_taskLists.push_back(QPair<QLabel*, MyListWidget*>(statusLabel, taskList));
    }

    for(auto list : m_taskLists)
    {
        ui->statusesLabelsHorizontalLayout->addWidget(list.first);
        ui->tasksContainerHorizontalLayout->addWidget(list.second);
    }

    updateTaskWidgets();
}

void MainWindow::updateDirectoryWidgets(QString filePath)
{
    ui->filePathLineEdit->setText(filePath + "/tasks.json");
}

void MainWindow::onTaskManager_TasksUpdated()
{
    updateTaskWidgets();
    enableTasksActions();
}

void MainWindow::updateTaskWidgets()
{
    for(auto list : m_taskLists)
    {
        list.second->clear();
    }

    ui->currentTaskIndexLineEdit->clear();
    ui->currentTaskTitleLineEdit->clear();
    ui->currentTaskTagsLineEdit->clear();
    ui->currentTaskUsersLineEdit->clear();
    ui->currentTaskDateCalendarWidget->setSelectedDate(QDate::currentDate());
    ui->currentTaskDescriptionPlainTextEdit->clear();

    QList<Task> tasks = m_taskManager->tasks();

    QList< QList<Task> > tasksContainers;
    for(auto list : m_taskLists)
    {
        QList<Task> tmp;
        for(auto item : tasks)
        {
            if(item.status() == list.first->text())
            {
                tmp.push_back(item);
            }
        }
        tasksContainers.push_back(tmp);
    }

    for(int i = 0; i < m_taskLists.size(); i++)
    {
        for(int j = 0; j < tasksContainers[i].size(); j++)
        {
            QListWidgetItem* item = new QListWidgetItem();
            item->setTextAlignment(Qt::TopLeftCorner);
            item->setFont(QFont("Arial", -1, 10, false));

            Task task = tasksContainers[i][j];

            //QString index = m_taskManager->getTaskIndex(data);
            //QString title = m_taskManager->getTitle(data);
            //QString desctiption = m_taskManager->getDescription(data);
            //QString date = m_taskManager->getDate(data);
            //QStringList tags = m_taskManager->getTags(data).split(" ", QString::SkipEmptyParts);
            //QStringList users = m_taskManager->getUsers(data).split(" ", QString::SkipEmptyParts);

            item->setText(QString::number(task.index()));

            MyListWidgetItem* taskBoard = new MyListWidgetItem(QString::number(task.index()),
                                                               task.title(),
                                                               task.description(),
                                                               task.date().toString(),
                                                               task.tags(),
                                                               task.users(),
                                                               m_taskLists[i].second);
            item->setSizeHint(taskBoard->minimumSizeHint());

            m_taskLists[i].second->addItem(item);
            m_taskLists[i].second->setItemWidget(item, taskBoard);
        }
    }

    int size = m_taskLists[0].second->sizeHintForColumn(0);
    for(auto list : m_taskLists)
    {
        size = qMax(size, list.second->sizeHintForColumn(0));
    }
    size += 10;

    for(auto list : m_taskLists)
    {
        list.first->setMinimumWidth(size);
        list.second->setMinimumWidth(size);
    }
}

void MainWindow::on_actionOpenRepository_triggered()
{
    QString file = QFileDialog::getOpenFileName(nullptr, "Open File", "", "*.json");
    if(!file.isNull())
    {
        QString path = QFileInfo(file).path();
        qDebug() << "Open Dir " << path;
        m_taskManager->openRepository(path);
    }
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(nullptr,"Open Directory", "");
    if(!path.isNull())
    {
        m_taskManager->initializeRepository(path);
    }
}

void MainWindow::changeTaskStatusAction(QString data)
{
    // get tasks index
    QString index = data.split(" ", QString::SkipEmptyParts).at(0);

    // get task status
    QString status = "undefined";
    MyListWidget* senderWidget = qobject_cast<MyListWidget *>(sender());
    if(senderWidget)
    {
        for(auto list : m_taskLists)
        {
            if(list.second == senderWidget)
            {
                status = list.first->text();
            }
        }
    }

    // change task status by index
    m_taskManager->changeTaskStatus(index.toULongLong(), status);
}

void MainWindow::showTask(QModelIndex index)
{
    ui->currentTaskIndexLineEdit->clear();
    ui->currentTaskTitleLineEdit->clear();
    ui->currentTaskTagsLineEdit->clear();
    ui->currentTaskUsersLineEdit->clear();
    ui->currentTaskDateCalendarWidget->setSelectedDate(QDate::currentDate());
    ui->currentTaskDescriptionPlainTextEdit->clear();

    MyListWidget* list = qobject_cast<MyListWidget*>(sender());
    if(list)
    {
        MyListWidgetItem* item = qobject_cast<MyListWidgetItem*>(list->indexWidget(index));
        if(item)
        {
            ui->currentTaskIndexLineEdit->setText(item->index());
            ui->currentTaskTitleLineEdit->setText(item->title());
            ui->currentTaskTagsLineEdit->setText(item->tags());



            QDate date = QDate::fromString(item->date());
            ui->currentTaskDateCalendarWidget->setSelectedDate(date);
            ui->currentTaskUsersLineEdit->setText(item->users());
            ui->currentTaskDescriptionPlainTextEdit->setPlainText(item->description());
            ui->editTaskPushButton->setEnabled(true);
        }
        else
        {
            qDebug() << "Can not convert QListWidgetItem to MyListWidgetItem";
        }
    }
    else
    {
        qDebug() << "Not success";
    }
}

void MainWindow::on_actionAddTask_triggered()
{
    AddDialog add(*(m_taskManager.data()), this);
    connect(&add, SIGNAL(addTask(QString)), m_taskManager.data(), SLOT(addTask(QString)));
    add.exec();
    disconnect(&add, SIGNAL(addTask(QString)), m_taskManager.data(), SLOT(addTask(QString)));
}

void MainWindow::enableTasksActions()
{
    ui->actionAddTask->setEnabled(true);
    ui->actionDeleteTask->setEnabled(true);
    ui->acceptFiltersPushButton->setEnabled(true);
    ui->actionArchive_Task_By_Status->setEnabled(true);
}

void MainWindow::showStatusMessage(QString message)
{
    ui->statusBar->showMessage(message, 500);
}

void MainWindow::on_actionDeleteTask_triggered()
{
    DeleteTaskDialog dialog(this);
    connect(&dialog, SIGNAL(deleteTask(QString)), m_taskManager.data(), SLOT(deleteTask(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(deleteTask(QString)), m_taskManager.data(), SLOT(deleteTask(QString)));
}

void MainWindow::on_editTaskPushButton_clicked()
{
    ui->currentTaskTitleLineEdit->setReadOnly(false);
    ui->currentTaskDescriptionPlainTextEdit->setReadOnly(false);
    ui->currentTaskDateCalendarWidget->setEnabled(true);
    ui->currentTaskTagsLineEdit->setReadOnly(false);
    ui->currentTaskUsersLineEdit->setReadOnly(false);
    ui->saveTaskPushButton->setEnabled(true);
}

void MainWindow::on_saveTaskPushButton_clicked()
{
    QString taskIndex = ui->currentTaskIndexLineEdit->text();

    QString taskTags = ui->currentTaskTagsLineEdit->text();

    QString taskUsers = ui->currentTaskUsersLineEdit->text();

    QString taskDate = QStringLiteral("until [") + ui->currentTaskDateCalendarWidget->selectedDate().toString() + QStringLiteral("]");

    QString taskTitle = QStringLiteral("#") + ui->currentTaskTitleLineEdit->text() + QStringLiteral("#");
    QString taskDescription = ui->currentTaskDescriptionPlainTextEdit->toPlainText();

    QString task = taskTitle + QStringLiteral(" ") + taskDescription + QStringLiteral(" ") + taskTags + QStringLiteral(" ") + taskUsers + QStringLiteral(" ") + taskDate;

    m_taskManager->editTask(taskIndex, task);

    ui->currentTaskTitleLineEdit->setReadOnly(true);
    ui->currentTaskDescriptionPlainTextEdit->setReadOnly(true);
    ui->currentTaskDateCalendarWidget->setEnabled(false);
    ui->currentTaskTagsLineEdit->setReadOnly(true);
    ui->currentTaskUsersLineEdit->setReadOnly(true);
    ui->saveTaskPushButton->setEnabled(false);
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dialog(m_taskManager->getSettingsManager(), this);
    connect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setWorkingDirectory(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setWorkingDirectory(QString)));
    updateTaskLists();
}

void MainWindow::on_acceptFiltersPushButton_clicked()
{
    m_taskManager->setTagFilter(ui->filterByTagLineEdit->text());
    m_taskManager->setUserFilter(ui->filterByUserLineEdit->text());
    m_taskManager->reopenRepository();
}

void MainWindow::on_commandLineLineEdit_returnPressed()
{
    QString text = ui->commandLineLineEdit->text();
    if(text.length() > 0)
    {
        ui->commandLineLineEdit->clear();
        m_taskManager->runCommand(text);
    }
}

void MainWindow::on_actionArchive_Task_By_Status_triggered()
{
    ArchiveDialog dialog(this);
    connect(&dialog, SIGNAL(archiveByStatus(QString)), m_taskManager.data(), SLOT(archiveByStatus(QString)));
    connect(&dialog, SIGNAL(garbageCollection()), m_taskManager.data(), SLOT(garbageCollection()));
    connect(&dialog, SIGNAL(unarchive(QString)), m_taskManager.data(), SLOT(unarchive(QString)));
    connect(&dialog, SIGNAL(lArchived()), m_taskManager.data(), SLOT(lArchived()));
    dialog.exec();
    disconnect(&dialog, SIGNAL(archiveByStatus(QString)), m_taskManager.data(), SLOT(archiveByStatus(QString)));
    disconnect(&dialog, SIGNAL(garbageCollection()), m_taskManager.data(), SLOT(garbageCollection()));
    disconnect(&dialog, SIGNAL(unarchive(QString)), m_taskManager.data(), SLOT(unarchive(QString)));
    disconnect(&dialog, SIGNAL(lArchived()), m_taskManager.data(), SLOT(lArchived()));
}
