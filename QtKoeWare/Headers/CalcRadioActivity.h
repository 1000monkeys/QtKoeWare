#pragma once

#include <QtUiTools>
#include "DataBase.h"

class RadioActivityCalculator {
private:

public:
	double elapsedHours(int BatchId);
	double molybdeenActivityFromDb(int BatchId);
	double technetiumActivityFromDb(int BatchId);
	int Random(int min, int max);
	double StartBatchWeekday();
	double StartBatchWeekend();
	double elapsedManually(QDateTime startTime, QDateTime measureTime);
	double moActivityManual(QDateTime startTime, QDateTime measureTime, double startActivity);
	double teActivityManual(QDateTime startTime, QDateTime measureTime, double startActivity);
	DataBase* db = new DataBase;
};