#pragma once
//#include "ui_DateDialog.h"
#include <Qdialog>
#include <QMainWindow>
#include <QtSql>

#include "ui_SelectPatientDialog.h"

QT_BEGIN_NAMESPACE
class SelectPatientDialog : public QDialog {
	Q_OBJECT

public:
	SelectPatientDialog(QWidget* parent = nullptr);
	void setItems();
	int getPatientId();

private:
	Ui_SelectPatientFile selectPatientDialog_Ui;
	
	QSqlQueryModel* model;
	void okPress();
	void tableOnClick(const QModelIndex& index);

	void setPatientIdVar();
	int patientId = NULL;
};
QT_END_NAMESPACE