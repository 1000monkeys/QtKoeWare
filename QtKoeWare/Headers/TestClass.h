#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>
#include "Screen.h"
#include "DataBase.h"
#include "ui_testUI.h"
#include "../Headers/SimRun.h"


QT_BEGIN_NAMESPACE
class TestUI : public QMainWindow, public Screen
{
	Q_OBJECT

public:
	TestUI(QMainWindow* parent = nullptr);

	void resetInputs();

private:
	Ui_testUI testUI;
	SimRun* sim = new SimRun;
	DataBase* db = new DataBase;
	double setXVal(int i, int xmax, int plotpoints);
	void closeEvent(QCloseEvent* event);
	void Test1();
	void Test2();
	void Test3();
};
QT_END_NAMESPACE