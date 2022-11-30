#include "QtKoeWare.h"
#include "Headers/pdf.h"

QtKoeWare::QtKoeWare(QMainWindow* parent)
    : QMainWindow(parent), Screen()
{
    menuUI.setupUi(this);

    setWindowTitle("KoeWare");
    setMinimumSize(750, 500);

    QSqlError err = db->initDb();
    if (err.type() != QSqlError::NoError) {
        db->showError((QWidget*)this, err);
        return;
    }
    db->printDB();

    connect(menuUI.moBatch, &QAction::triggered, this, &QtKoeWare::goToMolybdeenBatch);
    connect(menuUI.teBatch, &QAction::triggered, this, &QtKoeWare::goToTechnetiumBatch);
    connect(menuUI.addDose, &QAction::triggered, this, &QtKoeWare::goToAddDose);
    connect(menuUI.addPatient, &QAction::triggered, this, &QtKoeWare::goToSubmitPatient);
    connect(menuUI.settings, &QAction::triggered, this, &QtKoeWare::goToSettings);
    connect(menuUI.startSimulation, &QAction::triggered, this, &QtKoeWare::goToSimulation);
    connect(menuUI.test, &QAction::triggered, this, &QtKoeWare::goToTest);
    connect(menuUI.gotoGraph, &QAction::triggered, this, &QtKoeWare::goToGraph);


    stackWidget = menuUI.centralwidget->findChild<QStackedWidget*>(QString::fromStdString("stackedWidget"));

    moBatchUI = new MolybdeenBatchUI(this);
    stackWidget->insertWidget(screenIds.moBatchId, moBatchUI);
    qDebug() << "mobatch";

    teBatchUI = new TechnetiumBatchUI(this);
    stackWidget->insertWidget(screenIds.teBatchId, teBatchUI);
    qDebug() << "tebatch";

    addDoseUI = new AddDoseUI(this);
    stackWidget->insertWidget(screenIds.addDoseId, addDoseUI);
    qDebug() << "adddose";

    addPatientUI = new AddPatientUI(this);
    stackWidget->insertWidget(screenIds.addPatientId, addPatientUI);
    qDebug() << "addpatient";

    settingsUI = new SettingsUI(this);
    settingsUI->setFixedSize(1000, 1000);
    stackWidget->insertWidget(screenIds.settingsId, settingsUI);
    qDebug() << "settings";

    startSimulationUI = new SimulationUI(this);
    stackWidget->insertWidget(screenIds.simId, startSimulationUI);
    qDebug() << "startsim";

    testUI = new TestUI(this);
    stackWidget->insertWidget(screenIds.testId, testUI);
    qDebug() << "testui";

    graphUI = new GraphUI(this);
    stackWidget->insertWidget(screenIds.graphId, graphUI);
    qDebug() << "grapjeui";

    Pdf pdf;
}

void QtKoeWare::resetAll() {
    moBatchUI->resetInputs();
    teBatchUI->resetInputs();
    addDoseUI->resetInputs();
    addPatientUI->resetInputs();
    startSimulationUI->resetInputs();
    testUI->resetInputs();

    resetInputs();
}

void QtKoeWare::resetInputs()
{
}

void QtKoeWare::goToSettings() {
    stackWidget->setCurrentIndex(screenIds.settingsId);
}

void QtKoeWare::goToMolybdeenBatch() {
    stackWidget->setCurrentIndex(screenIds.moBatchId);
}

void QtKoeWare::goToTechnetiumBatch() {
    stackWidget->setCurrentIndex(screenIds.teBatchId);
}

void QtKoeWare::goToAddDose() {
    stackWidget->setCurrentIndex(screenIds.addDoseId);
}

void QtKoeWare::goToSubmitPatient() {
    stackWidget->setCurrentIndex(screenIds.addPatientId);
}

void QtKoeWare::goToSimulation() {
    stackWidget->setCurrentIndex(screenIds.simId);
}

void QtKoeWare::goToTest() {
    stackWidget->setCurrentIndex(screenIds.testId);
}

void QtKoeWare::goToGraph() {
    stackWidget->setCurrentIndex(screenIds.graphId);
}

void QtKoeWare::closeEvent(QCloseEvent* event) {
    QtKoeWare::Screen::closeEvent(event, this);
}

QtKoeWare::~QtKoeWare()
{
}