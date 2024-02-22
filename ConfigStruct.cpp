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
#include <QtGui>
#include <QSettings>

#include "ConfigStruct.h"

ConfigStruct* ConfigStruct::config = 0;

ConfigStruct::ConfigStruct()
{
    ReadConfig();
    CreateHighlightRuleList();
}

void ConfigStruct::CreateHighlightRuleList()
{
    HighlightRule rule;

    //预定义与宏高亮格式
    QTextCharFormat predefineAndMacroFormat;
    predefineAndMacroFormat.setForeground(predefineAndMacroColor);
    QStringList predefineAndMacroStringList(predefineAndMacroRegExpStringList);
    foreach (const QString &predefineAndMacro, predefineAndMacroStringList){
        rule.regExp = QRegularExpression(predefineAndMacro);
        rule.format = predefineAndMacroFormat;
        highlightRuleList.append(rule);
    }

    //关键字高亮
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(keywordColor);
    if (isKeywordFontBold)
        keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordList(keywordRegExpStringList);
    foreach (const QString &keyword, keywordList){
        rule.regExp = QRegularExpression(keyword);
        rule.format = keywordFormat;
        highlightRuleList.append(rule);
    }

    //函数高亮样式
    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(isFunctionFontBold);//斜体
    functionFormat.setForeground(functionColor);
    if (isFunctionFontBold)
        functionFormat.setFontWeight(QFont::Bold);
    rule.regExp = QRegularExpression(functionRegExpString);
    rule.format = functionFormat;
    highlightRuleList.append(rule);

    //字符串高亮样式
    QTextCharFormat stringFormat;
    stringFormat.setForeground(stringColor);
    rule.regExp = QRegularExpression(stringRegExpString);
    rule.format = stringFormat;
    highlightRuleList.append(rule);

    //头文件高亮样式
    QTextCharFormat headFileFormat;
    headFileFormat.setForeground(headFileColor);
    rule.regExp = QRegularExpression(headFileRegExpString);
    rule.format = headFileFormat;
    highlightRuleList.append(rule);

    //单行注释高亮样式
    QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setFontItalic(isSingleLineCommentFontItalic);
    singleLineCommentFormat.setForeground(commentColor);
    rule.regExp = QRegularExpression(singleLineCommentString);
	rule.format = singleLineCommentFormat;
	highlightRuleList.append(rule);

    //多行注释高亮样式
    multipleLinesCommentRegExpStart = QRegularExpression(multipleLinesCommentRegExpStartString);
    multipleLinesCommentRegExpEnd = QRegularExpression(multipleLinesCommentRegExpEndString);
    multiLineCommentFormat.setForeground(commentColor);
    multiLineCommentFormat.setFontItalic(isMultiLineCommentFontItalic);//斜体

    //数字高亮样式
    QTextCharFormat numberFormat;
    numberFormat.setForeground(numberColor);
    rule.regExp = QRegularExpression(numberRegExpString);
    rule.format = numberFormat;
    highlightRuleList.append(rule);
}

ConfigStruct* ConfigStruct::GetConfig()
{
    if(0 == config)
    {
        config = new ConfigStruct;
    }
    return config;
}

void ConfigStruct::ReadConfig()
{
    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);
    //settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    //settings.setIniCodec(QTextCodec::codecForLocale());

    settings.beginGroup("iEditorConfig");
    predefineAndMacroRegExpStringList = settings.value("predefineAndMacroRegExpStringList").toStringList();
    keywordRegExpStringList = settings.value("keywordRegExpStringList").toStringList();
    functionRegExpString = settings.value("functionRegExpString").toString();
    stringRegExpString = settings.value("stringRegExpString").toString();
    headFileRegExpString = settings.value("headFileRegExpString").toString();
    singleLineCommentString = settings.value("singleLineCommentString").toString();
    multipleLinesCommentRegExpStartString = settings.value("multipleLinesCommentRegExpStartString").toString();
    multipleLinesCommentRegExpEndString = settings.value("multipleLinesCommentRegExpEndString").toString();
    numberRegExpString = settings.value("numberRegExpString").toString();
    colorStyle = settings.value("colorStyle").toString().toLocal8Bit();
    codeStyle = settings.value("codeStyle").toString();
    terminalPath = settings.value("terminalPath").toString();
    gccPath = settings.value("gccPath").toString();
    gppPath = settings.value("gppPath").toString();
    gdbPath = settings.value("gdbPath").toString();
    terminalArgument = settings.value("terminalArgument").toString();
    compilerArgument = settings.value("compilerArgument").toString();
    debugerArgument = settings.value("debugerArgument").toString();
    recentDir = settings.value("recentDir").toString();
    textFont = settings.value("textFont").value<QFont>();
    isKeywordFontBold = settings.value("isKeywordFontBold").toBool();
    isFunctionFontBold = settings.value("isFunctionFontBold").toBool();
    isSingleLineCommentFontItalic = settings.value("isSingleLineCommentFontItalic").toBool();
    isMultiLineCommentFontItalic = settings.value("isMultiLineCommentFontItalic").toBool();
    isShowRowNumber = settings.value("isShowRowNumber").toBool();
    isHighlightCurrentLine = settings.value("isHighlightCurrentLine").toBool();
    isAutoIndent = settings.value("isAutoIndent").toBool();
    isIntelliSense = settings.value("isIntelliSense").toBool();
    isLockCompilerPage = settings.value("isLockCompilerPage").toBool();
    isSimpleTextMode = settings.value("isSimpleTextMode").toBool();
    tabWide = settings.value("tabWide").toInt();
    lineNumberAreaWide = settings.value("lineNumberAreaWide").toInt();
    settings.endGroup();

    settings.beginGroup(colorStyle);
    predefineAndMacroColor = settings.value("predefineAndMacroColor").value<QColor>();
    keywordColor = settings.value("keywordColor").value<QColor>();
    functionColor = settings.value("functionColor").value<QColor>();
    stringColor = settings.value("stringColor").value<QColor>();
    headFileColor = settings.value("headFileColor").value<QColor>();
    commentColor = settings.value("commentColor").value<QColor>();
    numberColor = settings.value("numberColor").value<QColor>();
    lineNumberColor = settings.value("lineNumberColor").value<QColor>();
    lineAreaBackgroundColor = settings.value("lineAreaBackgroundColor").value<QColor>();
    currentLineColor = settings.value("currentLineColor").value<QColor>();
    textEditBackgroundColor = settings.value("textEditBackgroundColor").value<QColor>();
    textColor = settings.value("textColor").value<QColor>();
    settings.endGroup();
}

