#include "../Headers/PrintPDF.h"
#include <QtSql>


PrintPDF::PrintPDF(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	printPDF.setupUi(this);

	connect(printPDF.searchButton, &QPushButton::released, this, &PrintPDF::search);
}

void PrintPDF::search() {
	QSqlQueryModel* model = new QSqlQueryModel;
	model->setQuery("SELECT * FROM batches WHERE batchId LIKE '%" + printPDF.searchLineEdit->text() + "%';");
	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Molybdeen Batch ID"));
	model->setHeaderData(2, Qt::Horizontal, tr("Date/Time produce"));
	model->setHeaderData(3, Qt::Horizontal, tr("Date/Time measured"));
	model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
	printPDF.tableView->setModel(model);
}

void PrintPDF::resetInputs() {

}

void PrintPDF::closeEvent(QCloseEvent* event) {
	PrintPDF::Screen::closeEvent(event, this);
}