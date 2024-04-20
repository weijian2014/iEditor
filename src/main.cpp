/****************************************************************************
**
**      Datetime:   2013-03-21 12:27
**      Author  :   韦   健
**      Titles  :   软件入口
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "Config.h"
#include "Singleton.h"
#include "FontAwesome.h"
#include "MainWindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QTranslator>
#include <QString>
#include <QDebug>

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);

   QCoreApplication::setApplicationName("ieditor");
   QCoreApplication::setApplicationVersion("v1.1.1");
   QCoreApplication::setOrganizationName("weijian");
   QCoreApplication::setOrganizationDomain("github.com");

   QCommandLineParser parser;
   parser.setApplicationDescription("ieditor");
   parser.addHelpOption();
   parser.addVersionOption();
   QCommandLineOption configOption(QStringList() << "c"
                                                 << "config",
                                   "The path of configuration",
                                   "./config/config.ini");
   parser.addOption(configOption);
   parser.process(app);

   QString configFile("");
   if (parser.isSet(configOption)) {
      configFile.assign(parser.value("c"));
   } else {
      configFile.assign("./config/config.ini");
   }

   Config* config = common::Singleton<Config>::instancePointer();
   Q_ASSERT(config);
   if (!config->init(configFile)) {
      qFatal() << "Config init failed" << Qt::endl;
   }

   FontAwesome* font = common::Singleton<FontAwesome>::instancePointer();
   Q_ASSERT(font);
   if (!font->init()) {
      qFatal() << "FontAwesome init failed" << Qt::endl;
   }

   // QString translatorPath(":/translations/en_US.qm");
   // QTranslator translator;
   // if (translator.load(translatorPath)) {
   //    app.installTranslator(&translator);
   // } else {
   //    qCritical() << "Load translation file " << translatorPath << " failed"
   //                << Qt::endl;
   //    app.exit(-1);
   // }

   MainWindow mainWindow;
   mainWindow.setMinimumSize(800, 500);
   mainWindow.show();

   return app.exec();
}
