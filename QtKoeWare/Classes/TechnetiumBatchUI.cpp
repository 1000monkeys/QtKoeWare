#include "../Headers/TechnetiumBatchUI.h"

TechnetiumBatchUI::TechnetiumBatchUI(QMainWindow* parent)
    :QMainWindow(parent), Screen()
{
    settings = db->getSettings();

    setWindowTitle("KoeWare");
    setMinimumSize(750, 500);

    technetiumUi.setupUi(this);

    connect(technetiumUi.selectBatchButton, &QPushButton::released, this, &TechnetiumBatchUI::showBatchDialog);
    connect(technetiumUi.moBatchLine, &QLineEdit::textChanged, this, &TechnetiumBatchUI::moBatchLineCheck);

    connect(technetiumUi.checkAvailabilityButton, &QPushButton::released, this, &TechnetiumBatchUI::checkAvailability);
    connect(technetiumUi.teBatchLine, &QLineEdit::textChanged, this, &TechnetiumBatchUI::checkAvailability);

    connect(technetiumUi.setDateButton, &QPushButton::released, this, &TechnetiumBatchUI::showDateDialog);
    connect(technetiumUi.teDateEdit, &QDateEdit::dateChanged, this, &TechnetiumBatchUI::dateEditCheck);

    connect(technetiumUi.teTimeEdit, &QTimeEdit::timeChanged, this, &TechnetiumBatchUI::timeEditCheck);

    connect(technetiumUi.teMeasureButton, &QPushButton::released, this, &TechnetiumBatchUI::randomRadioActivity);
    connect(technetiumUi.teRadioactivityLine, &QLineEdit::textChanged, this, &TechnetiumBatchUI::radioActivityCheck);

    technetiumUi.teDateEdit->setDate(QDate::currentDate());
    technetiumUi.teTimeEdit->setTime(QTime::currentTime());

    int num = db->LastInt("tebatches", "teBatchId");
    technetiumUi.teBatchLine->setText(QString::number(num + 1));

    connect(technetiumUi.teSubmitButton, &QPushButton::released, this, &TechnetiumBatchUI::submitBatch);
}

checkReturnData TechnetiumBatchUI::dateEditCheck()
{
    checkReturnData returnData;

    if (QDate::currentDate() > technetiumUi.teDateEdit->date()) {
        returnData.alerts.push_back("This date is in the past!");
    }
    else if (QDate::currentDate() < technetiumUi.teDateEdit->date()) {
        returnData.alerts.push_back("This date is in the future!");
    }

    return returnData;
}

checkReturnData TechnetiumBatchUI::timeEditCheck()
{
    checkReturnData returnData;

    int min_te_production_time = std::stoi(settings["min_te_production_time"]);
    int max_te_production_time = std::stoi(settings["max_te_production_time"]);

    if (technetiumUi.teTimeEdit->time().hour() < min_te_production_time) {
        returnData.alerts.push_back("This time is below 3 in the morning!\nSo outside normal production time!");
    }
    else if (technetiumUi.teTimeEdit->time().hour() > max_te_production_time) {
        returnData.alerts.push_back("This time is over 7 in the morning!\nSo outside normal production time!");
    }

    return returnData;
}

checkReturnData TechnetiumBatchUI::moBatchLineCheck() { 
    checkReturnData returnData;

    int moBatchId = technetiumUi.moBatchLine->text().toInt();
    int dbValue = db->GetdbInt("batches", "batchId", "batchId", std::to_string(moBatchId));
    if (dbValue == moBatchId) {
        technetiumUi.moBatchLine->setStyleSheet("background-color: white; color: black;");
    }
    else {
        returnData.errors.push_back("Batch ID is unknown!");
        technetiumUi.moBatchLine->setStyleSheet("background-color: #FF7276; color: black;");
    }

    return returnData;
}

checkReturnData TechnetiumBatchUI::radioActivityCheck() {
    checkReturnData returnData;

    int min_te_radioactivity = std::stoi(settings["min_te_radioactivity"]);
    int max_te_radioactivity = std::stoi(settings["max_te_radioactivity"]);

    if (technetiumUi.teRadioactivityLine->text().toInt() < min_te_radioactivity) {
        returnData.errors.push_back("Radio activity too low!");
        technetiumUi.teRadioactivityLine->setStyleSheet("background-color: #FF7276; color: black;");
    }
    else if (technetiumUi.teRadioactivityLine->text().toInt() > max_te_radioactivity) {
        returnData.errors.push_back("Radio activity too high!");
        technetiumUi.teRadioactivityLine->setStyleSheet("background-color: #FF7276; color: black;");
    }
    else {
        technetiumUi.teRadioactivityLine->setStyleSheet("background-color: white; color: black;");
    }

    return returnData;
}

