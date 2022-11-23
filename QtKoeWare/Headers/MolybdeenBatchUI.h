#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>

#include "ui_MolybdeenBatch.h"

#include "Screen.h"
#include "DataBase.h"
#include "SelectDateDialog.h"

#include "../Classes/General.cpp"

QT_BEGIN_NAMESPACE
class MolybdeenBatchUI : public QMainWindow, public Screen
{
    Q_OBJECT

public:
    MolybdeenBatchUI(QMainWindow* parent = nullptr);
    void resetInputs();

private:
    Ui_MolybdeenBatchUIFile batchUI;

    void closeEvent(QCloseEvent* event);

    DataBase* db = new DataBase;
    std::map<std::string, std::string> settings;

    checkReturnData batchIDCheck();
    checkReturnData produceDateCheck();
    checkReturnData produceTimeCheck();
    checkReturnData measureDateCheck();
    checkReturnData measureTimeCheck();
    checkReturnData radioActivityCheck();

    void showProducedDateDialog();
    void showMeasuredDateDialog();

    void submitBatch();

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QTime newTime = QTime::currentTime();
    QTime measureTime = QTime::currentTime();
};
QT_END_NAMESPACE