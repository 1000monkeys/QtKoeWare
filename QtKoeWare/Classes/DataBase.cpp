#include "../Headers/DataBase.h"

#include <QtSql>
#include <QSqlError>
#include <QtDebug>

QSqlError DataBase::initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("koewareDB.sqlite");
    db.setUserName("root");
    db.setPassword("");

    if (!db.open()) {
        qDebug() << "0";
        return db.lastError();
    }
    else {
        qDebug() << "opened";
    }
    QSqlQuery q;
    QStringList tables = db.tables();
    if (!tables.contains("batches", Qt::CaseInsensitive)) {
        qDebug() << "batches table nonexistant";
        if (!q.exec(BATCH_SQL)) {
            qDebug() << "created batches table";
            return q.lastError();
        }
        else {
            qDebug() << "1c";
        }
    }
    else {
        qDebug() << "1";
    }

    if (!tables.contains("tebatches", Qt::CaseInsensitive)) {
        qDebug() << "tebatches table nonexistant";
        if (!q.exec(TE_BATCH_SQL)) {
            qDebug() << "created tebatches table";
            return q.lastError();
        }
        else {
            qDebug() << "2c";
        }
    }
    else {
        qDebug() << "2";
    };

    if (!tables.contains("patients", Qt::CaseInsensitive)) {
        qDebug() << "patients table nonexistant";
        if (!q.exec(PATIENT_SQL)) {
            qDebug() << "created patients table";
            return q.lastError();
        }
        else {
            qDebug() << "3c";
        }
    }
    else {
        qDebug() << "3";
    };

    if (!tables.contains("dosetopatient", Qt::CaseInsensitive)) {
        qDebug() << "dosetopatient table nonexistant";
        if (!q.exec(DOSE_TO_PATIENT_SQL)) {
            qDebug() << "created dosetopatient table";
            return q.lastError();
        }
        else {
            qDebug() << "4c";
        }
    }
    else {
        qDebug() << "4";
    };

    if (!tables.contains("settings", Qt::CaseInsensitive)) {
        qDebug() << "settings table nonexistant";
        if (!q.exec(SETTINGS_SQL)) {
            qDebug() << "created settings table";
            return q.lastError();
        }
        else {
            qDebug() << "5c";
        }
    }
    else {
        qDebug() << "5";
    };

    //From here init simulation tables
    if (!tables.contains("batchessim", Qt::CaseInsensitive)) {
        qDebug() << "batches table nonexistant";
        if (!q.exec(BATCH_SQL_SIM)) {
            qDebug() << "can't create batchessim table";
            return q.lastError();
        }
        else {
            qDebug() << "bathces sim table created";
        }
    }
    else {
        qDebug() << "batches sim table already existant";
    };

    if (!tables.contains("tebatchessim", Qt::CaseInsensitive)) {
        qDebug() << "tebatches table nonexistant";
        if (!q.exec(TE_BATCH_SQL_SIM)) {
            qDebug() << "can't create tebatchessim table";
            return q.lastError();
        }
        else {
            qDebug() << "tebathces sim table created";
        }
    }
    else {
        qDebug() << "tebatches sim table already existant";
    };

    if (!tables.contains("patientssim", Qt::CaseInsensitive)) {
        qDebug() << "patients sim table nonexistant";
        if (!q.exec(PATIENT_SQL_SIM)) {
            qDebug() << "can't create patientssim table";
            return q.lastError();
        }
        else {
            qDebug() << "patients sim table created";
        }
    }
    else {
        qDebug() << "patients sim table already existant";
    };

    if (!tables.contains("dosetopatientsim", Qt::CaseInsensitive)) {
        qDebug() << "dosetopatientsim table nonexistant";
        if (!q.exec(DOSE_TO_PATIENT_SQL_SIM)) {
            qDebug() << "can't create dosetopatientsim table";
            return q.lastError();
        }
        else {
            qDebug() << "dosetopatient sim table created";
        }
    }
    else {
        qDebug() << "dosetopatient sim table already existant";
    };


    return QSqlError();
}

