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
#ifndef _IEDITOR_CONFIG_H_
#define _IEDITOR_CONFIG_H_

#include "Singleton.h"

#include <QVector>
#include <QTextCharFormat>
#include <QFont>
#include <QColor>
#include <QStringList>
#include <QString>
#include <QRegularExpression>
#include <memory>
#include <mutex>

// 颜色样式
struct ColorStyle {
   // 高亮中的颜色
   QColor predefineAndMacroColor;  // 预定义与宏字体颜色
   QColor keywordColor;            // 关键字字体颜色
   QColor functionColor;           // 函数名字体颜色
   QColor stringColor;             // 字符串字体颜色
   QColor headFileColor;           // 头文件名字字体颜色
   QColor commentColor;            // 注释字体颜色
   QColor numberColor;             // 数字字体颜色

   // 编辑区颜色
   QColor lineNumberColor;          // 行号的颜色
   QColor lineAreaBackgroundColor;  // 行区域的背景色
   QColor currentLineColor;         // 当前行颜色
   QColor textEditBackgroundColor;  // 编辑区背景色
   QColor textColor;                // 正常文件颜色
};

struct HighlightingRule {
   QRegularExpression pattern;
   QTextCharFormat format;
};

class QSettings;

struct Config {
public:
   Config(const Config&) = delete;
   Config& operator=(const Config&) = delete;
   Config(Config&&) = delete;
   ~Config();

   bool init(const QString& configFile);

   bool isInit() const {
      std::lock_guard<std::mutex> lg(lock_);
      return isInit_;
   }

   const QString& getConfigFile() const { return configFile_; }

   void saveConfig();
   void saveMainWindowConfig();
   void createHighlightingRules();
   const std::shared_ptr<ColorStyle> getColorStyleByName(
       const QString& colorStyleName);

   const std::shared_ptr<ColorStyle> getCurrentColorStyle() const;

   const QString& getColorStyle() const { return colorStyle_; }

   void setColorStyle(const QString& colorStyle) {
      colorStyle_.assign(colorStyle);
   }

protected:
   explicit Config();

   friend class common::Singleton<Config>;

private:
   bool loading();

private:
   bool isInit_;
   QString configFile_;
   mutable std::mutex lock_;
   QSettings* settings_;
   QString colorStyle_;  // 颜色方案

public:
   // IeditorConfig
   QStringList predefineAndMacroRegExpStringList_;  // 预定义与宏的正则表达
   QStringList keywordRegExpStringList_;            // 关键字的正则表达
   QString functionRegExpString_;                   // 函数名的正则表达
   QString stringRegExpString_;                     // 字符串的正则表达
   QString headFileRegExpString_;     // 头文件名的正则表达
   QString singleLineCommentString_;  // 单行注释的正则表达
   QString multipleLinesCommentRegExpStartString_;  // 多行注释开始的正则表达
   QString multipleLinesCommentRegExpEndString_;  // 多行注释结束的正则表达
   QString numberRegExpString_;                   // 数字的正则表达
   QString codeStyle_;
   QString terminalPath_;
   QString gccPath_;
   QString gppPath_;
   QString gdbPath_;
   QString terminalArgument_;
   QString compilerArgument_;
   QString debugerArgument_;
   QString recentDir_;
   QFont textFont_;                      // 正常文件字体
   bool isKeywordFontBold_;              // 关键字是否粗体
   bool isFunctionFontBold_;             // 函数名是否粗体
   bool isSingleLineCommentFontItalic_;  // 单行注释是否斜体
   bool isMultiLineCommentFontItalic_;   // 多行注释是否斜体
   bool isShowRowNumber_;                // 行号是否显示
   bool isHighlightCurrentLine_;         // 是否高亮当前行
   bool isAutoIndent_;                   // 是否自动缩进
   bool isIntelliSense_;                 // 是否智能提示
   bool isLockCompilerPage_;  // 是否锁定编译器与调试器设置
   bool isSimpleTextMode_;    // 是否简单的文本编译模式
   int tabWide_;              // Tab宽度
   int lineNumberAreaWide_;   // 行号区域大小

   // MainWindowConfig
   QByteArray mainWindowState_;
   QByteArray mainWindowGeometry_;
   QStringList recentFiles_;
   Qt::DockWidgetArea sideBarPosition_;
   bool isShowSideBar_;
   bool isShowOutputDock_;
   bool isShowFileToolsBar_;
   bool isShowEditToolsBar_;
   bool isShowStatusBar_;
   bool isLockTools_;

   // ColorStyle
   QMap<QString, std::shared_ptr<ColorStyle>> colorStyleMap_;  // 颜色样式

   QRegularExpression multipleLinesCommentRegExpStart_;
   QRegularExpression multipleLinesCommentRegExpEnd_;  // 多行注释范围
   QTextCharFormat multiLineCommentFormat_;            // 多行字体样式
   QVector<std::shared_ptr<HighlightingRule>>
       highlightRuleList_;  // 高亮规则集合
};

#endif  // _IEDITOR_CONFIG_H_
