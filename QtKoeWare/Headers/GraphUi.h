#pragma once
#include <QtWidgets/QMainWindow>
#include <QtUiTools>
#include "ui_GraphUI.h"
#include "Screen.h"
#include "DataBase.h"
#include "CalcRadioActivity.h"
#include <math.h>

QT_BEGIN_NAMESPACE
class GraphUI : public QMainWindow, public Screen
{
	Q_OBJECT

public:
	GraphUI(QMainWindow* parent = nullptr);
	void resetInputs();
	void closeEvent(QCloseEvent* event);
	void selectMoBatch();
	void selectTeBatch();
	void setGraph(int moBatchId = 0, int teBatchId = 0, bool simGraph = false);
	QDateTime getTimeNow(bool simGraph = false);

private:
	Ui_GraphUiFile graphui;
	DataBase* db = new DataBase;
	RadioActivityCalculator* ra = new RadioActivityCalculator;
};
QT_END_NAMESPACE