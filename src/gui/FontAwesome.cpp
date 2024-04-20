#include "FontAwesome.h"

#include <QFontDatabase>
#include <QStringList>

FontAwesome::FontAwesome() : isInit_(false) {}

FontAwesome::~FontAwesome() {}

bool FontAwesome::init() {
   std::lock_guard<std::mutex> lg(lock_);
   if (isInit_) {
      return true;
   }

   int fontId = QFontDatabase::addApplicationFont(
       ":/fontawesome-free-6.4.0/fa-regular-400.ttf");
   QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
   font_.setFamily(fontFamilies.at(0));
   font_.setPointSize(10);
   isInit_ = true;
   return true;
}
