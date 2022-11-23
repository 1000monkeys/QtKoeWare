#pragma once
#include <math.h>
#include <fstream>
#include <random> 
#include "../Headers/DataBase.h"
#include "../Headers/CalcRadioActivity.h"

class SimRun
{
public:
	int Random(int min, int max);
	QString RandomLine(const std::string& filename, int Number);
	bool CheckLeapYear(int year);
	void MolybdeenBatch();
	void FirstTeBatch();
	void TechnetiumBatch();
	void Run(int dayToRun);
	void CreatePatient();
	void AddDoseToPatient();



	RadioActivityCalculator* raCalculator = new RadioActivityCalculator;
	DataBase* db = new DataBase;


};