#include "../Headers/SimulationUI.h"

SimulationUI::SimulationUI(QMainWindow* parent)
	: QMainWindow(parent), Screen()
{
	setWindowTitle("KoeWare");
	setMinimumSize(1024, 786);
	simUI.setupUi(this);

	connect(simUI.startSimButton, &QPushButton::released, this, &SimulationUI::StartSimul);

}

void SimulationUI::resetInputs()
{
}

void SimulationUI::closeEvent(QCloseEvent* event) {
	SimulationUI::Screen::closeEvent(event, this);
}
 
void SimulationUI::StartSimul() {
	int days = simUI.dayBox->value();
	db->DeleteTableContents("batchessim");
	db->DeleteTableContents("tebatchessim");
	db->DeleteTableContents("patientssim");
	db->DeleteTableContents("dosetopatientsim");
	sim->Run(days);
}