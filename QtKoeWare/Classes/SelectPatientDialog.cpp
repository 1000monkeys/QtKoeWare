#include "../Headers/SelectPatientDialog.h"

SelectPatientDialog::SelectPatientDialog(QWidget* parent) : QDialog(parent)
{
	setMinimumSize(400, 300);
	selectPatientDialog_Ui.setupUi(this);

	model = new QSqlQueryModel(this);

	selectPatientDialog_Ui.tableView->setSelectionMode(QTableView::SingleSelection);
	
	connect(selectPatientDialog_Ui.okButton, &QPushButton::released, this, &SelectPatientDialog::okPress);
	connect(selectPatientDialog_Ui.tableView, &QTableView::clicked, this, &SelectPatientDialog::tableOnClick);
}

void SelectPatientDialog::setItems() {
	model->setQuery("SELECT * FROM patients;");

	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	model->setHeaderData(2, Qt::Horizontal, tr("Length"));
	model->setHeaderData(3, Qt::Horizontal, tr("Weight"));
	model->setHeaderData(4, Qt::Horizontal, tr("Birthdate"));
	model->setHeaderData(5, Qt::Horizontal, tr("Sex"));

	selectPatientDialog_Ui.tableView->setModel(model);
}

int SelectPatientDialog::getPatientId() {
	return patientId;
}

void SelectPatientDialog::tableOnClick(const QModelIndex& notUsed) {
	setPatientIdVar();
}

void SelectPatientDialog::okPress() {
	setPatientIdVar();
}

void SelectPatientDialog::setPatientIdVar() {
	if (selectPatientDialog_Ui.tableView->selectionModel()->hasSelection()) {
		QItemSelectionModel* select = selectPatientDialog_Ui.tableView->selectionModel();

		QModelIndex index = (QModelIndex)select->selectedIndexes().first();
		patientId = std::stoi(model->index(index.row(), 0).data().toString().toStdString());
	}
	else {
		patientId = NULL;
	}
}