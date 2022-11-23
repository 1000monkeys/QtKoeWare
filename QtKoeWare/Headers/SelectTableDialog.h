#pragma once

#include <Qdialog>
#include <QDateTime>
#include <QtSql>

#include "ui_SelectTableDialog.h"
#include "DataBase.h"

QT_BEGIN_NAMESPACE
class SelectTableDialog : public QDialog {
	Q_OBJECT

public:
	SelectTableDialog(QWidget* parent = nullptr);
	int getRowId();
	void setModel(QSqlQueryModel* model);

private:
	Ui_SelectTableDialogUIFile batchIDDialog_Ui;

	QSqlQueryModel* model;
	void okPress();
	void tableOnClick(const QModelIndex& index);
	void setRowId();

	int rowId = NULL;
};
QT_END_NAMESPACE