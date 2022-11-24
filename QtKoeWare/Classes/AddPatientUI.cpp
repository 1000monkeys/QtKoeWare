#include "../Headers/AddPatientUI.h"

AddPatientUI::AddPatientUI(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	settings = db->getSettings();

	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);

	patientUI.setupUi(this);

	connect(patientUI.setBirthDate, &QPushButton::released, this, &AddPatientUI::ShowDateDialog);

	connect(patientUI.name, &QLineEdit::textChanged, this, &AddPatientUI::nameCheck);
	connect(patientUI.length, &QLineEdit::textChanged, this, &AddPatientUI::lengthCheck);
	connect(patientUI.weigth, &QLineEdit::textChanged, this, &AddPatientUI::weigthCheck);
	connect(patientUI.birthdate, &QDateEdit::dateChanged, this, &AddPatientUI::birthdateCheck);
	connect(patientUI.sex, &QButtonGroup::buttonPressed, this, &AddPatientUI::sexCheck);

	connect(patientUI.submit, &QPushButton::released, this, &AddPatientUI::SubmitPatientData);

	patientUI.sex->setId(patientUI.male, 0);
	patientUI.sex->setId(patientUI.female, 1);
	patientUI.birthdate->setDate(QDate::currentDate());
}

void AddPatientUI::ShowDateDialog() {
	SelectDateDialog dateDialog = new SelectDateDialog(this);
	dateDialog.setDate(patientUI.birthdate->date());
	dateDialog.exec();

	QDate date = dateDialog.getDate();
	
	patientUI.birthdate->setDate(date);
}

void AddPatientUI::resetInputs() {
	patientUI.name->setText("");
	patientUI.length->setText("");
	patientUI.weigth->setText("");
	patientUI.birthdate->setDate(date);

	//qDebug() << patientUI.sex->checkedId();

	patientUI.sex->setExclusive(false);
	patientUI.male->setChecked(false);
	patientUI.female->setChecked(false);
	patientUI.sex->setExclusive(true);
}

checkReturnData AddPatientUI::nameCheck() {
	checkReturnData returnData;

	int min_name_length = std::stoi(settings["min_name_length"]);
	int short_name_length = std::stoi(settings["short_name_length"]);
	int really_long_name_length = std::stoi(settings["really_long_name_length"]);

	if (patientUI.name->text().length() < min_name_length) {
		returnData.errors.push_back("Name is not entered!");
	}

	if (patientUI.name->text().length() < short_name_length) {
		returnData.alerts.push_back("Name is really short!");
	}else if (patientUI.name->text().length() > really_long_name_length) {
		returnData.alerts.push_back("Name is really long!");
	}

	return returnData;
}

checkReturnData AddPatientUI::lengthCheck() {
	checkReturnData returnData;

	int min_length = std::stoi(settings["min_height"]);
	int really_really_short_length = std::stoi(settings["really_really_short_height"]);
	int really_short_length = std::stoi(settings["really_short_height"]);
	int really_tall_length = std::stoi(settings["really_tall_height"]);
	int really_really_tall_length = std::stoi(settings["really_really_tall_height"]);

	if (patientUI.length->text().length() < min_length) {
		returnData.errors.push_back("Length is not entered!");
	}

	int length = patientUI.length->text().toInt();
	if (length < really_really_short_length) {
		returnData.alerts.push_back("Length is really really short!");
	}
	else if (length < really_short_length) {
		returnData.alerts.push_back("Length is really short!");
	}
	else if (length > really_tall_length) {
		returnData.alerts.push_back("Length is really tall!");
	}
	else if (length > really_really_tall_length) {
		returnData.alerts.push_back("Length is really really tall!");
	}

	return returnData;
}

checkReturnData AddPatientUI::weigthCheck() {
	checkReturnData returnData;

	int min_weight = std::stoi(settings["min_weight"]);
	int really_really_light_weight = std::stoi(settings["really_really_light_weigth"]);
	int really_light_weight = std::stoi(settings["really_light_weigth"] );
	int really_really_light_weigth = std::stoi(settings["really_really_light_weigth"]);
	int really_heavy_weight = std::stoi(settings["really_heavy_weigth"]);

	if (patientUI.weigth->text().length() < min_weight) {
		returnData.errors.push_back("Weight is not entered!");
	}

	int weigth = patientUI.weigth->text().toInt();
	if (weigth < really_really_light_weight) {
		returnData.alerts.push_back("Weight is really really light!");
	}
	else if (weigth < really_light_weight) {
		returnData.alerts.push_back("Weight is really light!");
	}
	else if (weigth > really_really_light_weigth) {
		returnData.alerts.push_back("Weight is really really heavy!");
	}
	else if (weigth > really_heavy_weight) {
		returnData.alerts.push_back("Weight is really heavy!");
	}

	return returnData;
}

checkReturnData AddPatientUI::birthdateCheck() {
	checkReturnData returnData;

	if (patientUI.birthdate->date() == QDate::currentDate()) {
		returnData.alerts.push_back("Birthdate has not changed!");
	}

	return returnData;
}

checkReturnData AddPatientUI::sexCheck() {
	checkReturnData returnData;

	if (patientUI.sex->checkedId() == -1) {
		returnData.errors.push_back("No sex is selected!");
	}

	return returnData;
}

void AddPatientUI::SubmitPatientData() {
	std::list<std::string> errors;
	std::list<std::string> alerts;

	checkReturnData newReturnData = nameCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = lengthCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = weigthCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = birthdateCheck();
	alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
	errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

	newReturnData = sexCheck();
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
		QMessageBox::StandardButton reply{};
		if (alerts.size() != 0) {
			std::string alertString = "";
			for (auto const& item : alerts) {
				//qDebug() << QString::fromStdString(item);
				alertString += item + " \n";
			}

			reply = QMessageBox::question(this, "Is this right?", QString::fromStdString(alertString), QMessageBox::Yes | QMessageBox::No);
		}
		if(alerts.size() == 0 || reply == QMessageBox::Yes) {
			db->addPatient(
				patientUI.name->text(),
				patientUI.length->text().toInt(),
				patientUI.weigth->text().toInt(),
				patientUI.birthdate->date(),
				patientUI.sex->checkedId()
			);
			QMessageBox::information(this, "Submitted!", "Submitted succesfully!", QMessageBox::Ok);
		}
	}
}

void AddPatientUI::closeEvent(QCloseEvent* event) {
	AddPatientUI::Screen::closeEvent(event, this);
}
