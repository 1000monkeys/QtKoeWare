#pragma once

#include <qevent.h>
#include <qobject.h>
#include <qapplication.h>

class Screen
{

public:
	void closeEvent(QCloseEvent* event, QWidget* parent) {
		QMessageBox::StandardButton resBtn = QMessageBox::question(parent, "KoeWare",
			"Are you sure?\n",
			QMessageBox::No | QMessageBox::Yes,
			QMessageBox::Yes);
		if (resBtn != QMessageBox::Yes) {
			event->ignore();
		}
		else {
			QApplication::quit();
			event->accept();
		}
	}

	virtual void resetInputs() = 0;
};