checkReturnData TechnetiumBatchUI::checkAvailability() {
    checkReturnData returnData;

    std::string batchIdString = "teBatchId";
    std::string batchid = technetiumUi.teBatchLine->text().toStdString();
    QList<std::string> returnValues = { batchIdString };
    QList<QList<QVariant>> result = db->GetWhere("tebatches", returnValues, batchIdString, batchid);

    bool teBatchIdFlag = false;
    for (QList<QVariant> firstList : result) {
        for (QVariant var : firstList) {
            if (var.toInt() == std::stoi(batchid)) {
                returnData.errors.push_back("Technetium batch ID is not available!");
                technetiumUi.teBatchLine->setStyleSheet("background-color: #FF7276; color: black;");
                teBatchIdFlag = true;
            }
        }
    }

    if (!teBatchIdFlag) {
        technetiumUi.teBatchLine->setStyleSheet("background-color: white; color: black;");
    }

    return returnData;
}

void TechnetiumBatchUI::submitBatch() {
    std::list<std::string> errors;
    std::list<std::string> alerts;


    checkReturnData newReturnData = moBatchLineCheck();
    alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
    errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

    newReturnData = radioActivityCheck();
    alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
    errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

    newReturnData = checkAvailability();
    alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
    errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

    newReturnData = timeEditCheck();
    alerts.insert(alerts.end(), newReturnData.alerts.begin(), newReturnData.alerts.end());
    errors.insert(errors.end(), newReturnData.errors.begin(), newReturnData.errors.end());

    newReturnData = dateEditCheck();
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
                dt.setDate(technetiumUi.teDateEdit->date());
                dt.setTime(technetiumUi.teTimeEdit->time());
                db->addTechnetiumBatch(std::stoi(technetiumUi.moBatchLine->text().toStdString()), std::stoi(technetiumUi.teBatchLine->text().toStdString()), dt, std::stoi(technetiumUi.teRadioactivityLine->text().toStdString()));
                db->printDB();
                resetInputs();
            }
        }
    }
}

void TechnetiumBatchUI::showDateDialog() {
    SelectDateDialog dateDialog = new SelectDateDialog(this);
    dateDialog.setDate(technetiumUi.teDateEdit->date());
    dateDialog.exec();

    QDate date = dateDialog.getDate();
    technetiumUi.teDateEdit->setDate(date);
}

void TechnetiumBatchUI::randomRadioActivity() {
    int min_te_radioactivity = std::stoi(settings["min_te_radioactivity"]);
    int max_te_radioactivity = std::stoi(settings["max_te_radioactivity"]);

    technetiumUi.teRadioactivityLine->setText(QString::number(Random(min_te_radioactivity, max_te_radioactivity)));
}

void TechnetiumBatchUI::technetiumBatchID() {
    // get last batch id, add 1
    int lastid = db->LastInt("tebatches", "teBatchId");
    if (lastid == 0) {
        tebatchid = 1;
    }
    else {
        tebatchid += 1;
    }
    technetiumUi.teBatchLine->setText(QString::number(tebatchid));
}

void TechnetiumBatchUI::resetInputs() {
    technetiumUi.moBatchLine->setText("");
    technetiumUi.teBatchLine->setText("");

    technetiumUi.teDateEdit->setDate(QDate::currentDate());
    technetiumUi.teTimeEdit->setTime(QTime::currentTime());

    technetiumUi.teRadioactivityLine->setText("");
}

void TechnetiumBatchUI::showBatchDialog() {
    SelectTableDialog tableDialog = new SelectTableDialog(this);
    tableDialog.setModal(true);

    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM batches;");
    model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Batch ID"));
    model->setHeaderData(2, Qt::Horizontal, tr("Date/Time produced"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date/Time arrived"));
    model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
    tableDialog.setModel(model);

    tableDialog.exec();
    if (tableDialog.getRowId() != NULL) {
        int batchId = db->GetdbInt("batches", "batchId", "id", std::to_string(tableDialog.getRowId()));
        technetiumUi.moBatchLine->setText(QString::number(batchId));
    }
    else {
        technetiumUi.moBatchLine->setText("");
    }
}

void TechnetiumBatchUI::closeEvent(QCloseEvent* event) {
    TechnetiumBatchUI::Screen::closeEvent(event, this);
}
