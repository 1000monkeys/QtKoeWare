
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


    int plotpoints = 100;

    QVector<double> x(102), y(102);
    for (int i = 0; i < 101; ++i)
    {
        x[i] = setXVal(i, 10, 100);
        y[i] = x[i] * x[i];
    }


    x[101] = x[100];
    y[101] = y[100] - 50;

    testUI.plot->addGraph();
    testUI.plot->graph()->setData(x, y);
    testUI.plot->xAxis->setLabel("x");
    testUI.plot->yAxis->setLabel("y");
    testUI.plot->xAxis->setRange(-10, 20);
    testUI.plot->yAxis->setRange(0, 101);
    testUI.plot->replot();
    testUI.plot->saveJpg("graph.jpg");
}

double TestUI::setXVal(int i, int xmax, int plotpoints) {
    double x = 0;
    int halves = plotpoints / 2;
    double conversion = halves / xmax;
    x = i / conversion - xmax;
    return x;
}

void TestUI::closeEvent(QCloseEvent* event) {
	TestUI::Screen::closeEvent(event, this);
}	

void TestUI::resetInputs() {
}

void TestUI::Test1() {
   QList<QVariant> list = db->GetAllRowsForValue("tebatchessim", "dateTimeProduced", "moBatchId", std::to_string(15));
   QList<QVariant>::iterator i;
   for (i = list.begin(); i != list.end(); ++i) {
       qDebug() << i->toDateTime();
   }


}

void TestUI::Test2() {
    graph->setTechnetiumGraph(5, true, true);
}

void TestUI::Test3() {
    db->printSimDb();
}
