#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_KoeWare.h"

#include "Headers/Database.h"
#include "Headers/Screen.h"
#include "Headers/AddPatientUI.h"
#include "Headers/TechnetiumBatchUI.h"
#include "Headers/MolybdeenBatchUI.h"
#include "Headers/AddDoseUI.h"
#include "Headers/SettingsUI.h"

#include "Headers/TestClass.h"
#include "Headers/SimulationUI.h"


struct ScreenIds {
    int moBatchId = 0;
    int addPatientId = 1;
    int teBatchId = 2;
    int simId = 3;
    int addDoseId = 4;
    int testId = 5;
    int settingsId = 6;
};

QT_BEGIN_NAMESPACE
class QtKoeWare : public QMainWindow, public Screen
{
    Q_OBJECT

public:
    QtKoeWare(QMainWindow* parent = nullptr);
    void resetInputs();
    ~QtKoeWare();

private:
    Ui_MainWindow menuUI;

    DataBase* db;

    QStackedWidget* stackWidget;
    ScreenIds screenIds;

    void closeEvent(QCloseEvent* event);

    void resetAll();

    void goToMolybdeenBatch();
    void goToSubmitPatient();
    void goToTechnetiumBatch();
    void goToSimulation();
    void goToSettings();
    void goToAddDose();
    void goToTest();

    MolybdeenBatchUI* moBatchUI;
    TechnetiumBatchUI* teBatchUI;
    AddDoseUI* addDoseUI;
    AddPatientUI* addPatientUI;
    SettingsUI* settingsUI;
    SimulationUI* startSimulationUI;
    TestUI* testUI;

    void setLastPositionMoved();
    bool moved = false;
};
QT_END_NAMESPACE