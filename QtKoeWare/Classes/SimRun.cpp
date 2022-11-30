#include "../Headers/SimRun.h"




QString SimRun::RandomLine(const std::string& filename, int Number) {
    std::ifstream in(filename.c_str());
    std::string s;
    s.reserve(35);
    for (int i = 0; i < Number; ++i) {
        std::getline(in, s);
    };
    std::getline(in, s);
    QString s2 = QString::fromStdString(s);
    return s2;
}

int SimRun::Random(int min, int max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double>dist(min, max);
    return int(dist(mt));
}

void SimRun::MolybdeenBatch() {
    int moBatchId = db->LastInt("batchessim", "batchId");
    moBatchId += Random(1, 14);

    QString moLastProduceDate = db->LastDateTime("batchessim", "dateTimeProduced");
    QDate moNewDate;
    // Generate Date (production)
    if (moLastProduceDate == "no val") {
        QDate produceDate = QDate::currentDate();
        int day = produceDate.dayOfWeek();
        moNewDate = produceDate.addDays(-(day - 1));
    }
    else {
        QDateTime produceDate = produceDate.fromString(moLastProduceDate, "yyyy-MM-ddTHH:mm:ss.zzz");
        QDate moLastDate = produceDate.date();
        moNewDate = moLastDate.addDays(7);
    }
    // Generate time (production)
    int hour = Random(0, 3);
    int minute;
    if (hour == 0) {
        minute = Random(30, 60);
    }
    else if (hour == 2) {

        minute = Random(0, 31);
    }
    else {
        minute = Random(0, 60);
    }
    QTime moNewTime;
    moNewTime.setHMS(hour, minute, 0);
    // Combine time and date in new datetime
    QDateTime moNewProduceDate;
    moNewProduceDate.setDate(moNewDate);
    moNewProduceDate.setTime(moNewTime);

    // Generate Production radioactivity
    int productionRadioactivity = Random(300, 901);

    // Measure Date same as production date
    QDate moMeasureDate = moNewDate;

    // Measure time differs
    QTime moMeasureTime;
    hour = Random(4, 7);
    if (hour == 6) {
        minute = Random(0, 61);
        if (minute == 60) {
            hour = 7;
            minute = 0;
        }
    }
    else {minute = Random(0, 60);}
    moMeasureTime.setHMS(hour, minute, 0);

    // Combine measure date time in DateTime object
    QDateTime moMeasureDateTime;
    moMeasureDateTime.setDate(moMeasureDate);
    moMeasureDateTime.setTime(moMeasureTime);

    //Calculate Measured Radioactivity
    double moMeasuredRadioactivity = raCalculator->moActivityManual(moNewProduceDate, moMeasureDateTime, productionRadioactivity);

    db->addMolybdeenBatchToSim(moBatchId, moNewProduceDate, moMeasureDateTime, moMeasuredRadioactivity);

}

void SimRun::FirstTeBatch() {
    int fromMoBatchId = db->LastInt("batchessim", "batchId");
    int tebatchId = 1;
    QString lastMoBatchDTstring = db->LastDateTime("batchessim", "dateTimeProduced");
    QDateTime lastMoBatchDT = lastMoBatchDT.fromString(lastMoBatchDTstring, "yyyy-MM-ddTHH:mm:ss.zzz");
    QDate teBatchDate = lastMoBatchDT.date();
    QTime teBatchTime;
    teBatchTime.setHMS(8, 0, 0);
    int errMarginMinutes = Random(-2, 3) * 60;
    teBatchTime = teBatchTime.addSecs(errMarginMinutes);

    QDateTime newTeBatchDt;
    newTeBatchDt.setDate(teBatchDate);
    newTeBatchDt.setTime(teBatchTime);
    int radioactivity = raCalculator->StartBatchWeekday();


    db->addTechnetiumBatchToSim(fromMoBatchId, tebatchId, newTeBatchDt, radioactivity);

}

