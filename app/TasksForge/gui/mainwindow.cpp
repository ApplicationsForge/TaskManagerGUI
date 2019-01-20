#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidgets();
    setupModel();
    this->showMaximized();

    Router& router = Router::getInstance();
    auto defaultRepository = router.getRepository()->defaultTaskRepositoryPath();
    if(!defaultRepository.isEmpty())
    {
        router.openRepository(defaultRepository);
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
    Router& router = Router::getInstance();
    QObject::connect(&router, SIGNAL(directoryUpdated(QString)), this, SLOT(updateDirectoryWidgets(QString)));
    QObject::connect(&router, SIGNAL(taskListUpdated()), this, SLOT(onTaskManager_TasksUpdated()));
    QObject::connect(&router, SIGNAL(statusMessage(QString)), this, SLOT(showStatusMessage(QString)));
}

void MainWindow::updateTaskLists()
{
    for(auto list : m_taskLists)
    {
        list.first->deleteLater();
        list.second->deleteLater();
    }

    m_taskLists.clear();

    Router& router = Router::getInstance();
    QStringList statuses = router.getRepository()->readStatuses();

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

    Router& router = Router::getInstance();
    QList<Task> tasks = router.getRepository()->tasks();

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
        Router& router = Router::getInstance();
        router.openRepository(path);
    }
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(nullptr,"Open Directory", "");
    if(!path.isNull())
    {
        Router& router = Router::getInstance();
        router.initializeRepository(path);
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
    Router& router = Router::getInstance();
    router.changeTaskStatus(index.toULongLong(), status);
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
    Router& router = Router::getInstance();
    AddDialog add(this);
    connect(&add, SIGNAL(addTask(QString)), &router, SLOT(addTask(QString)));
    add.exec();
    disconnect(&add, SIGNAL(addTask(QString)), &router, SLOT(addTask(QString)));
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
    Router& router = Router::getInstance();
    DeleteTaskDialog dialog(this);
    connect(&dialog, SIGNAL(deleteTask(QString)), &router, SLOT(deleteTask(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(deleteTask(QString)), &router, SLOT(deleteTask(QString)));
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

    Router& router = Router::getInstance();
    router.editTask(taskIndex, task);

    ui->currentTaskTitleLineEdit->setReadOnly(true);
    ui->currentTaskDescriptionPlainTextEdit->setReadOnly(true);
    ui->currentTaskDateCalendarWidget->setEnabled(false);
    ui->currentTaskTagsLineEdit->setReadOnly(true);
    ui->currentTaskUsersLineEdit->setReadOnly(true);
    ui->saveTaskPushButton->setEnabled(false);
}

void MainWindow::on_actionSettings_triggered()
{
    Router& router = Router::getInstance();
    SettingsDialog dialog(this);
    connect(&dialog, SIGNAL(applytodoDirectory(QString)), &router, SLOT(setTaskTerminalBinPath(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(applytodoDirectory(QString)), &router, SLOT(setTaskTerminalBinPath(QString)));
    updateTaskLists();
}

void MainWindow::on_acceptFiltersPushButton_clicked()
{
    Router& router = Router::getInstance();
    router.getRepository()->setTagFilter(ui->filterByTagLineEdit->text());
    router.getRepository()->setUserFilter(ui->filterByUserLineEdit->text());
    router.openRepository();
}

void MainWindow::on_commandLineLineEdit_returnPressed()
{
    QString text = ui->commandLineLineEdit->text();
    if(text.length() > 0)
    {
        ui->commandLineLineEdit->clear();
        Router& router = Router::getInstance();
        router.executeTaskTerminalCommand(text);
    }
}

void MainWindow::on_actionArchive_Task_By_Status_triggered()
{
    Router& router = Router::getInstance();
    ArchiveDialog dialog(this);
    connect(&dialog, SIGNAL(archiveByStatus(QString)), &router, SLOT(archiveByStatus(QString)));
    connect(&dialog, SIGNAL(garbageCollection()), &router, SLOT(garbageCollection()));
    connect(&dialog, SIGNAL(unarchive(QString)), &router, SLOT(unarchive(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(archiveByStatus(QString)), &router, SLOT(archiveByStatus(QString)));
    disconnect(&dialog, SIGNAL(garbageCollection()), &router, SLOT(garbageCollection()));
    disconnect(&dialog, SIGNAL(unarchive(QString)), &router, SLOT(unarchive(QString)));
}
