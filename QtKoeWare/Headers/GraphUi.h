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
	void setMolybdenumGraph(int moBatchId = 0, bool simGraph = false, bool latestBatch = false, bool print = false);
	void setTechnetiumGraph(int teBatch = 0, bool simGraph = false, bool print = false);
	void setGraphTitle(QString titleText);
	void printGraphImage(QString title);
	QDateTime getTimeNow(bool simGraph = false);

private:
	Ui_GraphUiFile graphui;
	DataBase* db = new DataBase;
	RadioActivityCalculator* ra = new RadioActivityCalculator;
};
QT_END_NAMESPACE