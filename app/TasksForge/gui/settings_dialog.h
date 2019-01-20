#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>

#include "models/router.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

signals:
    void applytodoDirectory(QString directory);

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_todolistBinPathToolButton_clicked();

    void on_defaultPathToolButton_clicked();

private:
    Ui::SettingsDialog *ui;

    void setup();
};

#endif // SETTINGSDIALOG_H
