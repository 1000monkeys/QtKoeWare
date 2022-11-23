#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>

#include "ui_AddDose.h"

#include "Screen.h"
#include "SelectPatientDialog.h"
#include "SelectTableDialog.h"
#include "SelectDateDialog.h"

#include "../Classes/General.cpp"

QT_BEGIN_NAMESPACE
class AddDoseUI : public QMainWindow, public Screen
{
	Q_OBJECT

public:
	AddDoseUI(QMainWindow* parent = nullptr);
	void resetInputs();

private:
	Ui_AddDoseUIFile addDoseUI;

	DataBase* db = new DataBase;
	std::map<std::string, std::string> settings;

	void closeEvent(QCloseEvent* event);

	void ShowBatchDialog();
	void ShowPatientDialog();
	void ShowDateDialog();

	checkReturnData batchCheck();
	checkReturnData patientCheck();
	checkReturnData radioActivityCheck();

	void RandomizeRadioActivity();

	void submitDoseData();
};
QT_END_NAMESPACE