std::map<std::string, std::string> DataBase::getDefaultSettings() {
    std::map<std::string, std::string> settings;

    settings["min_te_radioactivity"] = "100";
    settings["max_te_radioactivity"] = "200";

    settings["min_name_length"] = "1";
    settings["short_name_length"] = "3";
    settings["really_long_name_length"] = "20";

    settings["min_height"] = "1";
    settings["really_really_short_height"] = "10";
    settings["really_short_height"] = "100";
    settings["really_tall_height"] = "250";
    settings["really_really_tall_height"] = "100";

    settings["min_weight"] = "1";
    settings["really_really_light_weigth"] = "10";
    settings["really_light_weigth"] = "25";
    settings["really_heavy_weigth"] = "200";
    settings["really_really_heavy_weigth"] = "1000";

    settings["min_mo_radioactivity"] = "300";
    settings["max_mo_radioactivity"] = "700";

    settings["min_dose_radioactivity"] = "300";
    settings["max_dose_radioactivity"] = "700";

    settings["secs_time_offset"] = "3600";

    settings["min_te_production_time"] = "3";
    settings["max_te_production_time"] = "7";

    settings["min_mo_production_time"] = "3";
    settings["max_mo_production_time"] = "7";

    return settings;
}

std::map<std::string, std::string> DataBase::getSettings()
{
    std::map<std::string, std::string> settings;
    std::map<std::string, std::string> defaultSettings = getDefaultSettings();

    std::vector<std::string> keys;
    for (const auto& [k, v] : defaultSettings) {
        keys.push_back(k);
    }

    for (const auto& key : keys) {
        QSqlQuery q;
        qDebug() << QString::fromStdString(key);
        if (q.exec(
            QLatin1String("SELECT * FROM settings WHERE name='") +
            QLatin1String(key) +
            QLatin1String("' ORDER BY setDateTime DESC LIMIT 1;")
        )) {
            qDebug() << "selected settings";
        }
        else {
            qDebug() << "not selected 12375612746";
        }
        while (q.next()) {
            settings[q.value("name").toString().toStdString()] = q.value("value").toString().toStdString();

            QString Values = "name: " + q.value("name").toString() +
                " & value: " + q.value("value").toString();
            qDebug() << Values;
        }
    }


    for (const auto& [k, v] : defaultSettings) {
        if (settings.count(std::string(k)) == 0) {
            settings[k] = defaultSettings[k];
            
            QSqlQuery q;
            if (!q.prepare(INSERT_SETTINGS_SQL)) {
                qDebug() << "No mo settings table found";
                //return q.lastError();
            }
            else {
                //qDebug() << "5c";
            }

            q.addBindValue(QDateTime::currentDateTime());
            q.addBindValue(QString::fromStdString(k));
            q.addBindValue(QString::fromStdString(defaultSettings[k]));
            q.exec();
            qDebug() << "INSERTED SETTING " << QString::fromStdString(k);
        }
    }

    return settings;
}

QVariant DataBase::addMolybdeenBatch(const int& batchid, const QDateTime& dateTimeProduced, const QDateTime& dateTimeMeasured, const int& radioactivity)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_BATCH_SQL)) {
        qDebug() << "No mo batch table found";
        //return q.lastError();
    }
    else {
        //qDebug() << "5c";
    }

    q.addBindValue(batchid);
    q.addBindValue(dateTimeProduced);
    q.addBindValue(dateTimeMeasured);
    q.addBindValue(radioactivity);
    q.exec();
    return q.lastInsertId();
}
// Simulation Table
QVariant DataBase::addMolybdeenBatchToSim(const int& batchid, const QDateTime& dateTimeProduced, const QDateTime& dateTimeMeasured, const int& radioactivity)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_BATCH_SQL_SIM)) {
        qDebug() << "No mo batch sim table found";
        //return q.lastError();
    }
    else {
        //qDebug() << "5c";
    }

    q.addBindValue(batchid);
    q.addBindValue(dateTimeProduced);
    q.addBindValue(dateTimeMeasured);
    q.addBindValue(radioactivity);
    q.exec();
    return q.lastInsertId();
}


QVariant DataBase::addTechnetiumBatch(const int& moBatchId, const int& teBatchId, QDateTime& dateTimeProduced, const int& Radioactivity) {
    QSqlQuery q;

    if (!q.prepare(INSERT_TEBATCH_SQL)) {
        qDebug() << "No te batch table found";
        //return q.lastError();
    }
    else {
        //qDebug() << "5c";
    }

    q.addBindValue(moBatchId);
    q.addBindValue(teBatchId);
    q.addBindValue(dateTimeProduced);
    q.addBindValue(Radioactivity);
    q.exec();
    return q.lastInsertId();
}
//Simulation Table
QVariant DataBase::addTechnetiumBatchToSim(const int& moBatchId, const int& teBatchId, QDateTime& dateTimeProduced, const int& Radioactivity) {
    QSqlQuery q;

    if (!q.prepare(INSERT_TEBATCH_SQL_SIM)) {
        qDebug() << "No te batch sim table found";
        //return q.lastError();
    }
    else {
        //qDebug() << "5c";
    }

    q.addBindValue(moBatchId);
    q.addBindValue(teBatchId);
    q.addBindValue(dateTimeProduced);
    q.addBindValue(Radioactivity);
    q.exec();
    return q.lastInsertId();
}

