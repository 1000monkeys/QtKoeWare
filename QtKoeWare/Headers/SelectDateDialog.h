#pragma once
//#include "ui_DateDialog.h"
#include <Qdialog>
#include <QMainWindow>
#include <QDate>

#include "ui_SelectDateDialog.h"

QT_BEGIN_NAMESPACE
class SelectDateDialog : public QDialog {
	Q_OBJECT

public:
	SelectDateDialog(QWidget* parent = nullptr);
	QDate getDate();
	void setDate(QDate date);

private:
	Ui_DateDialogFile dateDialog_Ui;
};
QT_END_NAMESPACE