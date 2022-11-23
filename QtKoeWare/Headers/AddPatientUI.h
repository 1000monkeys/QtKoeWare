#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>

#include "ui_AddPatient.h"
#include "Screen.h"
#include "DataBase.h"
#include "SelectTableDialog.h"
#include "SelectDateDialog.h"

#include "../Classes/General.cpp"

QT_BEGIN_NAMESPACE
class AddPatientUI : public QMainWindow, public Screen
{
	Q_OBJECT

public:
	AddPatientUI(QMainWindow* parent = nullptr);
	void resetInputs();

private:
	Ui_AddPatientUIFile patientUI;
	
	DataBase* db = new DataBase;
	std::map<std::string, std::string> settings;

	void closeEvent(QCloseEvent* event);

	void ShowDateDialog();

	checkReturnData nameCheck();
	checkReturnData lengthCheck();
	checkReturnData weigthCheck();
	checkReturnData birthdateCheck();
	checkReturnData sexCheck();

	void SubmitPatientData();

	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();
};
QT_END_NAMESPACE