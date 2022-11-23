#include "../Headers/SettingsUI.h"

SettingsUI::SettingsUI(QMainWindow* parent):QMainWindow(parent), Screen()
{
	settingsUI.setupUi(this);

	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);

	connect(settingsUI.submitButton, &QPushButton::released, this, &SettingsUI::submitSettings);
    connect(settingsUI.defaultButton, &QPushButton::released, this, &SettingsUI::defaultSettings);

    settings = db->getSettings();
    setInputs();
}

void SettingsUI::defaultSettings() {
    settings = db->getDefaultSettings();
    setInputs();
}

void SettingsUI::setInputs() {
    settingsUI.min_te_radioactivity->setText(QString::fromStdString(settings["min_te_radioactivity"]));
    settingsUI.max_te_radioactivity->setText(QString::fromStdString(settings["max_te_radioactivity"]));

    settingsUI.min_name_length->setText(QString::fromStdString(settings["min_name_length"]));
    settingsUI.short_name_length->setText(QString::fromStdString(settings["short_name_length"]));
    settingsUI.really_long_name_length->setText(QString::fromStdString(settings["really_long_name_length"]));

    settingsUI.min_height->setText(QString::fromStdString(settings["min_height"]));
    settingsUI.really_really_short_height->setText(QString::fromStdString(settings["really_really_short_height"]));
    settingsUI.really_short_height->setText(QString::fromStdString(settings["really_short_height"]));
    settingsUI.really_tall_height->setText(QString::fromStdString(settings["really_tall_height"]));
    settingsUI.really_really_tall_height->setText(QString::fromStdString(settings["really_really_tall_height"]));

    settingsUI.min_weight->setText(QString::fromStdString(settings["min_weight"]));
    settingsUI.really_really_light_weigth->setText(QString::fromStdString(settings["really_really_light_weigth"]));
    settingsUI.really_light_weigth->setText(QString::fromStdString(settings["really_light_weigth"]));
    settingsUI.really_heavy_weigth->setText(QString::fromStdString(settings["really_heavy_weigth"]));
    settingsUI.really_really_heavy_weigth->setText(QString::fromStdString(settings["really_really_heavy_weigth"]));

    settingsUI.min_mo_radioactivity->setText(QString::fromStdString(settings["min_mo_radioactivity"]));
    settingsUI.max_mo_radioactivity->setText(QString::fromStdString(settings["max_mo_radioactivity"]));

    settingsUI.min_dose_radioactivity->setText(QString::fromStdString(settings["min_dose_radioactivity"]));
    settingsUI.max_dose_radioactivity->setText(QString::fromStdString(settings["max_dose_radioactivity"]));

    settingsUI.secs_time_offset->setText(QString::fromStdString(settings["secs_time_offset"]));

    settingsUI.min_te_production_time->setText(QString::fromStdString(settings["min_te_production_time"]));
    settingsUI.max_te_production_time->setText(QString::fromStdString(settings["max_te_production_time"]));

    settingsUI.min_mo_production_time->setText(QString::fromStdString(settings["min_mo_production_time"]));
    settingsUI.max_mo_production_time->setText(QString::fromStdString(settings["max_mo_production_time"]));
}

void SettingsUI::submitSettings()
{
	std::map<std::string, std::string> settings;

    settings["min_te_radioactivity"] = settingsUI.min_te_radioactivity->text().toStdString();
    settings["max_te_radioactivity"] = settingsUI.max_te_radioactivity->text().toStdString();

    settings["min_name_length"] = settingsUI.min_name_length->text().toStdString();
    settings["short_name_length"] = settingsUI.short_name_length->text().toStdString();
    settings["really_long_name_length"] = settingsUI.really_long_name_length->text().toStdString();

    settings["min_height"] = settingsUI.min_height->text().toStdString();
    settings["really_really_short_height"] = settingsUI.really_really_short_height->text().toStdString();
    settings["really_short_height"] = settingsUI.really_short_height->text().toStdString();
    settings["really_tall_height"] = settingsUI.really_tall_height->text().toStdString();
    settings["really_really_tall_height"] = settingsUI.really_really_tall_height->text().toStdString();

    settings["min_weight"] = settingsUI.min_weight->text().toStdString();
    settings["really_really_light_weigth"] = settingsUI.really_really_light_weigth->text().toStdString();
    settings["really_light_weigth"] = settingsUI.really_light_weigth->text().toStdString();
    settings["really_heavy_weigth"] = settingsUI.really_heavy_weigth->text().toStdString();
    settings["really_really_heavy_weigth"] = settingsUI.really_really_heavy_weigth->text().toStdString();

    settings["min_mo_radioactivity"] = settingsUI.min_mo_radioactivity->text().toStdString();
    settings["max_mo_radioactivity"] = settingsUI.max_mo_radioactivity->text().toStdString();

    settings["min_dose_radioactivity"] = settingsUI.min_dose_radioactivity->text().toStdString();
    settings["max_dose_radioactivity"] = settingsUI.max_dose_radioactivity->text().toStdString();

    settings["secs_time_offset"] = settingsUI.secs_time_offset->text().toStdString();

    settings["min_te_production_time"] = settingsUI.min_te_production_time->text().toStdString();
    settings["max_te_production_time"] = settingsUI.max_te_production_time->text().toStdString();

    settings["min_mo_production_time"] = settingsUI.min_mo_production_time->text().toStdString();
    settings["max_mo_production_time"] = settingsUI.max_mo_production_time->text().toStdString();

    for (const auto& [k, v] : settings) {
        QSqlQuery q;
        if (!q.prepare(INSERT_SETTINGS_SQL)) {
            qDebug() << "No settings table found";
            //return q.lastError();
        }
        else {
            //qDebug() << "5c";
        }

        q.addBindValue(QDateTime::currentDateTime());
        q.addBindValue(QString::fromStdString(k));
        q.addBindValue(QString::fromStdString(settings[k]));
        q.exec();
        qDebug() << "INSERTED SETTING " << QString::fromStdString(k);
    }
}

void SettingsUI::resetInputs()
{
}
