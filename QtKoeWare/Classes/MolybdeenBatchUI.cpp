#include "../Headers/MolybdeenBatchUI.h"

MolybdeenBatchUI::MolybdeenBatchUI(QMainWindow* parent)
	: QMainWindow(parent), Screen()
{
	settings = db->getSettings();
	
	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);

	batchUI.setupUi(this);


	//connect(batchUI.produceDateEdit, &QDateEdit::, this, &MolybdeenBatchUI::ShowProducedDateDialog);

	connect(batchUI.measureDateEdit, &QDateEdit::dateChanged, this, &MolybdeenBatchUI::measureDateCheck);
	connect(batchUI.measureTimeEdit, &QTimeEdit::timeChanged, this, &MolybdeenBatchUI::measureTimeCheck);

	connect(batchUI.produceDateEdit, &QDateEdit::dateChanged, this, &MolybdeenBatchUI::produceDateCheck);
	connect(batchUI.produceTimeEdit, &QTimeEdit::timeChanged, this, &MolybdeenBatchUI::produceTimeCheck);

	connect(batchUI.batchIDline, &QLineEdit::textChanged, this, &MolybdeenBatchUI::batchIDCheck);
	connect(batchUI.radioActivityLine, &QLineEdit::textChanged, this, &MolybdeenBatchUI::radioActivityCheck);

	
	connect(batchUI.produceDateButton, &QPushButton::released, this, &MolybdeenBatchUI::showProducedDateDialog);
	connect(batchUI.measureDateButton, &QPushButton::released, this, &MolybdeenBatchUI::showMeasuredDateDialog);
	connect(batchUI.submitButton, &QPushButton::released, this, &MolybdeenBatchUI::submitBatch);

	batchUI.produceDateEdit->setDate(QDate::currentDate());
	batchUI.produceTimeEdit->setTime(QTime::currentTime());
	batchUI.measureDateEdit->setDate(QDate::currentDate());
	batchUI.measureTimeEdit->setTime(QTime::currentTime());
}

void MolybdeenBatchUI::showProducedDateDialog() {
	SelectDateDialog dateDialog = new SelectDateDialog(this);
	dateDialog.setDate(batchUI.produceDateEdit->date());
	dateDialog.exec();

	QDate date = dateDialog.getDate();
	batchUI.produceDateEdit->setDate(date);
}

void MolybdeenBatchUI::showMeasuredDateDialog() {
	SelectDateDialog dateDialog = new SelectDateDialog(this);
	dateDialog.setDate(batchUI.measureDateEdit->date());
	dateDialog.exec();

	QDate date = dateDialog.getDate();
	batchUI.measureDateEdit->setDate(date);
}

checkReturnData MolybdeenBatchUI::radioActivityCheck() {
	checkReturnData returnData;

	int radioActivity = batchUI.radioActivityLine->text().toInt();
	int min_mo_radioactivity = std::stoi(settings["min_mo_radioactivity"]);
	int max_mo_radioactivity = std::stoi(settings["max_mo_radioactivity"]);

	if (radioActivity < min_mo_radioactivity) {
		returnData.errors.push_back("Radio activity too low!");
		batchUI.radioActivityLine->setStyleSheet("background-color: #FF7276; color: black;");
	}
	else if (max_mo_radioactivity < radioActivity ) {
		returnData.errors.push_back("Radio activity too high!");
		batchUI.radioActivityLine->setStyleSheet("background-color: #FF7276; color: black;");
	}
	else {
		batchUI.radioActivityLine->setStyleSheet("background-color: white; color: black;");
	}

	return returnData;
}

checkReturnData MolybdeenBatchUI::batchIDCheck() {
	checkReturnData returnData;

	int moBatchID = batchUI.batchIDline->text().toInt();
	int dbValue = db->GetdbInt("batches", "batchId", "batchId", std::to_string(moBatchID));

	qDebug() << moBatchID;
	qDebug() << dbValue;

	if (dbValue == moBatchID) {
		returnData.errors.push_back("Batch ID already used!");
		batchUI.batchIDline->setStyleSheet("background-color: #FF7276; color: black;");
	}
	else {
		batchUI.batchIDline->setStyleSheet("background-color: white; color: black;");
	}

	return returnData;
}

