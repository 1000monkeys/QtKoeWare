#pragma once

#include"../Headers/GraphUi.h"



GraphUI::GraphUI(QMainWindow* parent)
	:QMainWindow(parent), Screen()
{
	setWindowTitle("KoeWare");
	setMinimumSize(750, 500);
	graphui.setupUi(this);
	int lastBatchId = db->LastInt("batches", "bathId");
	if (lastBatchId != 0) {
		setMolybdenumGraph(lastBatchId, false);
	}

	/*TEST*/
	int id = db->LastInt("batchessim", "batchId");
	setMolybdenumGraph(id, true);
	/*TEST*/
	
	
	
}

void GraphUI::setMolybdenumGraph(int moBatch, bool simGraph) {
	int startactivity = 0;
	QString measuredDateTimeString;
	graphui.plot->addGraph();
	if (!simGraph){
		measuredDateTimeString = db->LastDateTime("batches", "dateTimeMeasured");
		startactivity = db->LastInt("batches", "radioactivity");
		if (measuredDateTimeString == "no val") {
			return; 
		}
	}
	else {
		startactivity = db->LastInt("batchessim", "radioactivity");
		measuredDateTimeString = db->LastDateTime("batchessim", "dateTimeMeasured");
		if (measuredDateTimeString == "no val") {
			return;
		}
	}

	//Get start time and endtime. Starttime is measured time. Endtime is time of last activity (if no new mo batch)
	QDateTime measureDateTime = measureDateTime.fromString(measuredDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
	QDateTime timeNow = getTimeNow(true);
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

}

void GraphUI::selectTeBatch() {

}


void GraphUI::resetInputs() {

}

void GraphUI::closeEvent(QCloseEvent* event) {
	GraphUI::Screen::closeEvent(event, this);
}

