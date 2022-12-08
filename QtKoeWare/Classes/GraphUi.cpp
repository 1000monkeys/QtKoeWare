#pragma once

#include"../Headers/GraphUi.h"
#include "../Headers/SelectTableDialog.h"



GraphUI::GraphUI(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);
	graphui.setupUi(this);

	// Insert row for title
	graphui.plot->plotLayout()->insertRow(0);
	QCPTextElement* title = new QCPTextElement(graphui.plot, QString("TITLE"), QFont("Times", 20));
	graphui.plot->plotLayout()->addElement(0, 0, title);
	graphui.plot->addGraph();

	// Connect buttons to functions
	connect(graphui.selectMoButton, &QPushButton::released, this, &GraphUI::selectMoBatch);
	connect(graphui.selectTeButton, &QPushButton::released, this, &GraphUI::selectTeBatch);

	setTechnetiumGraph(7, true, true);
	
}

void GraphUI::setMolybdenumGraph(int moBatch, bool simGraph, bool latestBatch, bool print) {
	int startactivity = 0;
	QString measuredDateTimeString;
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
	dateTicker->setTickOrigin(starttime);
	graphui.plot->xAxis->setTicker(dateTicker);
	graphui.plot->xAxis->setRange(starttime, endtime);
		
	//Title
	QString titleText = QString("Mo-99  BatchID: ") + QString::number(moBatch);
	setGraphTitle(titleText);

	// Set y-axix
	graphui.plot->yAxis->setRange(0, startactivity + 20);
	graphui.plot->yAxis->setLabel("Radioactivity(Gbq)");

	// plot graph
	graphui.plot->replot();

	if (print) {
		QString printTitle = QString("mo99_batchid_") + QString::number(moBatch);
		printGraphImage(printTitle);
	}
}

void GraphUI::printGraphImage(QString title) {
	graphui.plot->savePng(title+QString(".png"));
}

