#pragma once


#include <QtWidgets/QMainWindow>
#include <QtUiTools>
#include <math.h>

#include "ui_TechnetiumBatch.h"

#include "Screen.h"
#include "DataBase.h"
#include "SelectTableDialog.h"
#include "SelectDateDialog.h"
#include "../Classes/General.cpp"

QT_BEGIN_NAMESPACE
class TechnetiumBatchUI : public QMainWindow, public Screen
{
	Q_OBJECT

public:
	TechnetiumBatchUI(QMainWindow* parent = nullptr);
	void resetInputs();

private:
	Ui_TechnetiumBatchUIFile technetiumUi;

	void closeEvent(QCloseEvent* event);

	DataBase* db = new DataBase;
	std::map<std::string, std::string> settings;

	void technetiumBatchID();
	void showBatchDialog();
	void showDateDialog();
	void submitBatch();
	
	void randomRadioActivity();
	checkReturnData checkAvailability();
	checkReturnData moBatchLineCheck();
	checkReturnData radioActivityCheck();
	checkReturnData dateEditCheck();
	checkReturnData timeEditCheck();

	bool moBatchIdFlag = false;
	bool teBatchIdFlag = false;

	int tebatchid = 0;
	QDateTime curDT = QDateTime::currentDateTime();
};
QT_END_NAMESPACE