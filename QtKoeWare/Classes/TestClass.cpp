
#include "../Headers/TestClass.h"
#include <iostream>

TestUI::TestUI(QMainWindow* parent)
	: QMainWindow(parent), Screen()
{
	setWindowTitle("KoeWare");
	setMinimumSize(500, 300);
	testUI.setupUi(this);
	connect(testUI.testButton1, &QPushButton::released, this, &TestUI::Test1);
	connect(testUI.testButton2, &QPushButton::released, this, &TestUI::Test2);
	connect(testUI.testButton3, &QPushButton::released, this, &TestUI::Test3);
}

void TestUI::closeEvent(QCloseEvent* event) {
	TestUI::Screen::closeEvent(event, this);
}	

void TestUI::resetInputs() {
}

void TestUI::Test1() {
	sim->CreatePatient();
}

void TestUI::Test2() {
	sim->MolybdeenBatch();
	sim->TechnetiumBatch();
}

void TestUI::Test3() {
	db->printSimDb();
}
