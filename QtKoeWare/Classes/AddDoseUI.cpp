#include "../Headers/AddDoseUI.h"


AddDoseUI::AddDoseUI(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	settings = db->getSettings();

	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);

	addDoseUI.setupUi(this);

	connect(addDoseUI.selectBatchButton, &QPushButton::released, this, &AddDoseUI::ShowBatchDialog);
	connect(addDoseUI.selectPatientButton, &QPushButton::released, this, &AddDoseUI::ShowPatientDialog);
	connect(addDoseUI.setRadioActivityButton, &QPushButton::released, this, &AddDoseUI::RandomizeRadioActivity);

	connect(addDoseUI.injectionDateButton, &QPushButton::released, this, &AddDoseUI::ShowDateDialog);
	connect(addDoseUI.submitButton, &QPushButton::released, this, &AddDoseUI::submitDoseData);

	connect(addDoseUI.batchLine, &QLineEdit::textChanged, this, &AddDoseUI::batchCheck);
	connect(addDoseUI.patientLine, &QLineEdit::textChanged, this, &AddDoseUI::patientCheck);
	connect(addDoseUI.radioActivityLine, &QLineEdit::textChanged, this, &AddDoseUI::radioActivityCheck);

	addDoseUI.injectionDateEdit->setDate(QDate::currentDate());
	addDoseUI.injectionTimeEdit->setTime(QTime::currentTime());
}

void AddDoseUI::resetInputs() {
	addDoseUI.batchLine->setText("");
	addDoseUI.injectionTimeEdit->setTime(QTime::currentTime());
	addDoseUI.patientLine->setText("");
	addDoseUI.radioActivityLine->setText("");
}

void AddDoseUI::closeEvent(QCloseEvent* event) {
	AddDoseUI::Screen::closeEvent(event, this);
}

void AddDoseUI::submitDoseData() {
	std::list<std::string> errors;
	std::list<std::string> alerts;

	checkReturnData newReturnData = batchCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = patientCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = radioActivityCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());


	if (errors.size() != 0) {
		std::string errorString = "";
		for (auto const& item : errors) {
			//qDebug() << QString::fromStdString(item);
			errorString += item + " \n";
		}
		QMessageBox::warning(this, "There are errors!", QString::fromStdString(errorString), QMessageBox::Ok);
	}else{
		// Show alerts
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
			//Show are you sure screen
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Are you sure?", "Submit to database?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				qDebug() << "Submitting to databse";
				QDateTime dt;
				dt.setDate(QDate::currentDate());
				dt.setTime(addDoseUI.injectionTimeEdit->time());

				db->addDoseToPatient(
					addDoseUI.patientLine->text().toInt(),
					addDoseUI.batchLine->text().toInt(),
					dt,
					addDoseUI.radioActivityLine->text().toInt()
				);
				resetInputs();
			}
		}
	}
}

void AddDoseUI::RandomizeRadioActivity() {
	addDoseUI.radioActivityLine->setText(QString::number(Random(300, 700)));
}

void AddDoseUI::ShowDateDialog() {
	SelectDateDialog dateDialog = new SelectDateDialog(this);
	dateDialog.setDate(addDoseUI.injectionDateEdit->date());
	dateDialog.exec();

	QDate date = dateDialog.getDate();
	addDoseUI.injectionDateEdit->setDate(date);
}

checkReturnData AddDoseUI::batchCheck()
{
	checkReturnData returnData;

	int teBatchId = addDoseUI.batchLine->text().toInt();
	int dbValue = db->GetdbInt("tebatches", "teBatchId", "teBatchId", std::to_string(teBatchId));
	if (teBatchId == dbValue) {
		addDoseUI.batchLine->setStyleSheet("background-color: white; color: black;");
	}
	else {
		addDoseUI.batchLine->setStyleSheet("background-color: #FF7276; color: black;");
		returnData.errors.push_back("Technetium batch ID is unknown!");
	}

	return returnData;
}

checkReturnData AddDoseUI::patientCheck()
{
	checkReturnData returnData;

	int patientId = addDoseUI.patientLine->text().toInt();
	int dbValue = db->GetdbInt("patients", "patientId", "patientId", std::to_string(patientId));
	if (patientId == dbValue) {
		addDoseUI.patientLine->setStyleSheet("background-color: white; color: black;");
	}
	else {
		addDoseUI.patientLine->setStyleSheet("background-color: #FF7276; color: black;");
		returnData.errors.push_back("Patient ID is unknown!");
	}

	return returnData;
}

checkReturnData AddDoseUI::radioActivityCheck()
{
	checkReturnData returnData;

	int min_dose_radioactivity = std::stoi(settings["min_dose_radioactivity"]);
	int max_dose_radioactivity = std::stoi(settings["max_dose_radioactivity"]);

	int radioActivity = addDoseUI.radioActivityLine->text().toInt();
	if (radioActivity < min_dose_radioactivity) {
		returnData.errors.push_back("Radio activity is too low!");
		addDoseUI.radioActivityLine->setStyleSheet("background-color: #FF7276; color: black;");
	}
	else if (radioActivity > max_dose_radioactivity) {
		returnData.errors.push_back("Radio activity is too high!");
		addDoseUI.radioActivityLine->setStyleSheet("background-color: #FF7276; color: black;");
	}
	else {
		addDoseUI.radioActivityLine->setStyleSheet("background-color: white; color: black;");
	}

	return returnData;
}

void AddDoseUI::ShowBatchDialog() {
	// CHANGE TO TE BATCH
	SelectTableDialog tableDialog = new SelectTableDialog(this);
	tableDialog.setModal(true);

	QSqlQueryModel* model = new QSqlQueryModel;
	model->setQuery("SELECT * FROM tebatches;");
	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Molybdeen Batch ID"));
	model->setHeaderData(2, Qt::Horizontal, tr("Technetium Batch ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("Date/Time produced"));
	model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
	tableDialog.setModel(model);

	tableDialog.exec();
	if (tableDialog.getRowId() != NULL) {
		int batchId = db->GetdbInt("tebatches", "teBatchId", "id", std::to_string(tableDialog.getRowId()));
		addDoseUI.batchLine->setText(QString::number(batchId));
	}
	else {
		addDoseUI.batchLine->setText("");
	}
}

void AddDoseUI::ShowPatientDialog() {
	SelectTableDialog tableDialog = new SelectTableDialog(this);
	tableDialog.setModal(true);

	QSqlQueryModel* model = new QSqlQueryModel;
	model->setQuery("SELECT * FROM patients;");
	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	model->setHeaderData(2, Qt::Horizontal, tr("Length"));
	model->setHeaderData(3, Qt::Horizontal, tr("Weight"));
	model->setHeaderData(4, Qt::Horizontal, tr("Birthdate"));
	model->setHeaderData(5, Qt::Horizontal, tr("Sex"));
	tableDialog.setModel(model);

	tableDialog.exec();
	if (tableDialog.getRowId() != NULL) {
		int patientId = db->GetdbInt("patients", "patientId", "patientId", std::to_string(tableDialog.getRowId()));
		addDoseUI.patientLine->setText(QString::number(patientId));
	}
	else {
		addDoseUI.patientLine->setText("");
	}
}
