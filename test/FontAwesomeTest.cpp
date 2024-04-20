#include <gtest/gtest.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TEST(FontAwesomeTest, test1) {
   QFile loadFile("D:/dc/git/ieditor/test/data/fontawesome-free-6.4.0.json");
   if (!loadFile.open(QIODevice::ReadOnly)) {
      qDebug() << "could't open json file";
      return;
   }

   QByteArray allData = loadFile.readAll();  // 读取文件所有内容
   loadFile.close();

   QJsonParseError jsonError;
   QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
   if (jsonError.error != QJsonParseError::NoError) {
      qDebug() << "json error!" << jsonError.errorString();
      return;
   }

   QString outputFile("C:/Users/30310/Desktop/ok.txt");
   QFile output(outputFile);
   if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
      qDebug() << "Can't open" << outputFile;
      return;
   }

   int i(0);
   QJsonObject rootObj = jsonDoc.object();
   for (const auto& item : rootObj) {
      QJsonObject itemObj = item.toObject();
      // qDebug() << rootObj.keys().at(i) << "="
      //          << itemObj.value("unicode").toString() << ";";

      QString name = rootObj.keys().at(i);
      name.replace('-', '_');
      output.write(QString("icon_%1 = 0x%2,\n")
                       .arg(name)
                       .arg(itemObj.value("unicode").toString())
                       .toStdString()
                       .c_str());

      ++i;
   }

   output.close();
   qDebug() << "Output to" << outputFile << "ok";
}
