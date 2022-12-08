#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QPrinter>
#include "DataBase.h"
#include <qprogressbar.h>

class Pdf {

public:
	Pdf();
	void createPdf(QProgressBar* bar, int batchId, bool sim);

private:
	DataBase* db = new DataBase;

	void okPress();
	void tableOnClick(const QModelIndex& index);
	void setRowId();
	std::wstring ExePath();
};