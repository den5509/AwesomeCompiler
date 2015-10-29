#include "ui.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QResource>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QResource::registerResource("resources.qrc");
    QApplication a(argc, argv);

    QTranslator myTranslator;
    myTranslator.load("AwesomeCompiler-" + QLocale::system().name());
    a.installTranslator(&myTranslator);

    UI w;
    w.show();

    return a.exec();
}
