#include "../Headers/pdf.h"

Pdf::Pdf()
{
}

void Pdf::createPdf() {
    int lastBatchId = db->LastInt("batches", "batchId");
    std::map<int, std::map<std::string, std::string>> data = db->getBatches(lastBatchId);
    qDebug() << "BATCHID!!!!!!!!!";
    for (int i = 0; i < data.size(); i++) {
        for (const auto& [k, v] : data[i]) {
            qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data[i][k]);
        }
    }

    int teBatchId = 0;
    data = db->getTeBatches(lastBatchId);
    qDebug() << "TEBATCHID!!!!!!!!!";
    for (int i = 0; i < data.size(); i++) {
        for (const auto& [k, v] : data[i]) {
            qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data[i][k]);
        }
    
        std::map<int, std::map<std::string, std::string>> data2 = db->getDoseToPatient(std::stoi(data[i]["teBatchId"]));
        qDebug() << "DOSETOPATIENT!!!!!!!!!";
        for (int i = 0; i < data2.size(); i++) {
            for (const auto& [k, v] : data2[i]) {
                qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data2[i][k]);
            }

            std::map<int, std::map<std::string, std::string>> data3 = db->getPatient(std::stoi(data2[i]["patientId"]));
            qDebug() << "PATIENT!!!!!!!!!";
            for (int i = 0; i < data3.size(); i++) {
                for (const auto& [k, v] : data3[i]) {
                    qDebug() << QString::fromStdString(k) << " : " << QString::fromStdString(data3[i][k]);
                }
            }
        }
    
    }

    //qDebug() << QString::fromStdString(data[1]["id"]);

    QString html = "<h1>Batches:</h1>";
    html += "<table border=\"1\">";
        html += "<tr>";
            html += "<th>Batch ID</th>";
            html += "<th>Date/Time produced</th>";
            html += "<th>Date/Time measured</th>";
            html += "<th>Radio activity</th>";
        html += "</tr>";
        html += "<tr>";
            html += "<td>123</td>";
            html += "<td>22-01-2021 08:56:34</td>";
            html += "<td>22-01-2021 05:45:51</td>";
            html += "<td>600 GBq</td>";
        html += "</tr>";
    html += "</table>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    document.print(&printer);
}