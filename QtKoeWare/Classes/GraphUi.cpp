#pragma once

#include"../Headers/GraphUi.h"
#include "../Headers/SelectTableDialog.h"



GraphUI::GraphUI(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);
	graphui.setupUi(this);
	int lastBatchId = db->LastInt("batches", "bathId");
	if (lastBatchId != 0) {
		setMolybdenumGraph(lastBatchId, false, true);
	}

	/*TEST*/
	int id = db->LastInt("batchessim", "batchId");
	setMolybdenumGraph(id, true, true);
	setTechnetiumGraph(5, true, true);
	/*TEST*/
	
	connect(graphui.selectMoButton, &QPushButton::released, this, &GraphUI::selectMoBatch);
	connect(graphui.selectTeButton, &QPushButton::released, this, &GraphUI::selectTeBatch);
	
}

void GraphUI::setMolybdenumGraph(int moBatch, bool simGraph, bool latestBatch) {
	int startactivity = 0;
	QString measuredDateTimeString;
	graphui.plot->addGraph();
	if (!simGraph){
		measuredDateTimeString = db->GetdbDateTime("batches", "dateTimeMeasured", "batchId", std::to_string(moBatch));
		startactivity = db->GetdbInt("batches", "radioactivity", "batchId", std::to_string(moBatch));
		if (measuredDateTimeString == "no val") {
			return; 
		}
	}
	else {
		measuredDateTimeString = db->GetdbDateTime("batchessim", "dateTimeMeasured", "batchId", std::to_string(moBatch));
		startactivity = db->GetdbInt("batchessim", "radioactivity", "batchId", std::to_string(moBatch));
		if (measuredDateTimeString == "no val") {
			return;
		}
	}

	//Get start time and endtime. Starttime is measured time. Endtime is time of last activity (if no new mo batch)
	QDateTime timeNow;
	QDateTime measureDateTime = measureDateTime.fromString(measuredDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
	if (latestBatch) {
		if (simGraph) {
			timeNow = getTimeNow(true);
		}
		else {
			timeNow = getTimeNow(false);
		}	
	}
	else {
		timeNow = measureDateTime.addDays(7);
	}
	double starttime = measureDateTime.toSecsSinceEpoch();
	double endtime = timeNow.toSecsSinceEpoch();


	// Set plot data
	QVector<QCPGraphData> timedel(101);
	double plotpoints = 100;
	double timeDelta = (endtime - starttime) / plotpoints;
	double halflife = 66 * 3600;

	for (int i = 0; i < 101; ++i) {
		timedel[i].key = starttime + (i * timeDelta);
		timedel[i].value = startactivity * pow(0.5, (i * timeDelta) / halflife);
		QDateTime checktime = QDateTime::fromSecsSinceEpoch(0);
		checktime = checktime.addSecs(timedel[i].key);
	}
	graphui.plot->graph()->data()->set(timedel);

	// Set date on x-axis
	QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
	dateTicker->setDateTimeFormat("d-MM-yyyy\nHH:mm");
	dateTicker->setTickCount(7);
	dateTicker->setTickOrigin(starttime);
	graphui.plot->xAxis->setTicker(dateTicker);
	graphui.plot->xAxis->setRange(starttime, endtime);
		

	// Set y-axix
	graphui.plot->yAxis->setRange(0, startactivity + 20);

	//Set graph Title
	graphui.plot->plotLayout()->insertRow(0);
	QCPTextElement* title = new QCPTextElement(graphui.plot, QString("Molybdenumbatch BatchID: ") + QString::number(moBatch), QFont("Times", 20));
	graphui.plot->plotLayout()->addElement(0, 0, title);

	// plot graph
	graphui.plot->replot();
		

}

void GraphUI::setTechnetiumGraph(int batchId, bool simGraph, bool latestBatch) {
	int moBatchId = db->GetdbInt("tebatchessim", "moBatchId", "teBatchId", std::to_string(batchId));
	qDebug() << moBatchId << "is mo batch idea";
	QList<QVariant> patientsTimeList;
	QList<QVariant> patientsDoseList;
	double startactivity;
	double rangeY;

	if (simGraph){
		patientsTimeList = db->GetAllRowsForValue("dosetopatientsim", "injectionTime", "teBatchId", std::to_string(batchId));
		patientsDoseList = db->GetAllRowsForValue("dosetopatientsim", "doseRadioactivity", "teBatchId", std::to_string(batchId));
		startactivity = db->GetdbInt("tebatchessim", "radioactivity", "teBatchId", std::to_string(batchId));
		rangeY = startactivity + 60;
		qDebug() << patientsDoseList << " = activity list";
		qDebug() << patientsTimeList;
	}
	else {
		qDebug() << "no db entry found for graph";
		return;
	}
	
	// Set date on x-axis
	QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
	QDateTime start = QDateTime::fromString(db->GetdbDateTime("tebatchessim", "dateTimeProduced", "teBatchId", std::to_string(batchId)), "yyyy-MM-ddTHH:mm:ss.zzz");
	QDateTime end = QDateTime(start.date(), patientsTimeList[patientsTimeList.count() - 1].toTime());
	double starttime = start.toSecsSinceEpoch();
	double endtime = end.toSecsSinceEpoch() + 3600;
	dateTicker->setDateTimeFormat("d-MM\nHH:mm");
	dateTicker->setTickOrigin(starttime);
	graphui.plot->xAxis->setTicker(dateTicker);
	QDateTime endOfDay = QDateTime(start.date(), QTime(17, 0));
	graphui.plot->xAxis->setRange(starttime, endOfDay.toSecsSinceEpoch()+ 15*60);


	// Collect data for graph
	int pointsPerBatch = 49;
	int plotpoints = patientsTimeList.count() * 50 + 100;
	double halflife = 6 * 3600;
	QVector<QCPGraphData> timedel(plotpoints);

	double starttimeInterval = starttime;
	double endtimeInterval;
	for (int i = 0; i < patientsTimeList.count(); ++i) {
		QDateTime endInterval = QDateTime(start.date(), patientsTimeList[i].toTime());
		endtimeInterval = endInterval.toSecsSinceEpoch();
		double timeDelta = (endtimeInterval - starttimeInterval) / pointsPerBatch;
 		for (int j = 0; j < 50; ++j) {
			timedel[i * 50 + j].key = starttimeInterval + j * timeDelta;
			timedel[i * 50 + j].value = startactivity * pow(0.5, ((j * timeDelta) / halflife));
			if (j == 49) {
				startactivity = startactivity * pow(0.5, ((j * timeDelta) / halflife));
				starttimeInterval = endtimeInterval;
			}
		}
		startactivity -= patientsDoseList[i].toDouble();
	}
	double endofdaytime = endOfDay.toSecsSinceEpoch();
	double timeDelta = (endofdaytime - starttimeInterval) / pointsPerBatch;
	int lastindex = patientsTimeList.count() * 50 - 1;
	qDebug() << "lastindex: " << lastindex;
	for (int i = 0; i < 50; ++i) {
		timedel[lastindex + i].key = starttimeInterval + i * timeDelta;
		timedel[lastindex + i].value = startactivity * pow(0.5, ((i * timeDelta) / halflife));
		if (i == 49) {
			timedel[lastindex + 50].key = endofdaytime;
			timedel[lastindex + 50].value = 0;
		}
	}
	graphui.plot->graph()->data()->set(timedel);


	// Set y-axix
	graphui.plot->yAxis->setRange(0, rangeY);

	//Set graph Title
	graphui.plot->plotLayout()->insertRow(0);
	QCPTextElement* title = new QCPTextElement(graphui.plot, QString("Techtnetium-99 BatchID: ") + QString::number(batchId), QFont("Times", 20));
	graphui.plot->plotLayout()->addElement(0, 0, title);

	// plot graph
	graphui.plot->replot();
	
}


QDateTime GraphUI::getTimeNow(bool simGraph) {
	QString lastPatientDateString;
	QString lastPatientTimeString;
	QString lastPatientDateTimeString;
	QDateTime lastPatientDateTime;
	QDateTime lastTeDateTime;
	QString lastTeDateTimeString;
	QString moBatchDateTimeString;
	QDateTime moBatchDateTime;

	if (!simGraph) {
		lastPatientDateString = db->LastDateString("dosetopatient", "injectionDate");
		lastPatientTimeString = db->LastTimeString("dosetopatient", "injectionTime");
		if (lastPatientDateString != "no val") {
			QDate lastPatientDate = lastPatientDate.fromString(lastPatientDateString, "yyyy-MM-dd");
			QTime lastPatientTime = lastPatientTime.fromString(lastPatientTimeString, "HH:mm:ss.zzz");
			lastPatientDateTime.setDate(lastPatientDate);
			lastPatientDateTime.setTime(lastPatientTime);
		}
		else {
			lastPatientDateTimeString = "no val";
		}
		lastTeDateTimeString = db->LastDateTime("tebatches", "dateTimeProduced");
		if (lastTeDateTimeString != "no val") {
			lastTeDateTime = lastTeDateTime.fromString(lastTeDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
		}

		moBatchDateTimeString = db->LastDateTime("batches", "dateTimeMeasured");
	}
	else {
		//Make graph from simulation data
		lastPatientDateString = db->LastDateString("dosetopatientsim", "injectionDate");
		lastPatientTimeString = db->LastTimeString("dosetopatientsim", "injectionTime");
		if (lastPatientDateString != "no val"){
			QDate lastPatientDate = lastPatientDate.fromString(lastPatientDateString, "yyyy-MM-dd");
			QTime lastPatientTime = lastPatientTime.fromString(lastPatientTimeString, "HH:mm:ss.zzz");
			lastPatientDateTime.setDate(lastPatientDate);
			lastPatientDateTime.setTime(lastPatientTime);
		}
		else {
			lastPatientDateTimeString = "no val";
		}
		lastTeDateTimeString = db->LastDateTime("tebatchessim", "dateTimeProduced");
		if (lastTeDateTimeString != "no val"){
			lastTeDateTime = lastTeDateTime.fromString(lastTeDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
		}
		moBatchDateTimeString = db->LastDateTime("batchessim", "dateTimeMeasured");

	}
	if (lastTeDateTimeString == "no val" && lastPatientDateTimeString == "no val") {
		moBatchDateTime = moBatchDateTime.fromString(moBatchDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
		moBatchDateTime = moBatchDateTime.addSecs(3600);
		return moBatchDateTime;
	}
	if (lastPatientDateTimeString == "no val") {
		return lastTeDateTime;
	}
	if (lastPatientDateTime > lastTeDateTime) {
		return lastPatientDateTime;
	}
	else {
		return lastTeDateTime;
	}
}


void GraphUI::selectMoBatch() {
	SelectTableDialog tableDialog = new SelectTableDialog(this);
	tableDialog.setModal(true);

	QSqlQueryModel* model = new QSqlQueryModel;
	if (graphui.simulationCheckBox->isChecked()) {
		model->setQuery("SELECT * FROM batchessim;");
	}
	else {
		model->setQuery("SELECT * FROM batches;");
	}
	//model->setQuery("SELECT * FROM tebatches;");
	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Molybdeen Batch ID"));
	model->setHeaderData(2, Qt::Horizontal, tr("Technetium Batch ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("Date/Time produced"));
	model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
	tableDialog.setModel(model);

	tableDialog.exec();
	if (tableDialog.getRowId() != NULL) {
		int batchId = 0;
		if (graphui.simulationCheckBox->isChecked()) {
			batchId = db->GetdbInt("batchessim", "batchId", "id", std::to_string(tableDialog.getRowId()));
		}
		else {
			batchId = db->GetdbInt("batches", "batchId", "id", std::to_string(tableDialog.getRowId()));
		}
		graphui.moLineEdit->setText(QString::number(batchId));
	}
	else {
		graphui.moLineEdit->setText("");
	}
}

void GraphUI::selectTeBatch() {
	SelectTableDialog tableDialog = new SelectTableDialog(this);
	tableDialog.setModal(true);

	QSqlQueryModel* model = new QSqlQueryModel;
	if (graphui.simulationCheckBox->isChecked()) {
		model->setQuery("SELECT * FROM tebatchessim;");
	}
	else {
		model->setQuery("SELECT * FROM tebatches;");
	}
	//model->setQuery("SELECT * FROM tebatches;");
	model->setHeaderData(0, Qt::Horizontal, tr("Row ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Molybdeen Batch ID"));
	model->setHeaderData(2, Qt::Horizontal, tr("Technetium Batch ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("Date/Time produced"));
	model->setHeaderData(4, Qt::Horizontal, tr("Radioactivity(GBq)"));
	tableDialog.setModel(model);

	tableDialog.exec();
	if (tableDialog.getRowId() != NULL) {
		int batchId = 0;
		if (graphui.simulationCheckBox->isChecked()) {
			batchId = db->GetdbInt("tebatchessim", "teBatchId", "id", std::to_string(tableDialog.getRowId()));
		}
		else {
			batchId = db->GetdbInt("tebatches", "teBatchId", "id", std::to_string(tableDialog.getRowId()));
		}
		graphui.teLineEdit->setText(QString::number(batchId));
	}
	else {
		graphui.teLineEdit->setText("");
	}
}


void GraphUI::resetInputs() {

}

void GraphUI::closeEvent(QCloseEvent* event) {
	GraphUI::Screen::closeEvent(event, this);
}

