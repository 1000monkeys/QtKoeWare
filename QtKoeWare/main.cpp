#include "QtKoeWare.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtKoeWare w;
    w.show();
    w.move(QPoint(std::int16_t(w.geometry().x()), std::int16_t(w.geometry().y())));

    return a.exec();
}
