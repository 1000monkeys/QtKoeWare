#include <QtWidgets/QMainWindow>
#include <QtUiTools>

#include "ui_Settings.h"

#include "Screen.h"
#include "DataBase.h"

QT_BEGIN_NAMESPACE
class SettingsUI : public QMainWindow, public Screen
{

public:
	SettingsUI(QMainWindow* parent = nullptr);

private:
	Ui_SettingsUIFile settingsUI;

	DataBase* db = new DataBase;
	std::map<std::string, std::string> settings;

	void resetInputs();
	void submitSettings();
	void defaultSettings();
	void setInputs();
};
QT_END_NAMESPACE