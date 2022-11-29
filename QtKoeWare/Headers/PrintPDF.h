#pragma once

#include <QtWidgets/QMainWindow>
#include <QtUiTools>

#include "ui_PrintPDF.h"

#include "Screen.h"

QT_BEGIN_NAMESPACE
class PrintPDF : public QMainWindow, public Screen
{
    Q_OBJECT

public:
    PrintPDF(QMainWindow* parent = nullptr);
    void resetInputs();

private:
    Ui_PrintPDFUIFile printPDF;

    void closeEvent(QCloseEvent* event);
    void search();
};
QT_END_NAMESPACE