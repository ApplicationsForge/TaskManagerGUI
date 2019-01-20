
/*QStringList TaskManager::filterByTagName(QStringList allTasks)
{
    QString tagTemplate = QString(QString("+") + m_tagFilter);
    QStringList filteredTasks;
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == tagTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}

QStringList TaskManager::filterByUserName(QStringList allTasks)
{
    QStringList filteredTasks;
    QString userTemplate = QString(QString("@") + m_userFilter);
    for(auto task : allTasks)
    {
        QStringList splittedTask = task.split(" ");
        for(auto word : splittedTask)
        {
            if(word == userTemplate)
            {
                filteredTasks.push_back(task);
            }
        }
    }
    return filteredTasks;
}*/
