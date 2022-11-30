#pragma once

#include <QtSql>
#include <QtDebug>
#include <QSqlError>
#include <string>
#include <QMainWindow>
#include <QMessageBox>
#include <vector>


const auto BATCH_SQL = QLatin1String(R"(
    create table batches(id integer primary key, batchId varchar, dateTimeProduced datetime, dateTimeMeasured datetime,
                       radioactivity integer)
    )");

const auto PATIENT_SQL = QLatin1String(R"(
    create table patients(patientId integer primary key, name varchar, length integer,
                        weight integer, birthdate date, sex integer)
    )");

const auto TE_BATCH_SQL = QLatin1String(R"(
    create table tebatches(id integer primary key, moBatchId integer, teBatchId int, dateTimeProduced datetime, radioactivity integer)
    )");

const auto DOSE_TO_PATIENT_SQL = QLatin1String(R"(
    create table dosetopatient(id integer primary key, patientId integer, teBatchId int, injectionDate date, injectionTime time, doseRadioactivity integer)
    )");

const auto SETTINGS_SQL = QLatin1String(R"(
    create table settings(id integer primary key, setDateTime datetime, name varchar, value varchar)
    )");



const auto INSERT_BATCH_SQL = QLatin1String(R"(
    insert into batches(batchId, dateTimeProduced, dateTimeMeasured, radioactivity) values(?, ?, ?, ?)
    )");

const auto INSERT_PATIENT_SQL = QLatin1String(R"(
    insert into patients(name, length, weight, birthdate, sex) values(?, ?, ?, ?, ?)
    )");

const auto INSERT_TEBATCH_SQL = QLatin1String(R"(
    insert into tebatches(moBatchId, teBatchId, dateTimeProduced, radioactivity) values(?, ?, ?, ?)
    )");

const auto INSERT_DOSE_TO_PATIENT_SQL = QLatin1String(R"(
    insert into dosetopatient(patientId, teBatchId, injectionDate, injectionTime, doseRadioactivity) values(?, ?, ?, ?, ?)
    )");

const auto INSERT_SETTINGS_SQL = QLatin1String(R"(
    insert into settings(setDateTime, name, value) values(?, ?, ?)
    )");





// Create sim tables

const auto BATCH_SQL_SIM = QLatin1String(R"(
    create table batchessim(id integer primary key, batchId varchar, dateTimeProduced datetime, dateTimeMeasured datetime,
                       radioactivity integer)
    )");

const auto PATIENT_SQL_SIM = QLatin1String(R"(
    create table patientssim(patientId integer primary key, name varchar, length integer,
                        weight integer, birthdate date, sex integer)
    )");

const auto TE_BATCH_SQL_SIM = QLatin1String(R"(
    create table tebatchessim(id integer primary key, moBatchId integer, teBatchId int, dateTimeProduced datetime, radioactivity integer)
    )");

const auto DOSE_TO_PATIENT_SQL_SIM = QLatin1String(R"(
    create table dosetopatientsim(id integer primary key, patientId integer, teBatchId int, injectionDate date, injectionTime time, doseRadioactivity integer)
    )");



// Insert in sim tables

const auto INSERT_BATCH_SQL_SIM = QLatin1String(R"(
    insert into batchessim(batchId, dateTimeProduced, dateTimeMeasured, radioactivity) values(?, ?, ?, ?)
    )");

const auto INSERT_PATIENT_SQL_SIM = QLatin1String(R"(
    insert into patientssim(name, length, weight, birthdate, sex) values(?, ?, ?, ?, ?)
    )");

const auto INSERT_TEBATCH_SQL_SIM = QLatin1String(R"(
    insert into tebatchessim(moBatchId, teBatchId, dateTimeProduced, radioactivity) values(?, ?, ?, ?)
    )");

const auto INSERT_DOSE_TO_PATIENT_SQL_SIM = QLatin1String(R"(
    insert into dosetopatientsim(patientId, teBatchId, injectionDate, injectionTime, doseRadioactivity) values(?, ?, ?, ?, ?)
    )");

class DataBase {

public:
    void printDB();
    void showError(QWidget* parent, const QSqlError& err);

    QVariant addPatient(const QString& name, const int& length, const int& weight, const QDate& birthdate, const int& sex);
    QVariant addMolybdeenBatch(const int& batchid, const QDateTime& dateTimeProduced, const QDateTime& dateTimeMeasured, const int& radioactivity);
    QVariant addTechnetiumBatch(const int& moBatchId, const int& teBatchId, QDateTime& dateTimeProduced, const int& Radioactivity);
    QVariant addDoseToPatient(const int& patientID, const int& teBatchId, QDateTime& injectionDateTime, const int& doseRadioactivity);
    QSqlError initDb();
    std::map<std::string, std::string> getDefaultSettings();
    std::map<std::string, std::string> getSettings();

    // All simualation tables
    QVariant addPatientToSim(const QString& name, const int& length, const int& weight, const QDate& birthdate, const int& sex);
    QVariant addMolybdeenBatchToSim(const int& batchid, const QDateTime& dateTimeProduced, const QDateTime& dateTimeMeasured, const int& radioactivity);
    QVariant addTechnetiumBatchToSim(const int& moBatchId, const int& teBatchId, QDateTime& dateTimeProduced, const int& Radioactivity);
    QVariant addDoseToPatientToSim(const int& patientID, const int& teBatchId, QDateTime& injectionDateTime, const int& doseRadioactivity);
    void printSimDb();

    QDate LastDate(std::string table, std::string value);
    int LastInt(std::string table, std::string value);
    QTime LastTime(std::string table, std::string value);
    QString LastString(std::string table, std::string value);
    std::vector<std::string> AllRowsFromColumn(std::string table, std::string column);
    QString LastDateTime(std::string table, std::string value);
    int GetdbInt(std::string table, std::string value, std::string known_index, std::string know_value);
    QDateTime GetdbDateTime(std::string table, std::string value, std::string known_index, std::string known_value);
    QList<QList<QVariant>> GetWhere(std::string table, QList<std::string> values, std::string known_index, std::string known_value);
    int GetLatestSetting(std::string setting_name);
    int CountRows(std::string table, std::string column, std::string check_value);
    void DeleteTableContents(std::string table);

    std::map<int, std::map<std::string, std::string>> getBatches(const int& batchid, bool sim);
    std::map<int, std::map<std::string, std::string>> getTeBatches(const int& moBatchId, bool sim);
    std::map<int, std::map<std::string, std::string>> getTe(const int& teBatchId);
    std::map<int, std::map<std::string, std::string>> getDoseToPatient(const int& teBatchId);
    std::map<int, std::map<std::string, std::string>> getPatient(const int& patientId);
    std::map<int, std::map<std::string, std::string>> getDosePatientInfo(const int& teBatchId, bool sim);


    //int newInt(std::string table, std::string value);
};