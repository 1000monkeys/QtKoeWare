#include "../Headers/PrintPDF.h"


PrintPDF::PrintPDF(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	printPDF.setupUi(this);
}

void PrintPDF::resetInputs() {

}

void PrintPDF::closeEvent(QCloseEvent* event) {
	PrintPDF::Screen::closeEvent(event, this);
}