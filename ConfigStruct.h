/****************************************************************************
**
**      Datetime:   2013-04-06
**      Author  :   韦   健
**      Titles  :   这是一个全局的iEditor配置结构，软件加载和关闭是会使用这些数据
**                  进行设置
**
**      Guangxi Normal University 2013
**
****************************************************************************/

#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H

#include <QVector>
#include <QTextCharFormat>
#include <QFont>
#include <QColor>
#include <QStringList>
#include <QString>
#include <QRegularExpression>

struct ConfigStruct
{
private:
    explicit ConfigStruct();

    void ReadConfig();
    static ConfigStruct *config;

public:
    static ConfigStruct* GetConfig();
    void WriterConfig();
    void CreateHighlightRuleList();

    struct HighlightRule{
        QRegularExpression regExp;
        QTextCharFormat format;
    };

public:
    QVector<HighlightRule> highlightRuleList;   //高亮规则集合
    QRegularExpression multipleLinesCommentRegExpStart;
    QRegularExpression multipleLinesCommentRegExpEnd;  //多行注释范围
    QTextCharFormat multiLineCommentFormat; //多行字体样式

    //高亮中的颜色
    QColor predefineAndMacroColor; //预定义与宏字体颜色
    QColor keywordColor;   //关键字字体颜色
    QColor functionColor;  //函数名字体颜色
    QColor stringColor;    //字符串字体颜色
    QColor headFileColor;  //头文件名字字体颜色
    QColor commentColor;   //注释字体颜色
    QColor numberColor;    //数字字体颜色
    //编辑区颜色
    QColor lineNumberColor;    //行号的颜色
    QColor lineAreaBackgroundColor;    //行区域的背景色
    QColor currentLineColor;   //当前行颜色
    QColor textEditBackgroundColor;    //编辑区背景色
    QColor textColor;  //正常文件颜色

    //高亮正则表达
    QStringList predefineAndMacroRegExpStringList; //预定义与宏的正则表达
    QStringList keywordRegExpStringList;   //关键字的正则表达
    QString functionRegExpString;  //函数名的正则表达
    QString stringRegExpString;    //字符串的正则表达
    QString headFileRegExpString;  //头文件名的正则表达
    QString singleLineCommentString;   //单行注释的正则表达
    QString multipleLinesCommentRegExpStartString; //多行注释开始的正则表达
    QString multipleLinesCommentRegExpEndString;   //多行注释结束的正则表达
    QString numberRegExpString;    //数字的正则表达
    QString colorStyle;   //颜色方案
    QString codeStyle;
    QString terminalPath;
    QString gccPath;
    QString gppPath;
    QString gdbPath;
    QString terminalArgument;
    QString compilerArgument;
    QString debugerArgument;
    QString recentDir;

    QFont textFont;    //正常文件字体

    bool isKeywordFontBold; //关键字是否粗体
    bool isFunctionFontBold;    //函数名是否粗体
    bool isSingleLineCommentFontItalic; //单行注释是否斜体
    bool isMultiLineCommentFontItalic;  //多行注释是否斜体
    bool isShowRowNumber;   //行号是否显示
    bool isHighlightCurrentLine;    //是否高亮当前行
    bool isAutoIndent;  //是否自动缩进
    bool isIntelliSense;    //是否智能提示
    bool isLockCompilerPage;  //是否锁定编译器与调试器设置
    bool isSimpleTextMode;  //是否简单的文本编译模式

    int tabWide;    //Tab宽度
    int lineNumberAreaWide; //行号区域大小
};

#endif // CONFIGSTRUCT_H
