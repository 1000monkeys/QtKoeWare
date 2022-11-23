#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>
#include "Screen.h"
#include "DataBase.h"
#include "ui_SimUi.h"
#include "SimRun.h"

QT_BEGIN_NAMESPACE
class SimulationUI : public QMainWindow, public Screen
{
	Q_OBJECT
public:
	SimulationUI(QMainWindow* parent = nullptr);
	void resetInputs();

private:
	Ui_SimulationScreen simUI;
	SimRun* sim = new SimRun;
	DataBase* db = new DataBase;

	void closeEvent(QCloseEvent* event);

	void StartSimul();
};
QT_END_NAMESPACE