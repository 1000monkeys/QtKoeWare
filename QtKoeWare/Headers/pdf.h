#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QPrinter>

QT_BEGIN_NAMESPACE
class Pdf : public QMainWindow {
	Q_OBJECT

public:
	Pdf(QMainWindow* parent = nullptr);
	void createPdf();
};
QT_END_NAMESPACE