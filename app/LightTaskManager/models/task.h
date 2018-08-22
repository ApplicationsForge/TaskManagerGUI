#ifndef TASK_H
#define TASK_H

#include <QStringList>
#include <QDate>

class Task
{
public:
    explicit Task(size_t index, QString status, QString title,
                  QDate date = QDate(),
                  QStringList tags = QStringList(),
                  QStringList users = QStringList(),
                  QString description = "");

    size_t index() const;

    QString status() const;
    void setStatus(const QString &status);

    QString title() const;
    void setTitle(const QString &title);

    QDate date() const;
    void setDate(const QDate &date);

    QStringList tags() const;
    void setTags(const QStringList &tags);

    QStringList users() const;
    void setUsers(const QStringList &users);

    QString description() const;
    void setDescription(const QString &description);

protected:
    size_t m_index;
    QString m_status;
    QString m_title;
    QDate m_date;
    QStringList m_tags;
    QStringList m_users;
    QString m_description;
};

#endif // TASK_H