void GraphUI::setTechnetiumGraph(int batchId, bool simGraph, bool print) {
	int moBatchId;
	QList<QVariant> patientsTimeList;
	QList<QVariant> patientsDoseList;
	QDateTime start;
	double startactivity;
	double rangeY;


	if (simGraph){
		moBatchId = db->GetdbInt("tebatchessim", "moBatchId", "teBatchId", std::to_string(batchId));
		patientsTimeList = db->GetAllRowsForValue("dosetopatientsim", "injectionTime", "teBatchId", std::to_string(batchId));
		patientsDoseList = db->GetAllRowsForValue("dosetopatientsim", "doseRadioactivity", "teBatchId", std::to_string(batchId));
		startactivity = db->GetdbInt("tebatchessim", "radioactivity", "teBatchId", std::to_string(batchId));
		start = QDateTime::fromString(db->GetdbDateTime("tebatchessim", "dateTimeProduced", "teBatchId", std::to_string(batchId)), "yyyy-MM-ddTHH:mm:ss.zzz");
	}
	else {
		moBatchId = db->GetdbInt("tebatches", "moBatchId", "teBatchId", std::to_string(batchId));
		patientsTimeList = db->GetAllRowsForValue("dosetopatient", "injectionTime", "teBatchId", std::to_string(batchId));
		patientsDoseList = db->GetAllRowsForValue("dosetopatient", "doseRadioactivity", "teBatchId", std::to_string(batchId));
		startactivity = db->GetdbInt("tebatches", "radioactivity", "teBatchId", std::to_string(batchId));
		start = QDateTime::fromString(db->GetdbDateTime("tebatches", "dateTimeProduced", "teBatchId", std::to_string(batchId)), "yyyy-MM-ddTHH:mm:ss.zzz");
	}
	// set RangeY before changing startactivity val
	rangeY = startactivity + 60;
	
	// Set date on x-axis
	QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
	QDateTime end = QDateTime(start.date(), patientsTimeList[patientsTimeList.count() - 1].toTime());
	double starttime = start.toSecsSinceEpoch();
	double endtime = end.toSecsSinceEpoch() + 3600;
	dateTicker->setDateTimeFormat("d-MM\nHH:mm");
	dateTicker->setTickOrigin(starttime);
	graphui.plot->xAxis->setTicker(dateTicker);
	QDateTime endOfDay = QDateTime(start.date(), QTime(17, 0));
	QDateTime endOfWeekendDay = QDateTime(start.date(), QTime(13, 0));
	graphui.plot->xAxis->setRange(starttime, endOfDay.toSecsSinceEpoch()+ 15*60);

	// Collect data for graph
	int pointsPerBatch = 49;
	int plotpoints = patientsTimeList.count() * 50 + 100;
	double halflife = 6 * 3600;
	QVector<QCPGraphData> timedel(plotpoints);
	if (!patientsTimeList.isEmpty()) {
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
		// 10 items means full day we fill the rest of the graph till 1700.
		if (patientsTimeList.count() == 10) {
			double endofdaytime = endOfDay.toSecsSinceEpoch();
			double timeDelta = (endofdaytime - starttimeInterval) / pointsPerBatch;
			int lastindex = patientsTimeList.count() * 50 - 1;
			for (int i = 0; i < 50; ++i) {
				timedel[lastindex + i].key = starttimeInterval + i * timeDelta;
				timedel[lastindex + i].value = startactivity * pow(0.5, ((i * timeDelta) / halflife));
				if (i == 49) {
					timedel[lastindex + 50].key = endofdaytime;
					timedel[lastindex + 50].value = 0;
				}
			}
		}
		else if (patientsTimeList.count() == 3 && (start.date().dayOfWeek() == 6 or start.date().dayOfWeek() == 7)) {
			double endofweekendtime = endOfWeekendDay.toSecsSinceEpoch();
			double timeDelta = (endofweekendtime - starttimeInterval) / pointsPerBatch;
			int lastindex = patientsTimeList.count() * 50 - 1;
			for (int i = 0; i < 50; ++i) {
				timedel[lastindex + i].key = starttimeInterval + i * timeDelta;
				timedel[lastindex + i].value = startactivity * pow(0.5, ((i * timeDelta) / halflife));
				if (i == 49) {
					timedel[lastindex + 50].key = endofweekendtime;
					timedel[lastindex + 50].value = 0;
				}
			}
			graphui.plot->xAxis->setRange(starttime, endOfWeekendDay.toSecsSinceEpoch() + 15 * 60);
		}
	}
	else {
		// If te batch is made but no patients administered, we assume it's 8,20
		QDateTime startBeforePatient = QDateTime(start.date(), QTime(8, 20));
		double starttimeInterval = starttime;
		double endtimeInterval = startBeforePatient.toSecsSinceEpoch();
		double timeDelta = (endtimeInterval - starttimeInterval) / pointsPerBatch;
		for (int i = 0; i < 50; ++i) {
			timedel[i].key = starttimeInterval + i * timeDelta;
			timedel[i].value = startactivity * pow(0.5, ((i * timeDelta) / halflife));
			if (i == 49) {
				timedel[50].key = endtimeInterval;
				timedel[50].value = 0;
			}
		}
		graphui.plot->xAxis->setRange(starttime, startBeforePatient.toSecsSinceEpoch() + 15 * 60);
	}
	graphui.plot->graph()->data()->set(timedel);


	// Set y-axix
	graphui.plot->yAxis->setRange(0, rangeY);
	graphui.plot->yAxis->setLabel("Radioactivity(Mbq)");
	


	//Graph Title
	QString titleText = QString("Te-99  BatchID: ") + QString::number(batchId);
	setGraphTitle(titleText);


	//Make graph image
	if (print) {
		QString printTitle = QString("te99_batchid_") + QString::number(batchId);
		printGraphImage(printTitle);
	}
	

	// plot graph
	graphui.plot->replot();

}
void GraphUI::setGraphTitle(QString titleText) {
	graphui.plot->plotLayout()->remove(graphui.plot->plotLayout()->element(0, 0));
	QCPTextElement* title = new QCPTextElement(graphui.plot);
	title->setText(titleText);
	title->setFont(QFont("Times", 20));
	graphui.plot->plotLayout()->addElement(0, 0, title);
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
		int checkId = 0;
		if (graphui.simulationCheckBox->isChecked()) {
			batchId = db->GetdbInt("batchessim", "batchId", "id", std::to_string(tableDialog.getRowId()));
			checkId = db->LastInt("batchessim", "batchId");
			if (checkId == batchId) {
				setMolybdenumGraph(batchId, true, true);
			}
			else {
				setMolybdenumGraph(batchId, true, false);
			}
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
			setTechnetiumGraph(batchId, true);

		}
		else {
			batchId = db->GetdbInt("tebatches", "teBatchId", "id", std::to_string(tableDialog.getRowId()));
			setTechnetiumGraph(batchId, false);
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

