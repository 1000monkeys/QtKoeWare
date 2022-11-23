#include "../Headers/DataBase.h"
#include "../Headers/SelectTableDialog.h"

SelectTableDialog::SelectTableDialog(QWidget* parent) : QDialog(parent) {
	setMinimumSize(800, 300);
	batchIDDialog_Ui.setupUi(this);

	model = new QSqlQueryModel(this);

	batchIDDialog_Ui.tableView->setSelectionMode(QTableView::SingleSelection);

	connect(batchIDDialog_Ui.okButton, &QPushButton::released, this, &SelectTableDialog::okPress);
	connect(batchIDDialog_Ui.tableView, &QTableView::clicked, this, &SelectTableDialog::tableOnClick);
}

void SelectTableDialog::setModel(QSqlQueryModel* model) {
	this->model = model;
	batchIDDialog_Ui.tableView->setModel(model);
}

void SelectTableDialog::tableOnClick(const QModelIndex& notUsed) {
	setRowId();
}

void SelectTableDialog::okPress() {
	setRowId();
}

int SelectTableDialog::getRowId() {
	return rowId;
}

void SelectTableDialog::setRowId() {
	if (batchIDDialog_Ui.tableView->selectionModel()->hasSelection()) {
		QItemSelectionModel* select = batchIDDialog_Ui.tableView->selectionModel();

		QModelIndex index = (QModelIndex)select->selectedIndexes().first();
		rowId = std::stoi(model->index(index.row(), 0).data().toString().toStdString());
	}
	else {
		rowId = NULL;
	}
}