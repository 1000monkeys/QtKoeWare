#include "../Headers/CalcRadioActivity.h"
#include <string>
#include <iostream>
#include <QtDebug>
#include <math.h>



double RadioActivityCalculator::elapsedHours(int BatchId) {
	std::string Batch = std::to_string(BatchId);
	QDateTime startDt = db->GetdbDateTime("batches", "dateTimeMeasured", "batchId", Batch);
	QDateTime currdt = QDateTime::currentDateTime();
	QTime currtime = QTime::currentTime();
	double elapsed_days = startDt.daysTo(currdt) * 24;
	double div_fact = 3600000;
	double elapsed_hours = startDt.time().msecsTo(currtime) / div_fact;
	double elapsed = elapsed_days + elapsed_hours;
	return elapsed;
}

double RadioActivityCalculator::molybdeenActivityFromDb(int BatchId) {
	// Half life is in hours
	double halflife = 66;
	std::string Batch = std::to_string(BatchId);
	double startActivity = db->GetdbInt("batches", "radioactivity", "batchId", Batch);
	double elapsed_hours = elapsedHours(BatchId);
	double currActivity = startActivity * pow(0.5, (elapsed_hours / halflife));
	if (currActivity > 0) { return currActivity; }
	else { return -1; }
}

double RadioActivityCalculator::technetiumActivityFromDb(int BatchId) {
	// Half life is in hours
	double halflife = 6;
	std::string Batch = std::to_string(BatchId);
	double startActivity = db->GetdbInt("batches", "radioactivity", "batchId", Batch);
	double elapsed_hours = elapsedHours(BatchId);
	double currActivity = startActivity * pow(0.5, (elapsed_hours / halflife));
	if (currActivity > 0) { return currActivity; }
	else { return -1; }
}

double RadioActivityCalculator::elapsedManually(QDateTime starttime, QDateTime measureTime) {
	double elapsed_days = starttime.daysTo(measureTime) * 24;
	double div_fact = 3600000;
	double elapsed_hours = starttime.time().msecsTo(measureTime.time()) / div_fact;
	double elapsed = elapsed_days + elapsed_hours;
	return elapsed;
}

double RadioActivityCalculator::moActivityManual(QDateTime starttime, QDateTime measureTime, double startActivity) {
	double halflife = 66;
	double elapsed_hours = elapsedManually(starttime, measureTime);
	double currActivity = startActivity * pow(0.5, (elapsed_hours / halflife));
	if (currActivity > 0) { return currActivity; }
	else { return -1; }
}

double RadioActivityCalculator::teActivityManual(QDateTime starttime, QDateTime measureTime, double startActivity) {
	double halflife = 6;
	double elapsed_hours = elapsedManually(starttime, measureTime);
	double currActivity = startActivity * pow(0.5, (elapsed_hours / halflife));
	if (currActivity > 0) { return currActivity; }
	else { return -1; }
}

int RadioActivityCalculator::Random(int min, int max) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(min, max);
	return int(dist(mt));
}

double RadioActivityCalculator::StartBatchWeekday() {
	// Calculate how many Te we need for one day (10 dosis)
	double start_val = 0;
	double minutesBetween = -50;
	double minutesInHour = 60;
	for (int i = 0; i < 10; ++i) {
		start_val = start_val * pow(0.5, ((double(-50) / double(60)) / double(6))) + double(800);
	}
	start_val = start_val * pow(0.5, ((double(-30) / double(60)) / double(6)));
	double ErrorMargin = Random(-2, 3);
	while (ErrorMargin == 0) {
		ErrorMargin = Random(-2, 3);
	}
	double correctionVal = double(100) - ErrorMargin;
	correctionVal = correctionVal / double(100);
	start_val = start_val * correctionVal;
	return start_val;
}

double RadioActivityCalculator::StartBatchWeekend() {
	double start_val = 0;
	for (int i = 0; i < 3; i++) {
		start_val = start_val * pow(0.5, ((double(-50) / double(60)) / double(6))) + double(800);
	}
	start_val = start_val * pow(0.5, ((double(-30) / double(60)) / double(6)));
	double ErrorMargin = Random(-2, 3);
	while (ErrorMargin == 0) {
		ErrorMargin = Random(-2, 3);
	}
	double correctionVal = double(100) - ErrorMargin;
	correctionVal = correctionVal / double(100);
	start_val = start_val * correctionVal;
	return start_val;
}
