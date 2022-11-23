#include "../Headers/SelectDateDialog.h"

SelectDateDialog::SelectDateDialog(QWidget* parent) : QDialog(parent)
{
	setMinimumSize(400, 300);
	dateDialog_Ui.setupUi(this);
}

QDate SelectDateDialog::getDate()
{
	return dateDialog_Ui.calendarWidget->selectedDate();
}

void SelectDateDialog::setDate(QDate date)
{
	dateDialog_Ui.calendarWidget->setSelectedDate(date);
}
