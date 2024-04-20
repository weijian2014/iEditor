/****************************************************************************
**
**      Datetime:   2013-04-06
**      Author  :   韦   健
**      Titles  : 这是一个全局的ieditor配置结构，软件加载和关闭是会使用这些数据
**                  进行设置
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "Config.h"

#include <QtGui>
#include <QSettings>
#include <memory>

static const char* IeditorConfigC("IeditorConfig");
static const char* MainWindowConfigC("MainWindowConfig");
static const char* CustomC("Custom");

// ColorStyle
static const QSet<const char*> ColorStyleNamesC = {
    "Default", "DarkVim", "Grayscale", "Grayscale", "Inkpot", "Custom"};

Config::Config()
    : isInit_(false),
      configFile_(""),
      lock_(),
      settings_(nullptr),
      colorStyle_(""),
      predefineAndMacroRegExpStringList_(),
      keywordRegExpStringList_(),
      functionRegExpString_(""),
      stringRegExpString_(""),
      headFileRegExpString_(""),
      singleLineCommentString_(""),
      multipleLinesCommentRegExpStartString_(),
      multipleLinesCommentRegExpEndString_(),
      numberRegExpString_(""),
      codeStyle_(""),
      terminalPath_(""),
      gccPath_(""),
      gppPath_(""),
      gdbPath_(""),
      terminalArgument_(""),
      compilerArgument_(""),
      debugerArgument_(""),
      recentDir_(QCoreApplication::applicationDirPath()),
      textFont_(),
      isKeywordFontBold_(false),
      isFunctionFontBold_(false),
      isSingleLineCommentFontItalic_(false),
      isMultiLineCommentFontItalic_(false),
      isShowRowNumber_(true),
      isHighlightCurrentLine_(true),
      isAutoIndent_(true),
      isIntelliSense_(false),
      isLockCompilerPage_(true),
      isSimpleTextMode_(true),
      tabWide_(3),
      lineNumberAreaWide_(60),
      mainWindowState_(),
      mainWindowGeometry_(),
      recentFiles_(),
      sideBarPosition_(Qt::DockWidgetArea::LeftDockWidgetArea),
      isShowSideBar_(false),
      isShowOutputDock_(""),
      isShowFileToolsBar_(false),
      isShowEditToolsBar_(false),
      isShowStatusBar_(true),
      isLockTools_(true),
      colorStyleMap_(),
      multipleLinesCommentRegExpStart_(),
      multipleLinesCommentRegExpEnd_(),
      multiLineCommentFormat_(),
      highlightRuleList_() {}

Config::~Config() {
   if (settings_) {
      delete settings_;
   }
}

bool Config::init(const QString& configFile) {
   std::lock_guard<std::mutex> lg(lock_);
   if (isInit_) {
      return true;
   }

   configFile_.assign(configFile);
   if (!loading()) {
      return false;
   }

   createHighlightingRules();
   isInit_ = true;
   return true;
}

void Config::createHighlightingRules() {
   highlightRuleList_.clear();
   const std::shared_ptr<ColorStyle> currentColorStyle = getCurrentColorStyle();
   std::shared_ptr<HighlightingRule> rule(nullptr);

   // 预定义与宏高亮格式
   QTextCharFormat predefineAndMacroFormat;
   predefineAndMacroFormat.setForeground(
       currentColorStyle->predefineAndMacroColor);
   // predefineAndMacroFormat.setFontWeight(QFont::Bold);
   foreach (const QString& predefineAndMacro,
            predefineAndMacroRegExpStringList_) {
      rule = std::make_shared<HighlightingRule>();
      rule->pattern = QRegularExpression(predefineAndMacro);
      rule->format = predefineAndMacroFormat;
      highlightRuleList_.append(rule);
   }

   // 关键字高亮
   QTextCharFormat keywordFormat;
   keywordFormat.setForeground(currentColorStyle->keywordColor);
   if (isKeywordFontBold_) keywordFormat.setFontWeight(QFont::Bold);
   foreach (const QString& keyword, keywordRegExpStringList_) {
      rule = std::make_shared<HighlightingRule>();
      rule->pattern = QRegularExpression(keyword);
      rule->format = keywordFormat;
      highlightRuleList_.append(rule);
   }

   // 函数高亮样式
   QTextCharFormat functionFormat;
   functionFormat.setForeground(currentColorStyle->functionColor);
   functionFormat.setFontItalic(isFunctionFontBold_);  // 斜体
   if (isFunctionFontBold_) {
      functionFormat.setFontWeight(QFont::Bold);
   }
   rule = std::make_shared<HighlightingRule>();
   rule->pattern = QRegularExpression(functionRegExpString_);
   rule->format = functionFormat;
   highlightRuleList_.append(rule);

   // 字符串高亮样式
   QTextCharFormat stringFormat;
   stringFormat.setForeground(currentColorStyle->stringColor);
   rule = std::make_shared<HighlightingRule>();
   rule->pattern = QRegularExpression(stringRegExpString_);
   rule->format = stringFormat;
   highlightRuleList_.append(rule);

   // // 头文件高亮样式
   QTextCharFormat headFileFormat;
   headFileFormat.setForeground(currentColorStyle->headFileColor);
   rule = std::make_shared<HighlightingRule>();
   rule->pattern = QRegularExpression(headFileRegExpString_);
   rule->format = headFileFormat;
   highlightRuleList_.append(rule);

   // 单行注释高亮样式
   QTextCharFormat singleLineCommentFormat;
   singleLineCommentFormat.setFontItalic(isSingleLineCommentFontItalic_);
   singleLineCommentFormat.setForeground(currentColorStyle->commentColor);
   rule = std::make_shared<HighlightingRule>();
   rule->pattern = QRegularExpression(singleLineCommentString_);
   rule->format = singleLineCommentFormat;
   highlightRuleList_.append(rule);

   // 多行注释高亮样式
   multipleLinesCommentRegExpStart_ =
       QRegularExpression(multipleLinesCommentRegExpStartString_);
   multipleLinesCommentRegExpEnd_ =
       QRegularExpression(multipleLinesCommentRegExpEndString_);
   multiLineCommentFormat_.setForeground(currentColorStyle->commentColor);
   multiLineCommentFormat_.setFontItalic(
       isMultiLineCommentFontItalic_);  // 斜体

   // 数字高亮样式
   QTextCharFormat numberFormat;
   numberFormat.setForeground(currentColorStyle->numberColor);
   rule = std::make_shared<HighlightingRule>();
   rule->pattern = QRegularExpression(numberRegExpString_);
   rule->format = numberFormat;
   highlightRuleList_.append(rule);
}

const std::shared_ptr<ColorStyle> Config::getColorStyleByName(
    const QString& colorStyleName) {
   const auto& iter = colorStyleMap_.find(colorStyleName);
   if (iter != colorStyleMap_.end()) {
      return iter.value();
   } else {
      return nullptr;
   }
}

const std::shared_ptr<ColorStyle> Config::getCurrentColorStyle() const {
   const auto& iter = colorStyleMap_.find(colorStyle_);
   Q_ASSERT(iter != colorStyleMap_.end());
   return iter.value();
}

bool Config::loading() {
   QFile file(configFile_);
   if (!file.exists()) {
      qFatal() << "The file" << configFile_ << "not exists";
   }

   settings_ = new QSettings(configFile_, QSettings::IniFormat);
   if (settings_->status() != QSettings::NoError) {
      qCritical() << "Read file " << configFile_ << " failed";
      return false;
   }

   // IeditorConfig
   settings_->beginGroup(IeditorConfigC);
   predefineAndMacroRegExpStringList_ =
       settings_->value("predefineAndMacroRegExpStringList").toStringList();
   keywordRegExpStringList_ =
       settings_->value("keywordRegExpStringList").toStringList();
   functionRegExpString_ = settings_->value("functionRegExpString").toString();
   stringRegExpString_ = settings_->value("stringRegExpString").toString();
   headFileRegExpString_ = settings_->value("headFileRegExpString").toString();
   singleLineCommentString_ =
       settings_->value("singleLineCommentString").toString();
   multipleLinesCommentRegExpStartString_ =
       settings_->value("multipleLinesCommentRegExpStartString").toString();
   multipleLinesCommentRegExpEndString_ =
       settings_->value("multipleLinesCommentRegExpEndString").toString();
   numberRegExpString_ = settings_->value("numberRegExpString").toString();
   colorStyle_ = settings_->value("colorStyle").toString().toLocal8Bit();
   codeStyle_ = settings_->value("codeStyle").toString();
   terminalPath_ = settings_->value("terminalPath").toString();
   gccPath_ = settings_->value("gccPath").toString();
   gppPath_ = settings_->value("gppPath").toString();
   gdbPath_ = settings_->value("gdbPath").toString();
   terminalArgument_ = settings_->value("terminalArgument").toString();
   compilerArgument_ = settings_->value("compilerArgument").toString();
   debugerArgument_ = settings_->value("debugerArgument").toString();
   if (!settings_->value("recentDir").toString().isEmpty()) {
      recentDir_ = settings_->value("recentDir").toString();
   }
   textFont_ = settings_->value("textFont").value<QFont>();
   isKeywordFontBold_ = settings_->value("isKeywordFontBold").toBool();
   isFunctionFontBold_ = settings_->value("isFunctionFontBold").toBool();
   isSingleLineCommentFontItalic_ =
       settings_->value("isSingleLineCommentFontItalic").toBool();
   isMultiLineCommentFontItalic_ =
       settings_->value("isMultiLineCommentFontItalic").toBool();
   isShowRowNumber_ = settings_->value("isShowRowNumber").toBool();
   isHighlightCurrentLine_ =
       settings_->value("isHighlightCurrentLine").toBool();
   isAutoIndent_ = settings_->value("isAutoIndent").toBool();
   isIntelliSense_ = settings_->value("isIntelliSense").toBool();
   isLockCompilerPage_ = settings_->value("isLockCompilerPage").toBool();
   isSimpleTextMode_ = settings_->value("isSimpleTextMode").toBool();
   tabWide_ = settings_->value("tabWide").toInt();
   lineNumberAreaWide_ = settings_->value("lineNumberAreaWide").toInt();
   settings_->endGroup();

   // MainWindowConfig
   settings_->beginGroup(MainWindowConfigC);
   mainWindowState_ = settings_->value("mainWindowState").toByteArray();
   mainWindowGeometry_ = settings_->value("mainWindowGeometry").toByteArray();
   recentFiles_ = settings_->value("recentFiles").toStringList();
   sideBarPosition_ = static_cast<Qt::DockWidgetArea>(
       settings_->value("sideBarPosition").toInt());
   isShowSideBar_ = settings_->value("isShowSideBar").toBool();
   isShowOutputDock_ = settings_->value("isShowOutputDock").toBool();
   isShowFileToolsBar_ = settings_->value("isShowFileToolsBar").toBool();
   isShowEditToolsBar_ = settings_->value("isShowEditToolsBar").toBool();
   isShowStatusBar_ = settings_->value("isShowStatusBar").toBool();
   isLockTools_ = settings_->value("isLockTools").toBool();
   settings_->endGroup();

   // ColorStyle
   for (const auto& name : ColorStyleNamesC) {
      settings_->beginGroup(name);
      std::shared_ptr<ColorStyle> cs = std::make_shared<ColorStyle>();
      cs->predefineAndMacroColor =
          settings_->value("predefineAndMacroColor").value<QColor>();
      cs->keywordColor = settings_->value("keywordColor").value<QColor>();
      cs->functionColor = settings_->value("functionColor").value<QColor>();
      cs->stringColor = settings_->value("stringColor").value<QColor>();
      cs->headFileColor = settings_->value("headFileColor").value<QColor>();
      cs->commentColor = settings_->value("commentColor").value<QColor>();
      cs->numberColor = settings_->value("numberColor").value<QColor>();
      cs->lineNumberColor = settings_->value("lineNumberColor").value<QColor>();
      cs->lineAreaBackgroundColor =
          settings_->value("lineAreaBackgroundColor").value<QColor>();
      cs->currentLineColor =
          settings_->value("currentLineColor").value<QColor>();
      cs->textEditBackgroundColor =
          settings_->value("textEditBackgroundColor").value<QColor>();
      cs->textColor = settings_->value("textColor").value<QColor>();
      settings_->endGroup();
      colorStyleMap_.insert(name, cs);
   }

   qInfo() << "Configuration file" << configFile_ << "loading successfully";
   return true;
}

void Config::saveConfig() {
   if (settings_->status() != QSettings::NoError) {
      qCritical() << "Read file " << configFile_ << " failed";
      return;
   }

   settings_->beginGroup(IeditorConfigC);
   settings_->setValue("predefineAndMacroRegExpStringList",
                       predefineAndMacroRegExpStringList_);
   settings_->setValue("keywordRegExpStringList", keywordRegExpStringList_);
   settings_->setValue("functionRegExpString", functionRegExpString_);
   settings_->setValue("stringRegExpString", stringRegExpString_);
   settings_->setValue("headFileRegExpString", headFileRegExpString_);
   settings_->setValue("singleLineCommentString", singleLineCommentString_);
   settings_->setValue("multipleLinesCommentRegExpStartString",
                       multipleLinesCommentRegExpStartString_);
   settings_->setValue("multipleLinesCommentRegExpEndString",
                       multipleLinesCommentRegExpEndString_);
   settings_->setValue("numberRegExpString", numberRegExpString_);
   settings_->setValue("colorStyle", colorStyle_);
   settings_->setValue("codeStyle", codeStyle_);
   settings_->setValue("terminalPath", terminalPath_);
   settings_->setValue("gccPath", gccPath_);
   settings_->setValue("gppPath", gppPath_);
   settings_->setValue("gdbPath", gdbPath_);
   settings_->setValue("terminalArgument", terminalArgument_);
   settings_->setValue("compilerArgument", compilerArgument_);
   settings_->setValue("debugerArgument", debugerArgument_);
   settings_->setValue("recentDir", recentDir_);
   settings_->setValue("textFont", textFont_.toString());
   settings_->setValue("isKeywordFontBold", isKeywordFontBold_);
   settings_->setValue("isFunctionFontBold", isFunctionFontBold_);
   settings_->setValue("isSingleLineCommentFontItalic",
                       isSingleLineCommentFontItalic_);
   settings_->setValue("isMultiLineCommentFontItalic",
                       isMultiLineCommentFontItalic_);
   settings_->setValue("isShowRowNumber", isShowRowNumber_);
   settings_->setValue("isHighlightCurrentLine", isHighlightCurrentLine_);
   settings_->setValue("isAutoIndent", isAutoIndent_);
   settings_->setValue("isIntelliSense", isIntelliSense_);
   settings_->setValue("isLockCompilerPage", isLockCompilerPage_);
   settings_->setValue("isSimpleTextMode", isSimpleTextMode_);
   settings_->setValue("tabWide", tabWide_);
   settings_->setValue("lineNumberAreaWide", lineNumberAreaWide_);
   settings_->endGroup();

   qInfo() << IeditorConfigC << "saved successfully";

   // 只保存Custom的颜色样式
   settings_->beginGroup(CustomC);
   auto iter = colorStyleMap_.find(CustomC);
   Q_ASSERT(iter != colorStyleMap_.end());
   std::shared_ptr<ColorStyle> cs = iter.value();
   settings_->setValue("predefineAndMacroColor",
                       cs->predefineAndMacroColor.name());
   settings_->setValue("keywordColor", cs->keywordColor.name());
   settings_->setValue("functionColor", cs->functionColor.name());
   settings_->setValue("stringColor", cs->stringColor.name());
   settings_->setValue("headFileColor", cs->headFileColor.name());
   settings_->setValue("commentColor", cs->commentColor.name());
   settings_->setValue("numberColor", cs->numberColor.name());
   settings_->setValue("lineNumberColor", cs->lineNumberColor.name());
   settings_->setValue("lineAreaBackgroundColor",
                       cs->lineAreaBackgroundColor.name());
   settings_->setValue("currentLineColor", cs->currentLineColor.name());
   settings_->setValue("textEditBackgroundColor",
                       cs->textEditBackgroundColor.name());
   settings_->setValue("textColor", cs->textColor.name());
   settings_->endGroup();
   settings_->sync();

   qInfo() << "Color style" << CustomC << "saved successfully";
}

void Config::saveMainWindowConfig() {
   if (settings_->status() != QSettings::NoError) {
      qCritical() << "Read file " << configFile_ << " failed";
      return;
   }

   settings_->beginGroup(MainWindowConfigC);
   settings_->setValue("mainWindowState", mainWindowState_);
   settings_->setValue("mainWindowGeometry", mainWindowGeometry_);
   settings_->setValue("recentFiles", recentFiles_);
   settings_->setValue("sideBarPosition", sideBarPosition_);
   settings_->setValue("isShowSideBar", isShowSideBar_);
   settings_->setValue("isShowOutputDock", isShowOutputDock_);
   settings_->setValue("isShowFileToolsBar", isShowFileToolsBar_);
   settings_->setValue("isShowEditToolsBar", isShowEditToolsBar_);
   settings_->setValue("isShowStatusBar", isShowStatusBar_);
   settings_->setValue("isLockTools", isLockTools_);
   settings_->endGroup();
   settings_->sync();

   qInfo() << MainWindowConfigC << "saved successfully";
}
