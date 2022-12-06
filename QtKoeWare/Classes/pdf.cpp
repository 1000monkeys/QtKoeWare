#include "../Headers/pdf.h"
#include <windows.h>
#include <shellapi.h>

#include <string>
#include <codecvt>
#include <locale>
using convert_t = std::codecvt_utf8<wchar_t>;

Pdf::Pdf()
{
}

void Pdf::createPdf(int batchId, bool sim) {
    //int lastBatchId = db->LastInt("batches", "batchId");

    QString html;
    std::map<int, std::map<std::string, std::string>> data = db->getTeBatches(batchId, sim);
    for (int i = 0; i < data.size(); i++) {
        std::map<int, std::map<std::string, std::string>> data2 = db->getBatches(batchId, sim);
        html += "<table width=\"100%\" style=\"page-break-after: always; padding: 3 3 3 3;\"><tr><td colspan=9>";
        html += "<div align=\"left\"><h2>Molybdeen batch:</h2>";
        std::list<std::string> keys;
        keys.push_back("batchId");
        keys.push_back("dateTimeProduced");
        keys.push_back("dateTimeMeasured");
        keys.push_back("radioactivity");
        html += "<table border=1>";
        html += "<tr>";
        for (std::string k : keys) {
            html += "<th style=\"padding: 3 3 3 3;\">";
            html += QString::fromStdString(k);
            if (k == "radioactivity") {
                html += QString::fromStdString("(GBq)");
            }
            html += "</th>";
        }
        html += "</tr>";
        for (int i = 0; i < data2.size(); i++) {
            html += "<tr>";
            for (std::string k : keys) {
                html += "<td align=\"center\" style=\"padding: 3 3 3 3;\">";
                html += QString::fromStdString(data2[i][k]);
                html += "</td>";
                qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data2[i][k]);

            }
            html += "</tr>";
        }
        html += "</table></div></td>";

        html += "<td colspan=1></td>";

        html += "<td colspan=9><div align=\"right\"><h2>Technetium batch:</h2>";
        html += "<table border=1 style=\"padding: 3 3 3 3;\">";
        html += "<tr>";
        keys.clear();
        keys.push_back("moBatchId");
        keys.push_back("teBatchId");
        keys.push_back("dateTimeProduced");
        keys.push_back("radioactivity");
        for (std::string k : keys) {
            html += "<th style=\"padding: 3 3 3 3;\">";
            html += QString::fromStdString(k);
            if (k == "radioactivity") {
                html += QString::fromStdString("(MBq)");
            }
            html += "</th>";
        }
        html += "</tr>";
        html += "<tr>";
        for (std::string k : keys) {
            html += "<td align=\"center\" style=\"padding: 3 3 3 3;\">";
            html += QString::fromStdString(data[i][k]);
            html += "</td>";
            qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data[i][k]);

        }
        html += "</tr>";
        html += "</table></div>";
        html += "</td></tr><tr><td colspan=19>";

        keys.clear();
        keys.push_back("InjectionDate");
        keys.push_back("InjectionTime");
        keys.push_back("DoseRadioactivity");
        keys.push_back("PatientId");
        keys.push_back("PatientName");
        keys.push_back("PatientBirthDate");
        keys.push_back("PatientLength");
        keys.push_back("PatientWeight");
        keys.push_back("PatientSex");
        html += "<br />";
        html += "<h2>Doses:</h2><table border=1 width=\"100%\" style=\"padding: 3 3 3 3;\">";
        html += "<tr>";
        for (std::string k : keys) {
            html += "<th style=\"padding: 3 3 3 3;\">";
            html += QString::fromStdString(k);
            if (k == "DoseRadioactivity") {
                html += QString::fromStdString("(MBq)");
            }
            html += "</th>";
        }
        html += "</tr>";
        std::map<int, std::map<std::string, std::string>> data3 = db->getDosePatientInfo(std::stoi(data[i]["teBatchId"]), sim);
        for (int i = 0; i < data3.size(); i++) {
            html += "<tr>";
            for (std::string k : keys) {
                html += "<td align=\"center\" style=\"padding: 3 3 3 3;\">";
                html += QString::fromStdString(data3[i][k]);
                html += "</td>";
                qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data3[i][k]);

            }
            html += "</tr>";
        }
        html += "</table>";
        html += "</tr></td></table>";
    }

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageOrientation(QPageLayout::Landscape);
    QString filename = QString::number(batchId) + QString::fromStdString("-report-") + QDate::currentDate().toString("yyyy-MM-dd") + QString::fromStdString(".pdf");
    printer.setOutputFileName(filename);

    document.print(&printer);


    std::string new_str = std::filesystem::current_path().generic_string().append("/" + filename.toStdString());
    qDebug() << QString::fromStdString(new_str);
    std::string op = std::string("start ").append(new_str);
    system(op.c_str());
}

std::wstring Pdf::ExePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}