QVariant DataBase::addPatient(const QString& name, const int& length, const int& weight, const QDate& birthdate, const int& sex)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_PATIENT_SQL)) {
        qDebug() << "6";
        //return q.lastError();
    }
    else {
        //qDebug() << "6c";
    }
    q.addBindValue(name);
    q.addBindValue(length);
    q.addBindValue(weight);
    q.addBindValue(birthdate);
    q.addBindValue(sex);
    q.exec();

    // qDebug() << q.lastInsertId();

    return q.lastInsertId();
}

//Simulation table
QVariant DataBase::addPatientToSim(const QString& name, const int& length, const int& weight, const QDate& birthdate, const int& sex)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_PATIENT_SQL_SIM)) {
        qDebug() << "6";
        //return q.lastError();
    }
    else {
        //qDebug() << "6c";
    }
    q.addBindValue(name);
    q.addBindValue(length);
    q.addBindValue(weight);
    q.addBindValue(birthdate);
    q.addBindValue(sex);
    q.exec();

    // qDebug() << q.lastInsertId();

    return q.lastInsertId();
}

QVariant DataBase::addDoseToPatient(const int& patientId, const int& teBatchId, QDateTime&injectionDateTime, const int& doseRadioactivity) {
    QSqlQuery q;

    if (!q.prepare(INSERT_DOSE_TO_PATIENT_SQL)) {
        qDebug() << "Dose To Patient table doesn't exist";
    }

    q.addBindValue(patientId);
    q.addBindValue(teBatchId);
    q.addBindValue(injectionDateTime.date());
    q.addBindValue(injectionDateTime.time());
    q.addBindValue(doseRadioactivity);
    q.exec();

    return q.lastInsertId();
}
//Simulation table
QVariant DataBase::addDoseToPatientToSim(const int& patientId, const int& teBatchId, QDateTime& injectionDateTime, const int& doseRadioactivity) {
    QSqlQuery q;

    if (!q.prepare(INSERT_DOSE_TO_PATIENT_SQL_SIM)) {
        qDebug() << "Dose To Patient table doesn't exist";
    }

    q.addBindValue(patientId);
    q.addBindValue(teBatchId);
    q.addBindValue(injectionDateTime.date());
    q.addBindValue(injectionDateTime.time());
    q.addBindValue(doseRadioactivity);
    q.exec();

    return q.lastInsertId();
}

void DataBase::printDB() {
    QSqlQuery q;
    qDebug() << "Got here!!";
    q.exec("use tre");
    if (q.exec("SELECT * FROM batches")) {
        qDebug() << "selected batches";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "id: " + q.value("id").toString() + 
            " & BatchID: " + q.value("batchId").toString() +
            " & Dtp: " + q.value("dateTimeProduced").toString() +
            " & dtm: " + q.value("dateTimeMeasured").toString() +
            " & ra: " + q.value("radioactivity").toString();

        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM tebatches")) {
        qDebug() << "selected tebatches";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "id: " + q.value("id").toString() +
            " & moBatchId: " + q.value("moBatchId").toString() +
            " & teBatchId: " + q.value("teBatchId").toString() +
            " & dateTimeProduced: " + q.value("dateTimeProduced").toString() +
            " & radioactivity: " + q.value("radioactivity").toString();
        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM patients")) {
        qDebug() << "selected patients";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "Name: " + q.value("name").toString() +
            " & length: " + q.value("length").toString() +
            " & weight: " + q.value("weight").toString() +
            " & birthdate: " + q.value("birthdate").toString() +
            " & sex: " + q.value("sex").toString();
        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM dosetopatient")) {
        qDebug() << "selected dosetopatient";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "patientId: " + q.value("patientId").toString() +
            " & teBatchId: " + q.value("teBatchId").toString() +
            " & dateInjection: " + q.value("injectionDate").toString() +
            " & timeInjection: " + q.value("injectionTime").toString() +
            " & radioactivity: " + q.value("doseRadioactivity").toString();
        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM settings")) {
        qDebug() << "selected settings";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "id: " + q.value("id").toString() +
            " & setDateTime: " + q.value("setDateTime").toString() +
            " & name: " + q.value("name").toString() +
            " & value: " + q.value("value").toString();
        qDebug() << Values;
    }
}

