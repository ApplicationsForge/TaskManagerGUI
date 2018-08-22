#include "task.h"

Task::Task(size_t index, QString status,
           QString title, QDate date, QStringList tags, QStringList users, QString description) :
    m_index(index),
    m_status(status),
    m_title(title),
    m_date(date),
    m_tags(tags),
    m_users(users),
    m_description(description)
{

}

size_t Task::index() const
{
    return m_index;
}

QString Task::status() const
{
    return m_status;
}

void Task::setStatus(const QString &status)
{
    m_status = status;
}

QString Task::title() const
{
    return m_title;
}

void Task::setTitle(const QString &title)
{
    m_title = title;
}

QDate Task::date() const
{
    return m_date;
}

void Task::setDate(const QDate &date)
{
    m_date = date;
}

QStringList Task::tags() const
{
    return m_tags;
}

void Task::setTags(const QStringList &tags)
{
    m_tags = tags;
}

QStringList Task::users() const
{
    return m_users;
}

void Task::setUsers(const QStringList &users)
{
    m_users = users;
}

QString Task::description() const
{
    return m_description;
}

void Task::setDescription(const QString &description)
{
    m_description = description;
}
