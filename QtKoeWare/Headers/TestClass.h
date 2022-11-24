#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>
#include "Screen.h"
#include "DataBase.h"
#include "ui_testUI.h"
#include "../Headers/SimRun.h"
#include "../Headers/pdf.h"

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

	void closeEvent(QCloseEvent* event);

	void Test1();
	void Test2();
	void Test3();
	void createPdf();
};
QT_END_NAMESPACE