void DataBase::printSimDb() {
    QSqlQuery q;
    qDebug() << "Got here!!";
    q.exec("use tre");
    if (q.exec("SELECT * FROM batchessim")) {
        qDebug() << "selected batchessim";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "id: " + q.value("id").toString() +
            " & BatchID: " + q.value("batchId").toString() +
            " & Dtp: " + q.value("dateTimeProduced").toString() +
            " & dtm: " + q.value("dateTimeMeasured").toString() +
            " & ra: " + q.value("radioactivity").toString();

        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM tebatchessim")) {
        qDebug() << "selected tebatchessim";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "id: " + q.value("id").toString() +
            " & moBatchId: " + q.value("moBatchId").toString() +
            " & teBatchId: " + q.value("teBatchId").toString() +
            " & dateTimeProduced: " + q.value("dateTimeProduced").toString() +
            " & radioactivity: " + q.value("radioactivity").toString();
        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM patientssim")) {
        qDebug() << "selected patientssim";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "Name: " + q.value("name").toString() +
            " & length: " + q.value("length").toString() +
            " & weight: " + q.value("weight").toString() +
            " & birthdate: " + q.value("birthdate").toString() +
            " & sex: " + q.value("sex").toString();
        qDebug() << Values;
    }

    if (q.exec("SELECT * FROM dosetopatientsim")) {
        qDebug() << "selected dosetopatientsim";
    }
    else {
        qDebug() << "not selected";
    }
    while (q.next()) {
        QString Values = "patientId: " + q.value("patientId").toString() +
            " & teBatchId: " + q.value("teBatchId").toString() +
            " & dateInjection: " + q.value("injectionDate").toString() +
            " & timeInjection: " + q.value("injectionTime").toString() +
            " & radioactivity: " + q.value("doseRadioactivity").toString();
        qDebug() << Values;
    }
}

QDate DataBase::LastDate(std::string table, std::string value) {
    const auto FIND_DATE_SQL = QLatin1String("SELECT ") +
        QLatin1String(value) + 
        QLatin1String(" FROM ") + 
        QLatin1String(table) + 
        QLatin1String(" WHERE id=(SELECT max(id) FROM ") + 
        QLatin1String(table) + 
        QLatin1String(");");

    QSqlQuery q;
    q.prepare(FIND_DATE_SQL);
    q.exec();
    while (q.next()) {
        QDate dt = q.value(QLatin1String(value)).toDate();
        return dt;
    }
    QDate date = QDate::currentDate();
    return date;
}

int DataBase::LastInt(std::string table, std::string value) {
    const auto FIND_ID_SQL = QLatin1String("SELECT MAX(") +
        QLatin1String(value) +
        QLatin1String(") AS ") +
        QLatin1String(value) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(";");
    int dt;
    QSqlQuery q;
    q.prepare(FIND_ID_SQL);
    q.exec();
    while (q.next()) {
        dt = q.value(QLatin1String(value)).toInt();
        return dt;
    }
    return NULL;
}

QTime DataBase::LastTime(std::string table, std::string value) {
    QTime time;
    const auto FIND_TIME_SQL = QLatin1String("SELECT ") + 
        QLatin1String(value) + 
        QLatin1String(" FROM ") + 
        QLatin1String(table) + 
        QLatin1String(" WHERE id=(SELECT max(id) FROM ") + 
        QLatin1String(table) + 
        QLatin1String(");");
    QSqlQuery q;
    q.prepare(FIND_TIME_SQL);
    q.exec();
    while (q.next()) {
        time = q.value(QLatin1String(value)).toTime();
        return time;
    }
    time = QTime::currentTime();
    return time;
}

