#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QPrinter>
#include "DataBase.h"

class Pdf {

public:
	Pdf();
	void createPdf();

private:
	QString filename = "output.pdf";

	DataBase* db = new DataBase;
};