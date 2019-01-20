#include "add_dialog.h"
#include "ui_add_dialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    m_date(QDate())
{
    ui->setupUi(this);
    setup();

    //QDate date = ui->calendarWidget->selectedDate();
    //date.setDate(2018, 06, 15);
    //ui->calendarWidget->setSelectedDate(date);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setup()
{
    Router& router = Router::getInstance();

    ui->tagComboBox->clear();
    m_tags = router.readTags();
    ui->tagComboBox->addItems(m_tags);

    ui->userComboBox->clear();
    m_users = router.readUsers();
    ui->userComboBox->addItems(m_users);
}

void AddDialog::on_addButtonBox_accepted()
{
    Router& router = Router::getInstance();

    if(ui->addTitleLineEdit->text() == nullptr)
    {
        QMessageBox(QMessageBox::Information, "Information", "Введите заголовок задачи").exec();
    }
    else
    {
        QString desctiption = QStringLiteral("#") + ui->addTitleLineEdit->text() + QStringLiteral("# ") +
                ui->addDescriptionTextEdit->toPlainText();

        QString tags = "";
        QStringList currentTags = ui->activeTagsLineEdit->text().split(" ", QString::SkipEmptyParts);
        QStringList supportedTags = router.readTags();
        for(auto currentTag : currentTags)
        {
            QString currentTagData = currentTag;
            if(!(currentTagData.contains("+")))
            {
                currentTag = QStringLiteral("+") + currentTag;
            }
            else
            {
                currentTagData = currentTagData.remove("+");
            }

            bool contains = false;
            for(auto supportedTagData : supportedTags)
            {
                if(supportedTagData == currentTagData)
                {
                    contains = true;
                    break;
                }
            }

            if(!contains)
            {
                addNewTag(currentTagData);
            }

            tags += currentTag + " ";
        }


        QString users = "";
        QStringList currentUsers = ui->activeUsersLineEdit->text().split(" ", QString::SkipEmptyParts);
        QStringList supportedUsers = router.readUsers();
        for(auto currentUser : currentUsers)
        {
            QString currentUserData = currentUser;
            if(!(currentUserData.contains("@")))
            {
                currentUser = QStringLiteral("@") + currentUser;
            }
            else
            {
                currentUserData = currentUserData.remove("@");
            }

            bool contains = false;
            for(auto supportedUserData : supportedUsers)
            {
                if(supportedUserData == currentUserData)
                {
                    contains = true;
                    break;
                }
            }

            if(!contains)
            {
                addNewUser(currentUserData);
            }

            users += currentUser + " ";
        }


        QString date = "until [" + m_date.toString() +"]";

        QString task = desctiption + " " + tags + users + " " + date;
        qDebug() << "new task =" << task;
        emit addTask(task);
    }
}

void AddDialog::on_calendarWidget_clicked(const QDate &date)
{
    m_date = date;
}

void AddDialog::on_addTagPushButton_clicked()
{
    QString tag = ui->tagComboBox->currentText();
    QString currentTags = ui->activeTagsLineEdit->text();
    ui->activeTagsLineEdit->setText(currentTags + QStringLiteral(" +") + tag);
}

void AddDialog::on_addUserPushButton_clicked()
{
    QString user = ui->userComboBox->currentText();
    QString currentUsers = ui->activeUsersLineEdit->text();
    ui->activeUsersLineEdit->setText(currentUsers + QStringLiteral(" @") + user);
}

void AddDialog::addNewTag(QString tag)
{
    Router& router = Router::getInstance();
    unsigned int tagsCount = (unsigned int) m_tags.size() + 1;
    router.getRepository()->setTagsCount(tagsCount);
    QString newTagName = QStringLiteral("Tag") + QString::number(tagsCount - 1);
    router.getRepository()->addTag(newTagName, tag);
}

void AddDialog::addNewUser(QString user)
{
    Router& router = Router::getInstance();
    unsigned int usersCount = (unsigned int) m_users.size() + 1;
    router.getRepository()->setUsersCount(usersCount);
    QString newUserName = QStringLiteral("User") + QString::number(usersCount - 1);
    router.getRepository()->addUser(newUserName, user);
}
