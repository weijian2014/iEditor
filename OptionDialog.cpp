/****************************************************************************
**
**      Datetime:   2013-03-23 17:23
**      Author  :   韦   健
**      Titles  :   软件配置类，派生于QDialog，该类用于软件的配制，能那时的设置
**                  编辑器、着色器和一些常用的GCC、G++、以及终端的参数
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QtGui>
#include <QComboBox>
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "ConfigStruct.h"
#include "OptionDialog.h"

OptionDialog::OptionDialog(QWidget *parent) : QDialog(parent)
{
    listWidget = new QListWidget;
    listWidget->addItem(new QListWidgetItem(QIcon(":/images/editerPage.png"), tr(" 编　辑　器")));
    listWidget->addItem(new QListWidgetItem(QIcon(":/images/tinterPage.png"), tr(" 着　色　器")));
    listWidget->addItem(new QListWidgetItem(QIcon(":/images/codeStylePage.png"), tr(" 代 码  风 格")));
    listWidget->addItem(new QListWidgetItem(QIcon(":/images/compilerPage.png"), tr(" 编 译  调 试")));
    listWidget->setFixedWidth(125);
    CreateEditerPage();
    CreateTinterPage();
    CreatecodeStylePage();
    CreateCompilerPage();
    applyButton = new QPushButton(tr("应用"));
    cancelButton = new QPushButton(tr("取消"));
    okButton = new QPushButton(tr("确定"));

    QHBoxLayout *buttonHBLayout = new QHBoxLayout;
    buttonHBLayout->addStretch();
    buttonHBLayout->addWidget(applyButton);
    buttonHBLayout->addWidget(cancelButton);
    buttonHBLayout->addWidget(okButton);

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(editerPage);
    stackedLayout->addWidget(tinterPage);
    stackedLayout->addWidget(codeStylePage);
    stackedLayout->addWidget(compilerPage);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 3);
    mainLayout->addWidget(listWidget, 0, 0);
    mainLayout->addLayout(stackedLayout, 0, 1);
    mainLayout->addLayout(buttonHBLayout, 1, 0, 1, 2);
    setLayout(mainLayout);

    raise();
    activateWindow();
    setModal(true);
    OptionSetup();
}

void OptionDialog::CreateEditerPage()
{
    textFontLabel = new QLabel(tr("文本字体:"));
    textFontPushButton = new QPushButton;
    textFontPushButton->setObjectName(tr("选择文本字体-iEditor"));
    textFontPushButton->setPalette(QPalette(QColor("#F07746")));
    textFontPushButton->setAutoFillBackground(true);
    textFontPushButton->setFlat(true);

    tabWideLabel = new QLabel(tr("Tab宽度:"));
    tabWideSpinBox = new QSpinBox;
    tabWideSpinBox->setRange(0, 20);
    simpleTextCheckBox = new QCheckBox(tr("简单文本模式"));
    showRowNumberCheckBox = new QCheckBox(tr("文本显示行号"));
    highlightCurRowCheckBox = new QCheckBox(tr("高亮当前的行"));
    autoIndentCheckBox = new QCheckBox(tr("代码智能缩进"));
    autoIndentCheckBox->setEnabled(false);
    intelliSenseCheckBox = new QCheckBox(tr("代码智能提示"));
    intelliSenseCheckBox->setEnabled(false);

    QHBoxLayout *textFontHBLayout = new QHBoxLayout;
    textFontHBLayout->addWidget(textFontLabel);
    textFontHBLayout->addWidget(textFontPushButton);

    QVBoxLayout *fontVBoxLayout = new QVBoxLayout;
    fontVBoxLayout->addLayout(textFontHBLayout);
    fontLayouGB = new QGroupBox(tr("字体样式"));
    fontLayouGB->setLayout(fontVBoxLayout);

    QHBoxLayout *tabWideHBLayou = new QHBoxLayout;
    tabWideHBLayou->addWidget(tabWideLabel);
    tabWideHBLayou->addWidget(tabWideSpinBox);
    QGridLayout *editOptionGLayout = new QGridLayout;
    editOptionGLayout->addWidget(highlightCurRowCheckBox, 0, 0);
    editOptionGLayout->addWidget(showRowNumberCheckBox, 0, 1);
    editOptionGLayout->addWidget(intelliSenseCheckBox, 1, 0);
    editOptionGLayout->addWidget(autoIndentCheckBox, 1, 1);
    editOptionGLayout->addWidget(simpleTextCheckBox, 2, 0);
    editOptionGLayout->addLayout(tabWideHBLayou, 2, 1);
    editOptionGB = new QGroupBox(tr("编辑选项"));
    editOptionGB->setLayout(editOptionGLayout);

    QVBoxLayout *editerPageLayou = new QVBoxLayout;
    editerPageLayou->addWidget(fontLayouGB);
    editerPageLayou->addWidget(editOptionGB);
    editerPageLayou->addStretch();

    editerPage = new QWidget;
    editerPage->setLayout(editerPageLayou);
}

void OptionDialog::CreateTinterPage()
{
    colorStyleLabel = new QLabel(tr("预设方案:"));
    colorStyleComboBox = new QComboBox;
    QStringList items;
    items << tr("Default") << tr("DarkVim") << tr("Grayscale") << tr("Inkpot") << tr("Custom");
    colorStyleComboBox->addItems(items);
    connect(colorStyleComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ColorStyleChanged(QString)));

    predefineAndMacroColorLabel = new QLabel(tr("预定义:"));
    predefineAndMacroColorPushButton = new QPushButton;
    predefineAndMacroColorPushButton->setObjectName(tr("选择预定义颜色-iEditor"));
    predefineAndMacroColorPushButton->setFlat(true);
    predefineAndMacroColorPushButton->setAutoFillBackground(true);
    connect(predefineAndMacroColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    keywordColorLabel = new QLabel(tr("关键字:"));
    keywordColorPushButton = new QPushButton;
    keywordColorPushButton->setObjectName(tr("选择关键字颜色-iEditor"));
    keywordColorPushButton->setFlat(true);
    keywordColorPushButton->setAutoFillBackground(true);
    connect(keywordColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    functionColorLabel = new QLabel(tr("函数名:"));
    functionColorPushButton = new QPushButton;
    functionColorPushButton->setObjectName(tr("选择函数名颜色-iEditor"));
    functionColorPushButton->setFlat(true);
    functionColorPushButton->setAutoFillBackground(true);
    connect(functionColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    stringColorLabel = new QLabel(tr("字符串:"));
    stringColorPushButton = new QPushButton;
    stringColorPushButton->setObjectName(tr("选择字符串颜色-iEditor"));
    stringColorPushButton->setFlat(true);
    stringColorPushButton->setAutoFillBackground(true);
    connect(stringColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    headFileColorLabel = new QLabel(tr("头文件:"));
    headFileColorPushButton = new QPushButton;
    headFileColorPushButton->setObjectName(tr("选择头文件颜色-iEditor"));
    headFileColorPushButton->setFlat(true);
    headFileColorPushButton->setAutoFillBackground(true);
    connect(headFileColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    commentColorLabel = new QLabel(tr("注　释:"));
    commentColorPushButton = new QPushButton;
    commentColorPushButton->setObjectName(tr("选择注释颜色-iEditor"));
    commentColorPushButton->setFlat(true);
    commentColorPushButton->setAutoFillBackground(true);
    connect(commentColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    numberColorLabel = new QLabel(tr("数　字:"));
    numberColorPushButton = new QPushButton;
    numberColorPushButton->setObjectName(tr("选择数字颜色-iEditor"));
    numberColorPushButton->setFlat(true);
    numberColorPushButton->setAutoFillBackground(true);
    connect(numberColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    lineNumberColorLabel = new QLabel(tr("行　号:"));
    lineNumberColorPushButton = new QPushButton;
    lineNumberColorPushButton->setObjectName(tr("选择行号颜色-iEditor"));
    lineNumberColorPushButton->setFlat(true);
    lineNumberColorPushButton->setAutoFillBackground(true);
    connect(lineNumberColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    lineAreaBackgroundColorLabel = new QLabel(tr("行背景:"));
    lineAreaBackgroundColorPushButton = new QPushButton;
    lineAreaBackgroundColorPushButton->setObjectName(tr("选择行背景颜色-iEditor"));
    lineAreaBackgroundColorPushButton->setFlat(true);
    lineAreaBackgroundColorPushButton->setAutoFillBackground(true);
    connect(lineAreaBackgroundColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    curLineColorLabel = new QLabel(tr("当前行:"));
    curLineColorPushButton = new QPushButton;
    curLineColorPushButton->setObjectName(tr("选择当前行颜色-iEditor"));
    curLineColorPushButton->setFlat(true);
    curLineColorPushButton->setAutoFillBackground(true);
    connect(curLineColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    backgroundColorLabel = new QLabel(tr("背　景:"));
    backgroundColorPushButton = new QPushButton;
    backgroundColorPushButton->setObjectName(tr("选择背景颜色-iEditor"));
    backgroundColorPushButton->setFlat(true);
    backgroundColorPushButton->setAutoFillBackground(true);
    connect(backgroundColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    textColorLabel = new QLabel(tr("文　本:"));
    textColorPushButton = new QPushButton;
    textColorPushButton->setObjectName(tr("选择文本颜色-iEditor"));
    textColorPushButton->setFlat(true);
    textColorPushButton->setAutoFillBackground(true);
    connect(textColorPushButton, SIGNAL(clicked()), this, SLOT(ColorStylePagePushButtonClicked()));

    QHBoxLayout *colorComboxBoxHBLayou = new QHBoxLayout;
    colorComboxBoxHBLayou->addWidget(colorStyleLabel);
    colorComboxBoxHBLayou->addWidget(colorStyleComboBox);
    colorComboxBoxHBLayou->addStretch();

    QHBoxLayout *rowOneHBLayou = new QHBoxLayout;
    rowOneHBLayou->addWidget(predefineAndMacroColorLabel);
    rowOneHBLayou->addWidget(predefineAndMacroColorPushButton);
    rowOneHBLayou->addStretch();
    rowOneHBLayou->addWidget(keywordColorLabel);
    rowOneHBLayou->addWidget(keywordColorPushButton);
    rowOneHBLayou->addStretch();
    rowOneHBLayou->addWidget(functionColorLabel);
    rowOneHBLayou->addWidget(functionColorPushButton);

    QHBoxLayout *rowTwoHBLayou = new QHBoxLayout;
    rowTwoHBLayou->addWidget(stringColorLabel);
    rowTwoHBLayou->addWidget(stringColorPushButton);
    rowTwoHBLayou->addStretch();
    rowTwoHBLayou->addWidget(headFileColorLabel);
    rowTwoHBLayou->addWidget(headFileColorPushButton);
    rowTwoHBLayou->addStretch();
    rowTwoHBLayou->addWidget(commentColorLabel);
    rowTwoHBLayou->addWidget(commentColorPushButton);

    QHBoxLayout *rowThreeHBLayou = new QHBoxLayout;
    rowThreeHBLayou->addWidget(numberColorLabel);
    rowThreeHBLayou->addWidget(numberColorPushButton);
    rowThreeHBLayou->addStretch();
    rowThreeHBLayou->addWidget(lineNumberColorLabel);
    rowThreeHBLayou->addWidget(lineNumberColorPushButton);
    rowThreeHBLayou->addStretch();
    rowThreeHBLayou->addWidget(lineAreaBackgroundColorLabel);
    rowThreeHBLayou->addWidget(lineAreaBackgroundColorPushButton);

    QHBoxLayout *rowFourHBLayou = new QHBoxLayout;
    rowFourHBLayou->addWidget(curLineColorLabel);
    rowFourHBLayou->addWidget(curLineColorPushButton);
    rowFourHBLayou->addStretch();
    rowFourHBLayou->addWidget(backgroundColorLabel);
    rowFourHBLayou->addWidget(backgroundColorPushButton);
    rowFourHBLayou->addStretch();
    rowFourHBLayou->addWidget(textColorLabel);
    rowFourHBLayou->addWidget(textColorPushButton);

    QVBoxLayout *colorVBLayou = new QVBoxLayout;
    colorVBLayou->addLayout(colorComboxBoxHBLayou);
    colorVBLayou->addLayout(rowOneHBLayou);
    colorVBLayou->addLayout(rowTwoHBLayou);
    colorVBLayou->addLayout(rowThreeHBLayou);
    colorVBLayou->addLayout(rowFourHBLayou);
    colorVBLayou->addStretch();
    colorStyleGB = new QGroupBox(tr("配色方案"));
    colorStyleGB->setLayout(colorVBLayou);

    QVBoxLayout *tinterPageLayou = new QVBoxLayout;
    tinterPageLayou->addWidget(colorStyleGB);

    tinterPage = new QWidget;
    tinterPage->setLayout(tinterPageLayou);
}

void OptionDialog::CreatecodeStylePage()
{
    codeStyleLabel = new QLabel(tr("预设方案"));
    QStringList items;
    items << "Qt" << "GNU" << "JAVA";
    codeStyleComboBox = new QComboBox;
    codeStyleComboBox->addItems(items);
    codeStyleTextEdit = new QTextEdit("<h4>暂不实现</h4>");

    QHBoxLayout *codeComboBoxHBLayout = new QHBoxLayout;
    codeComboBoxHBLayout->addWidget(codeStyleLabel);
    codeComboBoxHBLayout->addWidget(codeStyleComboBox);
    codeComboBoxHBLayout->addStretch();

    QVBoxLayout *codeVBLayout = new QVBoxLayout;
    codeVBLayout->addLayout(codeComboBoxHBLayout);
    codeVBLayout->addWidget(codeStyleTextEdit);
    codeStyleGB = new QGroupBox(tr("代码风格"));
    codeStyleGB->setLayout(codeVBLayout);

    QVBoxLayout *codeStylePageVBLayout = new QVBoxLayout;
    codeStylePageVBLayout->addWidget(codeStyleGB);
    codeStylePage = new QWidget;
    codeStylePage->setLayout(codeStylePageVBLayout);
}

void OptionDialog::CreateCompilerPage()
{
    terminalPathLabel = new QLabel(tr("终端　路径:"));
    gccPathLabel = new QLabel(tr("ＧＣＣ路径:"));
    gppPathLabel = new QLabel(tr("Ｇ＋＋路径:"));
    gdbPathLabel = new QLabel(tr("ＧＤＢ路径:"));
    terminalArgumentLabel = new QLabel(tr("终端　参数:"));
    compilerArgumentLabel = new QLabel(tr("编译器参数:"));
    debugerArgumentLabel = new QLabel(tr("调试器参数:"));

    terminalPathPushButton = new QPushButton(tr("浏览"));
    gccPathPushButton = new QPushButton(tr("浏览"));
    gppPathPushButton = new QPushButton(tr("浏览"));
    gdbPathPushButton = new QPushButton(tr("浏览"));
    terminalPathPushButton->setObjectName(tr("选择终端路径-iEditor"));
    gppPathPushButton->setObjectName(tr("选择G++路径-iEditor"));
    gccPathPushButton->setObjectName(tr("选择GCC路径-iEditor"));
    gdbPathPushButton->setObjectName(tr("选择GDB路径-iEditor"));

    terminalPathLineEdit = new QLineEdit;
    gccPathLineEdit = new QLineEdit;
    gppPathLineEdit = new QLineEdit;
    gdbPathLineEdit = new QLineEdit;
    terminalPathLineEdit->setReadOnly(true);
    gccPathLineEdit->setReadOnly(true);
    gppPathLineEdit->setReadOnly(true);
    gdbPathLineEdit->setReadOnly(true);
    terminalArgumentLineEdit = new QLineEdit;
    compilerArgumentLineEdit = new QLineEdit;
    debugerArgumentLineEdit = new QLineEdit;

    lockConfigCheckBox = new QCheckBox(tr("锁定配置"));
    connect(lockConfigCheckBox, SIGNAL(clicked(bool)), this, SLOT(LockConfigCheckBox(bool)));
    connect(terminalPathPushButton, SIGNAL(clicked()), this, SLOT(CompilerPagePushButtonClicked()));
    connect(gppPathPushButton, SIGNAL(clicked()), this, SLOT(CompilerPagePushButtonClicked()));
    connect(gccPathPushButton, SIGNAL(clicked()), this, SLOT(CompilerPagePushButtonClicked()));
    connect(gdbPathPushButton, SIGNAL(clicked()), this, SLOT(CompilerPagePushButtonClicked()));
    connect(terminalArgumentLineEdit, SIGNAL(textChanged(QString)), this, SLOT(SetApplyPushButtonEnable()));
    connect(compilerArgumentLineEdit, SIGNAL(textChanged(QString)), this, SLOT(SetApplyPushButtonEnable()));
    connect(debugerArgumentLineEdit, SIGNAL(textChanged(QString)), this, SLOT(SetApplyPushButtonEnable()));

    QHBoxLayout *terminalPathHBLayout = new QHBoxLayout;
    terminalPathHBLayout->addWidget(terminalPathLabel);
    terminalPathHBLayout->addWidget(terminalPathLineEdit);
    terminalPathHBLayout->addWidget(terminalPathPushButton);

    QHBoxLayout *gccPathHBLayout = new QHBoxLayout;
    gccPathHBLayout->addWidget(gccPathLabel);
    gccPathHBLayout->addWidget(gccPathLineEdit);
    gccPathHBLayout->addWidget(gccPathPushButton);

    QHBoxLayout *gppPathHBLayout = new QHBoxLayout;
    gppPathHBLayout->addWidget(gppPathLabel);
    gppPathHBLayout->addWidget(gppPathLineEdit);
    gppPathHBLayout->addWidget(gppPathPushButton);

    QHBoxLayout *gdbPathHBLayout = new QHBoxLayout;
    gdbPathHBLayout->addWidget(gdbPathLabel);
    gdbPathHBLayout->addWidget(gdbPathLineEdit);
    gdbPathHBLayout->addWidget(gdbPathPushButton);

    QVBoxLayout *compileAndDebugVBLayout = new QVBoxLayout;
    compileAndDebugVBLayout->addLayout(terminalPathHBLayout);
    compileAndDebugVBLayout->addLayout(gccPathHBLayout);
    compileAndDebugVBLayout->addLayout(gppPathHBLayout);
    compileAndDebugVBLayout->addLayout(gdbPathHBLayout);

    compileAndDebugGB = new QGroupBox(tr("路径设置"));
    compileAndDebugGB->setLayout(compileAndDebugVBLayout);
    QHBoxLayout *terminalArgumentHBLayout = new QHBoxLayout;
    terminalArgumentHBLayout->addWidget(terminalArgumentLabel);
    terminalArgumentHBLayout->addWidget(terminalArgumentLineEdit);

    QHBoxLayout *compilerArgumentHBLayout = new QHBoxLayout;
    compilerArgumentHBLayout->addWidget(compilerArgumentLabel);
    compilerArgumentHBLayout->addWidget(compilerArgumentLineEdit);

    QHBoxLayout *debugerArgumentHBLayout = new QHBoxLayout;
    debugerArgumentHBLayout->addWidget(debugerArgumentLabel);
    debugerArgumentHBLayout->addWidget(debugerArgumentLineEdit);

    QVBoxLayout *argumentOptionVBLayout = new QVBoxLayout;
    argumentOptionVBLayout->addLayout(terminalArgumentHBLayout);
    argumentOptionVBLayout->addLayout(compilerArgumentHBLayout);
    argumentOptionVBLayout->addLayout(debugerArgumentHBLayout);

    argumentOptionGB = new QGroupBox(tr("参数设置"));
    argumentOptionGB->setLayout(argumentOptionVBLayout);

    QVBoxLayout *compilerPageVBLayout = new QVBoxLayout;
    compilerPageVBLayout->addWidget(lockConfigCheckBox);
    compilerPageVBLayout->addWidget(compileAndDebugGB);
    compilerPageVBLayout->addWidget(argumentOptionGB);
    compilerPage = new QWidget;
    compilerPage->setLayout(compilerPageVBLayout);
}

void OptionDialog::OptionSetup()
{
    setWindowTitle(tr("首选项-iEditor"));
    setObjectName(tr("optionDialog"));

    EditerPageSetup();
    TinterPageSetup();
    CodeStylePageSetup();
    CompilerPageSetup();
    applyButton->setEnabled(false);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(Apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(Ok()));
    connect(listWidget, SIGNAL(currentRowChanged(int)), stackedLayout, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(ApplyPushButtonEnableSignal()), this, SLOT(SetApplyPushButtonEnable()));
}

void OptionDialog::EditerPageSetup()
{
    textFontPushButton->setText(ConfigStruct::GetConfig()->textFont.toString().split(",").first());
    textFontPushButton->setFont(ConfigStruct::GetConfig()->textFont);
    highlightCurRowCheckBox->setChecked(ConfigStruct::GetConfig()->isHighlightCurrentLine);
    showRowNumberCheckBox->setChecked(ConfigStruct::GetConfig()->isShowRowNumber);
    intelliSenseCheckBox->setChecked(ConfigStruct::GetConfig()->isIntelliSense);
    autoIndentCheckBox->setChecked(ConfigStruct::GetConfig()->isAutoIndent);
    simpleTextCheckBox->setChecked(ConfigStruct::GetConfig()->isSimpleTextMode);
    tabWideSpinBox->setValue(ConfigStruct::GetConfig()->tabWide);

    connect(textFontPushButton, SIGNAL(clicked()), this, SLOT(EditerPagePushButtonClicked()));
    connect(highlightCurRowCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
    connect(showRowNumberCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
    connect(intelliSenseCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
    connect(autoIndentCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
    connect(simpleTextCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
    connect(tabWideSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetApplyPushButtonEnable()));
}

void OptionDialog::TinterPageSetup()
{
    QString colorStyleName = ConfigStruct::GetConfig()->colorStyle;

    if ("Default" == colorStyleName)
    {
        colorStyleComboBox->setCurrentIndex(0);
    } else if ("DarkVim" == colorStyleName)
    {
        colorStyleComboBox->setCurrentIndex(1);
    } else if ("Grayscale" == colorStyleName)
    {
        colorStyleComboBox->setCurrentIndex(2);
    } else if ("Inkpot" == colorStyleName)
    {
        colorStyleComboBox->setCurrentIndex(3);
    } else if ("Custom" == colorStyleName)
    {
        colorStyleComboBox->setCurrentIndex(4);
    }

    predefineAndMacroColorPushButton->setPalette(ConfigStruct::GetConfig()->predefineAndMacroColor);
    keywordColorPushButton->setPalette(ConfigStruct::GetConfig()->keywordColor);
    functionColorPushButton->setPalette(ConfigStruct::GetConfig()->functionColor);
    stringColorPushButton->setPalette(ConfigStruct::GetConfig()->stringColor);
    headFileColorPushButton->setPalette(ConfigStruct::GetConfig()->headFileColor);
    commentColorPushButton->setPalette(ConfigStruct::GetConfig()->commentColor);
    numberColorPushButton->setPalette(ConfigStruct::GetConfig()->numberColor);
    lineNumberColorPushButton->setPalette(ConfigStruct::GetConfig()->lineNumberColor);
    lineAreaBackgroundColorPushButton->setPalette(ConfigStruct::GetConfig()->lineAreaBackgroundColor);
    curLineColorPushButton->setPalette(ConfigStruct::GetConfig()->currentLineColor);
    backgroundColorPushButton->setPalette(ConfigStruct::GetConfig()->textEditBackgroundColor);
    textColorPushButton->setPalette(ConfigStruct::GetConfig()->textColor);
}

void OptionDialog::CodeStylePageSetup()
{
    codeStyleGB->setEnabled(false);
}

void OptionDialog::CompilerPageSetup()
{
    terminalPathLineEdit->setText(ConfigStruct::GetConfig()->terminalPath);
    gccPathLineEdit->setText(ConfigStruct::GetConfig()->gccPath);
    gppPathLineEdit->setText(ConfigStruct::GetConfig()->gppPath);
    gdbPathLineEdit->setText(ConfigStruct::GetConfig()->gdbPath);
    terminalArgumentLineEdit->setText(ConfigStruct::GetConfig()->terminalArgument);
    compilerArgumentLineEdit->setText(ConfigStruct::GetConfig()->compilerArgument);
    debugerArgumentLineEdit->setText(ConfigStruct::GetConfig()->debugerArgument);
    lockConfigCheckBox->setChecked(ConfigStruct::GetConfig()->isLockCompilerPage);
    compileAndDebugGB->setEnabled(!(ConfigStruct::GetConfig()->isLockCompilerPage));
    argumentOptionGB->setEnabled(!(ConfigStruct::GetConfig()->isLockCompilerPage));

    connect(lockConfigCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetApplyPushButtonEnable()));
}

void OptionDialog::EditerPagePushButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    bool isOk;
    QFont font = QFontDialog::getFont(&isOk, clickedButton->font(), this, clickedButton->objectName());

    if (!isOk)
        return;

    if ("选择文本字体-iEditor" == clickedButton->objectName())
    {
        textFontPushButton->setText(font.toString().split(",").first());
        textFontPushButton->setFont(font);
    }

    emit ApplyPushButtonEnableSignal();
}

void OptionDialog::ColorStylePagePushButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QColor color = QColorDialog::getColor(clickedButton->palette().color(QPalette::Button), this, clickedButton->objectName());

    if (!color.isValid())
        return;

    clickedButton->setPalette(color);
    emit ApplyPushButtonEnableSignal();
}

void OptionDialog::CompilerPagePushButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    QString filePath = QFileDialog::getOpenFileName(this, clickedButton->objectName());

    if (filePath.isEmpty())
        return;

    if (tr("选择终端路径-iEditor") == clickedButton->objectName())
    {
        terminalPathLineEdit->setText(filePath);
    } else if (tr("选择GCC路径-iEditor") == clickedButton->objectName())
    {
        gccPathLineEdit->setText(filePath);
    } else if (tr("选择G++路径-iEditor") == clickedButton->objectName())
    {
        gppPathLineEdit->setText(filePath);
    } else if (tr("选择GDB路径-iEditor") == clickedButton->objectName())
    {
        gdbPathLineEdit->setText(filePath);
    }

    emit ApplyPushButtonEnableSignal();
}

void OptionDialog::ReadColorConfig(const QString &layoutName)
{
    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);

    if (!settings.isWritable())
    {
        QMessageBox::critical(this, "错误提示-iEditor", "<p><font color=red>iEditor无法读取软件配置，请检查 [config.ini] 是否丢失？</font></p>");
        return;
    }

    settings.beginGroup(layoutName);
    predefineAndMacroColorPushButton->setPalette(settings.value("predefineAndMacroColor").value<QColor>());
    keywordColorPushButton->setPalette(settings.value("keywordColor").value<QColor>());
    functionColorPushButton->setPalette(settings.value("functionColor").value<QColor>());
    stringColorPushButton->setPalette(settings.value("stringColor").value<QColor>());
    headFileColorPushButton->setPalette(settings.value("headFileColor").value<QColor>());
    commentColorPushButton->setPalette(settings.value("commentColor").value<QColor>());
    numberColorPushButton->setPalette(settings.value("numberColor").value<QColor>());
    lineNumberColorPushButton->setPalette(settings.value("lineNumberColor").value<QColor>());
    lineAreaBackgroundColorPushButton->setPalette(settings.value("lineAreaBackgroundColor").value<QColor>());
    curLineColorPushButton->setPalette(settings.value("currentLineColor").value<QColor>());
    backgroundColorPushButton->setPalette(settings.value("textEditBackgroundColor").value<QColor>());
    textColorPushButton->setPalette(settings.value("textColor").value<QColor>());
    settings.endGroup();
}

void OptionDialog::ColorStyleChanged(const QString &styleName)
{
    ReadColorConfig(styleName.toLocal8Bit());
    emit ApplyPushButtonEnableSignal();
}

void OptionDialog::LockConfigCheckBox(bool flags)
{
    compileAndDebugGB->setEnabled(!flags);
    argumentOptionGB->setEnabled(!flags);
}

void OptionDialog::SetApplyPushButtonEnable()
{
    applyButton->setEnabled(true);
}

void OptionDialog::Apply()
{
    //EditerPage
    ConfigStruct::GetConfig()->textFont = textFontPushButton->font();
    ConfigStruct::GetConfig()->isShowRowNumber = showRowNumberCheckBox->isChecked();
    ConfigStruct::GetConfig()->isHighlightCurrentLine = highlightCurRowCheckBox->isChecked();
    ConfigStruct::GetConfig()->isAutoIndent = autoIndentCheckBox->isChecked();
    ConfigStruct::GetConfig()->isIntelliSense = intelliSenseCheckBox->isChecked();
    ConfigStruct::GetConfig()->isSimpleTextMode = simpleTextCheckBox->isChecked();
    ConfigStruct::GetConfig()->tabWide = tabWideSpinBox->value();

    //TinterPage
    ConfigStruct::GetConfig()->colorStyle = colorStyleComboBox->currentText().toLocal8Bit();
    ConfigStruct::GetConfig()->predefineAndMacroColor = predefineAndMacroColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->keywordColor = keywordColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->functionColor = functionColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->stringColor = stringColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->headFileColor = headFileColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->commentColor = commentColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->numberColor = numberColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->lineNumberColor = lineNumberColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->lineAreaBackgroundColor = lineAreaBackgroundColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->currentLineColor = curLineColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->textEditBackgroundColor = backgroundColorPushButton->palette().color(QPalette::Button);
    ConfigStruct::GetConfig()->textColor = textColorPushButton->palette().color(QPalette::Button);

    //codeStylePage

    //CompilerPage
    ConfigStruct::GetConfig()->isLockCompilerPage = lockConfigCheckBox->isChecked();
    ConfigStruct::GetConfig()->terminalPath = terminalPathLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->gccPath = gccPathLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->gppPath = gppPathLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->gdbPath = gdbPathLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->terminalArgument = terminalArgumentLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->compilerArgument = compilerArgumentLineEdit->text().toLocal8Bit();
    ConfigStruct::GetConfig()->debugerArgument = debugerArgumentLineEdit->text().toLocal8Bit();

    emit ConfigChangedSignal();
    applyButton->setEnabled(false);
    okButton->setFocus();
}

void OptionDialog::Ok()
{
    if (applyButton->isEnabled())
    {
        Apply();
    }

    accept();
}
