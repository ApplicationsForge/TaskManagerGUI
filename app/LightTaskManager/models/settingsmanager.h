#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <memory>

#define DEFAULT_SETTINGS_FILE "Settings.ini"

class SettingsManager
{
protected:
    std::shared_ptr<QSettings> settings;

public:
    SettingsManager();
    SettingsManager(QString settingsPath);

    ~SettingsManager();

    void saveSettings();

    void exportSettings(QString path);

    void importSettings(QString settingsPath);

    QVariant get(QString group, QString key) const;

    void set(QString group, QString key, QVariant value);

    void generateDefaultSettings();

    QStringList settingsKeys();
};
#endif // SETTINGSMANAGER_H
