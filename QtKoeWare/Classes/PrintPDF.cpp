#include "../Headers/PrintPDF.h"
#include <QtSql>
#include "../Headers/pdf.h"


PrintPDF::PrintPDF(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	printPDF.setupUi(this);

	connect(printPDF.searchButton, &QPushButton::released, this, &PrintPDF::search);
	connect(printPDF.printButton, &QPushButton::released, this, &PrintPDF::createPDF);

	connect(printPDF.tableView, &QTableView::clicked, this, &PrintPDF::tableOnClick);
	printPDF.tableView->setSelectionMode(QTableView::SingleSelection);
}

void PrintPDF::search() {
	model = new QSqlQueryModel;
	if (printPDF.simulationsCheckBox->isChecked()) {
		model->setQuery("SELECT * FROM batchessim WHERE batchId LIKE '%" + printPDF.searchLineEdit->text() + "%';");
	}
	else {
		model->setQuery("SELECT * FROM batches WHERE batchId LIKE '%" + printPDF.searchLineEdit->text() + "%';");
	}

	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Molybdeen Batch ID"));
	model->setHeaderData(2, Qt::Horizontal, tr("Date/Time produce"));
	model->setHeaderData(3, Qt::Horizontal, tr("Date/Time measured"));
	model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
	printPDF.tableView->setModel(model);
}

void PrintPDF::tableOnClick(const QModelIndex& notUsed) {
	if (printPDF.tableView->selectionModel()->hasSelection()) {
		QItemSelectionModel* select = printPDF.tableView->selectionModel();

		QModelIndex index = (QModelIndex)select->selectedIndexes().first();
		rowId = std::stoi(model->index(index.row(), 1).data().toString().toStdString());
		printPDF.selectedBatch->setText(QString::number(rowId));
	}
	else {
		rowId = NULL;
		printPDF.selectedBatch->setText("");
	}
}

void PrintPDF::createPDF() {
	if (rowId != NULL) {
		Pdf pdf;
		pdf.createPdf(rowId, true);
	}
}

void PrintPDF::resetInputs() {

}

void PrintPDF::closeEvent(QCloseEvent* event) {
	PrintPDF::Screen::closeEvent(event, this);
}