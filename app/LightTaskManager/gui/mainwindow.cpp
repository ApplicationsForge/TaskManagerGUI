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
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_statusesLabels.begin(), m_statusesLabels.end());
    qDeleteAll(m_tasksLists.begin(), m_tasksLists.end());
    delete ui;
}

void MainWindow::setupWidgets()
{
    auto toolbar = ui->mainToolBar;
    //removeToolBar(toolbar);
    //addToolBar(Qt::LeftToolBarArea, toolbar);
    //toolbar->show();

    toolbar->addAction(ui->actionInitializeRepository);
    toolbar->addAction(ui->actionOpenRepository);
    toolbar->addAction(ui->actionAddTask);
    toolbar->addAction(ui->actionDeleteTask);
    toolbar->addAction(ui->actionSettings);
    //toolbar->addAction(ui->actionArchive_Task_By_Status);
    //toolbar->addAction(ui->actionShow_Task);

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
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(setTasks(QStringList)));
    connect(m_taskManager.data(), SIGNAL(dataUpdated(QStringList)), this, SLOT(enableTasksActions()));
    connect(m_taskManager.data(), SIGNAL(statusMessage(QString)), this, SLOT(showStatusMessage(QString)));
}

void MainWindow::updateTaskLists()
{
    for(auto label : m_statusesLabels)
    {
        label->deleteLater();
    }

    for(auto list : m_tasksLists)
    {
        list->deleteLater();
    }

    m_statusesLabels.clear();
    m_tasksLists.clear();

    QStringList statuses = m_taskManager->readStatuses();

    for(auto status : statuses)
    {
        QLabel* statusLabel = new QLabel(ui->tasksContainerWidget);
        QString objectName = status + QStringLiteral("Label");
        statusLabel->setText(status);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setObjectName(objectName);
        m_statusesLabels.push_back(statusLabel);

        MyListWidget* taskList = new MyListWidget(ui->tasksContainerWidget);
        objectName = status + QStringLiteral("ListWidget");
        taskList->setObjectName(objectName);
        taskList->setDragEnabled(true);
        taskList->setDropIndicatorShown(true);
        taskList->setDragDropMode(QAbstractItemView::DragDrop);
        connect(taskList, SIGNAL(dropAction(QString)), this, SLOT(changeTaskStatusAction(QString)));
        connect(taskList, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));
        m_tasksLists.push_back(taskList);
    }

    for(auto label : m_statusesLabels)
    {
        ui->statusesLabelsHorizontalLayout->addWidget(label);
    }

    for(auto list : m_tasksLists)
    {
        ui->tasksContainerHorizontalLayout->addWidget(list);
    }

    updateTaskWidgets();
}

void MainWindow::updateDirectoryWidgets(QString filePath)
{
    ui->filePathLineEdit->setText(filePath + "/tasks.json");
}

void MainWindow::setTasks(QStringList taskList)
{
    m_tasks = taskList;
    updateTaskWidgets();
}

void MainWindow::updateTaskWidgets()
{
    for(auto list : m_tasksLists)
    {
        list->clear();
    }

    ui->currentTaskIndexLineEdit->clear();
    ui->currentTaskTitleLineEdit->clear();
    ui->currentTaskTagsLineEdit->clear();
    ui->currentTaskUsersLineEdit->clear();
    ui->currentTaskDateCalendarWidget->setSelectedDate(QDate::currentDate());
    ui->currentTaskDescriptionPlainTextEdit->clear();

    QList< QStringList > tasksContainers;
    for(auto status : m_statusesLabels)
    {
        QStringList tmp;
        QString statusTemplate = QStringLiteral("[") + status->text() + QStringLiteral("]");
        for(auto item : m_tasks)
        {
            if(item.contains(statusTemplate))
            {
                QString data = item.remove(statusTemplate);
                tmp.push_back(data);
            }
        }
        tasksContainers.push_back(tmp);
    }

    for(size_t i = 0; i < (size_t) m_statusesLabels.size(); i++)
    {
        for(size_t j = 0; j < (size_t) tasksContainers[i].size(); j++)
        {
            QListWidgetItem* item = new QListWidgetItem();
            item->setTextAlignment(Qt::TopLeftCorner);
            item->setFont(QFont("Arial", -1, 10, false));

            QString data = tasksContainers[i][j];
            QString index = m_taskManager->getTaskIndex(data);
            QString title = m_taskManager->getTitle(data);
            QString desctiption = m_taskManager->getDescription(data);
            QString date = m_taskManager->getDate(data);
            QStringList tags = m_taskManager->getTags(data).split(" ", QString::SkipEmptyParts);
            QStringList users = m_taskManager->getUsers(data).split(" ", QString::SkipEmptyParts);

            item->setText(index);

            MyListWidgetItem* taskBoard = new MyListWidgetItem(index, title, desctiption, date, tags, users, m_tasksLists[i]);
            item->setSizeHint(taskBoard->minimumSizeHint());

            m_tasksLists[i]->addItem(item);
            m_tasksLists[i]->setItemWidget(item, taskBoard);
        }
    }

    int size = m_tasksLists[0]->sizeHintForColumn(0);
    for(auto list : m_tasksLists)
    {
        size = qMax(size, list->sizeHintForColumn(0));
    }
    size += 10;

    for(auto list : m_tasksLists)
    {
        list->setMinimumWidth(size);
    }

    for(auto label : m_statusesLabels)
    {
        label->setMinimumWidth(size);
    }
}

void MainWindow::on_actionOpenRepository_triggered()
{
    QString file = QFileDialog::getOpenFileName(0, "Open File", "", "*.json");
    if(!file.isNull())
    {
        QString path = QFileInfo(file).path();
        qDebug() << "Open Dir " << path;
        m_taskManager->openRepository(path);
    }
}

void MainWindow::on_actionInitializeRepository_triggered()
{
    QString path = QFileDialog::getExistingDirectory(0,"Open Directory", "");
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

        for(size_t i = 0; i < (size_t) m_statusesLabels.size(); i++)
        {
            if(m_tasksLists[i] == senderWidget)
            {
                status = m_statusesLabels[i]->text();
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
    connect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setTodolistDirectory(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(applytodoDirectory(QString)), m_taskManager.data(), SLOT(setTodolistDirectory(QString)));
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
    ArchiveByStatusDialog dialog(this);
    connect(&dialog, SIGNAL(archiveByStatus(QString)), m_taskManager.data(), SLOT(archiveByStatus(QString)));
    dialog.exec();
    disconnect(&dialog, SIGNAL(archiveByStatus(QString)), m_taskManager.data(), SLOT(archiveByStatus(QString)));
}
