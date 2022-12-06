#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QPrinter>
#include "DataBase.h"

class Pdf {

public:
	Pdf();
	void createPdf(int batchId, bool sim);

private:
	DataBase* db = new DataBase;

	void okPress();
	void tableOnClick(const QModelIndex& index);
	void setRowId();
	std::wstring ExePath();
};