void ConfigStruct::WriterConfig()
{

    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);
    //settings.clear();
    //settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    //settings.setIniCodec(QTextCodec::codecForLocale());

    settings.beginGroup("iEditorConfig");
    settings.setValue("predefineAndMacroRegExpStringList", predefineAndMacroRegExpStringList);
    settings.setValue("keywordRegExpStringList", keywordRegExpStringList);
    settings.setValue("functionRegExpString", functionRegExpString);
    settings.setValue("stringRegExpString", stringRegExpString);
    settings.setValue("headFileRegExpString", headFileRegExpString);
    settings.setValue("singleLineCommentString", singleLineCommentString);
    settings.setValue("multipleLinesCommentRegExpStartString", multipleLinesCommentRegExpStartString);
    settings.setValue("multipleLinesCommentRegExpEndString", multipleLinesCommentRegExpEndString);
    settings.setValue("numberRegExpString", numberRegExpString);
    settings.setValue("colorStyle", colorStyle);
    settings.setValue("codeStyle", codeStyle);
    settings.setValue("terminalPath", terminalPath);
    settings.setValue("gccPath", gccPath);
    settings.setValue("gppPath", gppPath);
    settings.setValue("gdbPath", gdbPath);
    settings.setValue("terminalArgument", terminalArgument);
    settings.setValue("compilerArgument", compilerArgument);
    settings.setValue("debugerArgument", debugerArgument);
    settings.setValue("recentDir", recentDir);
    settings.setValue("textFont", textFont.toString());
    settings.setValue("isKeywordFontBold", isKeywordFontBold);
    settings.setValue("isFunctionFontBold", isFunctionFontBold);
    settings.setValue("isSingleLineCommentFontItalic", isSingleLineCommentFontItalic);
    settings.setValue("isMultiLineCommentFontItalic", isMultiLineCommentFontItalic);
    settings.setValue("isShowRowNumber", isShowRowNumber);
    settings.setValue("isHighlightCurrentLine", isHighlightCurrentLine);
    settings.setValue("isAutoIndent", isAutoIndent);
    settings.setValue("isIntelliSense", isIntelliSense);
    settings.setValue("isLockCompilerPage", isLockCompilerPage);
    settings.setValue("isSimpleTextMode", isSimpleTextMode);
    settings.setValue("tabWide", tabWide);
    settings.setValue("lineNumberAreaWide", lineNumberAreaWide);
    settings.endGroup();

    settings.beginGroup(colorStyle);
    settings.setValue("predefineAndMacroColor", predefineAndMacroColor.name());
    settings.setValue("keywordColor", keywordColor.name());
    settings.setValue("functionColor", functionColor.name());
    settings.setValue("stringColor", stringColor.name());
    settings.setValue("headFileColor", headFileColor.name());
    settings.setValue("commentColor", commentColor.name());
    settings.setValue("numberColor", numberColor.name());
    settings.setValue("lineNumberColor", lineNumberColor.name());
    settings.setValue("lineAreaBackgroundColor", lineAreaBackgroundColor.name());
    settings.setValue("currentLineColor", currentLineColor.name());
    settings.setValue("textEditBackgroundColor", textEditBackgroundColor.name());
    settings.setValue("textColor", textColor.name());
    settings.endGroup();
}