checkReturnData MolybdeenBatchUI::produceDateCheck() {
	checkReturnData returnData;

	if (QDate::currentDate() > batchUI.produceDateEdit->date()) {
		returnData.alerts.push_back("Production date is in the past!");
	}
	else if (QDate::currentDate() < batchUI.produceDateEdit->date()) {
		returnData.alerts.push_back("Production date is in the future!");
	}

	return returnData;
}

checkReturnData MolybdeenBatchUI::produceTimeCheck() {
	checkReturnData returnData;

	int min_mo_production_time = std::stoi(settings["min_mo_production_time"]);
	int max_mo_production_time = std::stoi(settings["max_mo_production_time"]);
	if (batchUI.produceTimeEdit->time().hour() < min_mo_production_time) {
		returnData.alerts.push_back("This time is below 3 in the morning!\nSo outside normal production time!");
	}
	else if (batchUI.produceTimeEdit->time().hour() > max_mo_production_time) {
		returnData.alerts.push_back("This time is over 7 in the morning!\nSo outside normal production time!");
	}

	return returnData;
}

checkReturnData MolybdeenBatchUI::measureDateCheck() {
	checkReturnData returnData;

	if (QDate::currentDate() > batchUI.measureDateEdit->date()) {
		returnData.alerts.push_back("Measure date is in the past!");
	}
	else if (QDate::currentDate() < batchUI.measureDateEdit->date()) {
		returnData.alerts.push_back("Measure date is in the future!");
	}

	return returnData;
}

checkReturnData MolybdeenBatchUI::measureTimeCheck() {
	checkReturnData returnData;


	int time_offset = std::stoi(settings["secs_time_offset"]);
	if (QTime::currentTime() < batchUI.produceTimeEdit->time().addSecs(-time_offset)) {
		returnData.alerts.push_back("Measure time is far in the past!");
	}
	else if (QTime::currentTime() > batchUI.produceTimeEdit->time().addSecs(time_offset)) {
		returnData.alerts.push_back("Measure time is far in the future!");
	}

	return returnData;
}

void MolybdeenBatchUI::submitBatch() {
	std::list<std::string> errors;
	std::list<std::string> alerts;

	checkReturnData newReturnData = batchIDCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = produceDateCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = produceTimeCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = measureDateCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = measureTimeCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	if (errors.size() != 0) {
		std::string errorString = "";
		for (auto const& item : errors) {
			//qDebug() << QString::fromStdString(item);
			errorString += item + " \n";
		}
		QMessageBox::warning(this, "There are errors!", QString::fromStdString(errorString), QMessageBox::Ok);
	}else {
		QMessageBox::StandardButton reply{};
		if (alerts.size() != 0) {
			std::string alertString = "";
			for (auto const& item : alerts) {
				//qDebug() << QString::fromStdString(item);
				alertString += item + " \n";
			}

			reply = QMessageBox::question(this, "Is this right?", QString::fromStdString(alertString), QMessageBox::Yes | QMessageBox::No);
		}
		if (alerts.size() == 0 || reply == QMessageBox::Yes) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Are you sure?", "Submit to database?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				qDebug() << "Submitting to database";
				db->addMolybdeenBatch(
					std::stoi(batchUI.batchIDline->text().toStdString()),
					QDateTime(batchUI.produceDateEdit->date(), batchUI.produceTimeEdit->time()),
					QDateTime(batchUI.measureDateEdit->date(), batchUI.measureTimeEdit->time()),
					batchUI.radioActivityLine->text().toInt()
				);
				db->printDB();
				resetInputs();
			}
		}
	}
}

void MolybdeenBatchUI::resetInputs() {
	batchUI.batchIDline->setText("");

	batchUI.produceDateEdit->setDate(date);
	batchUI.produceTimeEdit->setTime(time);

	batchUI.measureTimeEdit->setTime(time);
	batchUI.measureDateEdit->setDate(date);

	batchUI.radioActivityLine->setText("");
}

void MolybdeenBatchUI::closeEvent(QCloseEvent* event) {
	MolybdeenBatchUI::Screen::closeEvent(event, this);
}