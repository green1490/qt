#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    if(w.setting.value("gui/Theme") != "Default")
    {
        QFile styleShett(":/icons/" + w.setting.value("gui/Theme").toString() + ".qss");
        styleShett.open(QFile::ReadOnly);
        QString style = QLatin1String(styleShett.readAll());
        a.setStyleSheet(style);
        w.show();
        return a.exec();
    }

    w.show();
    return a.exec();
}