void SimRun::TechnetiumBatch() {
    bool weekendday = false;
    int fromMoBatchId = db->LastInt("batchessim", "batchId");
    int teBatchId = db->LastInt("tebatchessim", "teBatchId");
    if (teBatchId == 0) {
        FirstTeBatch();
    }
    else {
        teBatchId += 1;
        QDate newTeBatchDate;
        QTime newTeBatchTime;
        QString lastTeBatchDateTimeString = db->LastDateTime("tebatchessim", "dateTimeProduced");
        QDateTime lastTeBatchDateTime = lastTeBatchDateTime.fromString(lastTeBatchDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
        QDate lastTeBatchDate = lastTeBatchDateTime.date();
        newTeBatchDate = lastTeBatchDate.addDays(1);

        if (newTeBatchDate.dayOfWeek() > 5) {
            weekendday = true;
        }

        newTeBatchTime.setHMS(8, 0, 0);
        int errMarginMinutes = Random(-2, 3) * 60;
        newTeBatchTime = newTeBatchTime.addSecs(errMarginMinutes);
        QDateTime newTeBatchDateTime;
        newTeBatchDateTime.setDate(newTeBatchDate);
        newTeBatchDateTime.setTime(newTeBatchTime);
        int radioactivity;
        if (weekendday) {
            radioactivity = raCalculator->StartBatchWeekend();
        }
        else {
            radioactivity = raCalculator->StartBatchWeekday();
        }
        db->addTechnetiumBatchToSim(fromMoBatchId, teBatchId, newTeBatchDateTime , radioactivity);
    }
}

void SimRun::AddDoseToPatient() {
    int patientId = db->LastInt("patientssim", "patientId");
    int teBatchId = db->LastInt("tebatchessim", "teBatchId");

    //Date administered is always the same as the last datetime of the te-99 batch
    QString lastTeBatchDateTimeString = db->LastDateTime("tebatchessim", "dateTimeProduced");
    QDate lastPatientDate= db->LastDate("dosetopatientsim", "injectionDate");
    QTime lastPatientTime = db->LastTime("dosetopatientsim", "injectionTime");

    QDateTime lastTeBatchDateTime = lastTeBatchDateTime.fromString(lastTeBatchDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
    

    QDate injectionDate = lastTeBatchDateTime.date();
    QTime injectionTime;
    


    if (lastTeBatchDateTime.date() == lastPatientDate) {
        injectionTime = lastPatientTime.addSecs((Random(45, 51) * 60));
    }
    else {
        injectionTime.setHMS(8, 30, 0);
        int errMarginMinutes = Random(-2, 3) * 60;
        injectionTime = injectionTime.addSecs(errMarginMinutes);
    }

    QDateTime injectionDateTime;
    injectionDateTime.setDate(injectionDate);
    injectionDateTime.setTime(injectionTime);
        
    int min_radioactiviyy = db->GetLatestSetting("min_dose_radioactivity");
    int max_radioactivity = db->GetLatestSetting("max_dose_radioactivity");

    int injectionDose = Random(min_radioactiviyy, max_radioactivity + 1);

    db->addDoseToPatientToSim(patientId, teBatchId, injectionDateTime, injectionDose);

}

bool SimRun::CheckLeapYear(int year) {
    if (year % 4 != 0) {
        return false;
    }
    else {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true;
            }
            return false;
        }
        return true;
    }
}

void SimRun::CreatePatient() {
    QString surname = RandomLine("surnames.txt", Random(0, 3003));
    QString lastname = RandomLine("lastnames.txt", Random(0, 320));
    QString name = surname + " " + lastname;
    int weight = Random(25, 175);
    int height = Random(80, 210);
    int sex = Random(0, 2);

    QDate birthdate;
    int year = Random(1920, 2011);
    int month = Random(1, 13);
    int day;
    if (month == 2) {
        if (CheckLeapYear(year)) {
            day = Random(1, 30);
        }
        else {
            day = Random(1, 29);
        }
    }
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        day = Random(1, 32);
    }
    else {
        day = Random(1, 31);
    }
    birthdate.setDate(year, month, day);
    db->addPatientToSim(name, height, weight, birthdate, sex);
}

void SimRun::Run(int daysToRun) {
    //Run the sim for daysToRun amount times. Every iteration is 1 day
    for (int i = 0; i < daysToRun; i++) {
        QString lastTeBatchDateTimeString = db->LastDateTime("tebatchessim", "dateTimeProduced");
        QString lastMoBatchDateTimeString = db->LastDateTime("batchessim", "dateTimeMeasured");

        QDateTime lastTeBatchDateTime = lastTeBatchDateTime.fromString(lastTeBatchDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");
        QDateTime lastMoBatchDateTime = lastMoBatchDateTime.fromString(lastMoBatchDateTimeString, "yyyy-MM-ddTHH:mm:ss.zzz");


        // Check if te-99 batch exist in db
        if (db->LastDateTime("tebatchessim", "dateTimeProduced") == "no val") {
            // if no te batch is found check for last mo batch in db
            if (db->LastDateTime("batchessim", "dateTimeMeasured") == "no val") {
                //no mo batch exists
                MolybdeenBatch();
            }
            TechnetiumBatch();
        }
        std::string lastTeBatchDateTimeStdString = lastTeBatchDateTime.toString("yyyy-MM-dd").toStdString();
        int patientsAdministered = db->CountRows("dosetopatientsim", "injectionDateTime", lastTeBatchDateTimeStdString);
        if (lastTeBatchDateTime.date().dayOfWeek() < 6) {
            while (patientsAdministered < 10) {
                CreatePatient();
                AddDoseToPatient();
                patientsAdministered++;
            }
            if (daysToRun - 1 != i) {
                TechnetiumBatch();
            }
            
        }
        else {
            while (patientsAdministered < 3) {
                CreatePatient();
                AddDoseToPatient();
                patientsAdministered++;
            }


            if (lastTeBatchDateTime.date().dayOfWeek() == 7) {
                QDate checkdate = lastTeBatchDateTime.date().addDays(1);
                if (lastMoBatchDateTime.date() != checkdate) {

                    MolybdeenBatch();
                }
            }
            if (daysToRun - 1 != i) {
                TechnetiumBatch();
            }
        }
    }
}