QString DataBase::LastDateTime(std::string table, std::string value) {
    QString dt;
    const auto FIND_LAST_DT_SQL = QLatin1String("SELECT ") +
        QLatin1String(value) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE id=(SELECT max(id) FROM ") +
        QLatin1String(table) +
        QLatin1String(");");
    QSqlQuery q;
    q.prepare(FIND_LAST_DT_SQL);
    q.exec();
    while (q.next()) {
        dt = q.value(QLatin1String(value)).toString();
        return dt;
    }
    dt = "no val";
    return dt;
}

QString DataBase::LastString(std::string table, std::string value) {
    QString s;
    const auto FIND_STRING_SQL = QLatin1String("SELECT ") +
        QLatin1String(value) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE id=(SELECT max(id) FROM ") +
        QLatin1String(table) +
        QLatin1String(");");
    QSqlQuery q;
    q.prepare(FIND_STRING_SQL);
    q.exec();
    while (q.next()) {
        s = q.value(QLatin1String(value)).toString();
        return s;
    }
    s = "Last string not found";
    return s;
}

int DataBase::GetdbInt(std::string table, std::string value, std::string known_index, std::string known_value) {
    int val;
    const auto FIND_INT_SQL = QLatin1String("SELECT ") +
        QLatin1String(value) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE ") +
        QLatin1String(known_index) +
        QLatin1String("= ") +
        QLatin1String(known_value) +
        QLatin1String(";");


    QSqlQuery q;
    q.prepare(FIND_INT_SQL);

    q.exec();
    while (q.next()) {
        val = q.value(QLatin1String(value)).toInt();
        return val;
    }
    val = -1;
    return val;
}

int DataBase::GetLatestSetting(std::string setting_name) {
    int val;
    const auto FIND_SETTING_SQL =
        QLatin1String("SELECT value FROM settings WHERE name= '") +
        QLatin1String(setting_name) +
        QLatin1String("' ORDER BY setDateTime DESC LIMIT 1;");


    QSqlQuery q;
    q.prepare(FIND_SETTING_SQL);
    q.exec();
    while (q.next()) {
        val = q.value(QLatin1String("value")).toInt();
        return val;
    }
    val = -1;
    return val;
}

QDateTime DataBase::GetdbDateTime(std::string table, std::string value, std::string known_index, std::string known_value) {
    QDateTime dt;
    const auto FIND_DT_SQL = QLatin1String("SELECT ") +
        QLatin1String(value) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE ") +
        QLatin1String(known_index) +
        QLatin1String("= ") +
        QLatin1String(known_value) +
        QLatin1String(";");

    QSqlQuery q;
    q.prepare(FIND_DT_SQL);
    q.exec();
    while (q.next()) {
        dt = q.value(QLatin1String(value)).toDateTime();
        return dt;
    }
    dt = QDateTime::currentDateTime();
    return dt;
}

QList<QList<QVariant>> DataBase::GetWhere(std::string table, QList<std::string> values, std::string known_index, std::string known_value) {
    std::string valueString = "";
    for (const std::string temp : values) {
        valueString += temp + ",";
    }
    valueString = valueString.substr(0, valueString.length() - 1);
    qDebug() << QString::fromStdString(valueString);

    const auto FIND_SQL = QLatin1String("SELECT ") +
        QLatin1String(valueString) +
        QLatin1String(" FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE ") +
        QLatin1String(known_index) +
        QLatin1String("= ") +
        QLatin1String(known_value) +
        QLatin1String(";");
    QSqlQuery q;
    q.prepare(FIND_SQL);
    q.exec();

    printDB();

    QList<QList<QVariant>> variantRowList;
    while (q.next()) {
        qDebug() << "NEXT";
        qDebug() << values.size();
        QList<QVariant> currentRowList;
        for (int i = 0; i < values.size(); i++) {
            qDebug() << i;
            currentRowList.append(q.value(QLatin1String(values[i])).toInt());
        }
        variantRowList.append(currentRowList);
    }
    qDebug() << variantRowList;
    return variantRowList;
}

int DataBase::CountRows(std::string table, std::string column, std::string check_value) {
    const auto COUNT_ROWS = 
        QLatin1String("SELECT COUNT(*) FROM ") +
        QLatin1String(table) +
        QLatin1String(" WHERE ") +
        QLatin1String(column) +
        QLatin1String("= '") +
        QLatin1String(check_value) +
        QLatin1String("';");

    QSqlQuery q;

    q.prepare(COUNT_ROWS);
    q.exec();

    int rows = 0;
    if (q.next()) {
        rows = q.value(0).toInt();
    }

    return rows;
}

void DataBase::DeleteTableContents(std::string table) {
    const auto DELETE_TABLE =
        QLatin1String("DELETE FROM ") +
        QLatin1String(table) +
        QLatin1String(";");

    QSqlQuery q;
    q.prepare(DELETE_TABLE);
    q.exec();
}


void DataBase::showError(QWidget* parent, const QSqlError& err)
{
    QMessageBox::critical(parent, "Unable to initialize Database", "Error initializing database: " + err.text());
}
