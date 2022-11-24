#include "../Headers/pdf.h"

Pdf::Pdf(QMainWindow* parent) : QMainWindow(parent)
{
    QString test;
    QString html = "<h1>Hi!</h1>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("test.pdf");

    document.print(&printer);
}

void Pdf::createPdf() {
    qDebug() << "CREATE PDF!!";
}