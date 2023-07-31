/****************************************************************************
**
**      Datetime:   2013-03-21 12:27
**      Author  :   韦   健
**      Titles  :   软件入口
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QString>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_WS_X11 //Linux

    QString translatorPath = "/usr/share/qt4/translations/qt_zh_CN.qm";
    QTextCodec *textc = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(textc);
    QTextCodec::setCodecForCStrings(textc);
    QTextCodec::setCodecForTr(textc);
#endif

#ifdef Q_WS_WIN //Windows

    QString translatorPath = "F:\\Soft\\QtLib\\translations\\qt_zh_CN.qm";
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

    QTranslator translator;
    translator.load(translatorPath);
    app.installTranslator(&translator);
    
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
