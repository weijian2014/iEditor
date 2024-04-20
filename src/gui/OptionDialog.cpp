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
#include "OptionDialog.h"
#include "Config.h"
#include "qlogging.h"

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
#include <memory>

static const QString TerminalPathPushButtonObjectNameC(
    "terminalPathPushButton");
static const QString GccPathPushButtonObjectNameC("gccPathPushButton");
static const QString GppPathPushButtonObjectNameC("gppPathPushButton");
static const QString GdbPathPushButtonObjectNameC("gdbPathPushButton");

OptionDialog::OptionDialog(QWidget* parent)
    : QDialog(parent),
      config_(common::Singleton<Config>::instancePointer()),
      stackedLayout_(nullptr),
      listWidget_(nullptr),
      editerPage_(nullptr),
      colorStylePage_(nullptr),
      codeStylePage_(nullptr),
      compilerPage_(nullptr),
      applyButton_(nullptr),
      cancelButton_(nullptr),
      okButton_(nullptr),
      textFontLabel_(nullptr),
      tabWideLabel_(nullptr),
      textFontPushButton_(nullptr),
      tabWideSpinBox_(nullptr),
      simpleTextCheckBox_(nullptr),
      autoIndentCheckBox_(nullptr),
      showRowNumberCheckBox_(nullptr),
      highlightCurRowCheckBox_(nullptr),
      intelliSenseCheckBox_(nullptr),
      fontLayouGB_(nullptr),
      editOptionGB_(nullptr),
      colorStyleLabel_(nullptr),
      predefineAndMacroColorLabel_(nullptr),
      keywordColorLabel_(nullptr),
      functionColorLabel_(nullptr),
      stringColorLabel_(nullptr),
      headFileColorLabel_(nullptr),
      commentColorLabel_(nullptr),
      numberColorLabel_(nullptr),
      lineNumberColorLabel_(nullptr),
      lineAreaBackgroundColorLabel_(nullptr),
      curLineColorLabel_(nullptr),
      backgroundColorLabel_(nullptr),
      textColorLabel_(nullptr),
      predefineAndMacroColorPushButton_(nullptr),
      keywordColorPushButton_(nullptr),
      functionColorPushButton_(nullptr),
      stringColorPushButton_(nullptr),
      headFileColorPushButton_(nullptr),
      commentColorPushButton_(nullptr),
      numberColorPushButton_(nullptr),
      lineNumberColorPushButton_(nullptr),
      lineAreaBackgroundColorPushButton_(nullptr),
      curLineColorPushButton_(nullptr),
      backgroundColorPushButton_(nullptr),
      textColorPushButton_(nullptr),
      colorStyleComboBox_(nullptr),
      colorStyleGB_(nullptr),
      codeStyleLabel_(nullptr),
      codeStyleComboBox_(nullptr),
      codeStyleGB_(nullptr),
      codeStyleTextEdit_(nullptr),
      terminalPathLabel_(nullptr),
      gccPathLabel_(nullptr),
      gppPathLabel_(nullptr),
      gdbPathLabel_(nullptr),
      terminalArgumentLabel_(nullptr),
      compilerArgumentLabel_(nullptr),
      debugerArgumentLabel_(nullptr),
      terminalPathLineEdit_(nullptr),
      gccPathLineEdit_(nullptr),
      gppPathLineEdit_(nullptr),
      gdbPathLineEdit_(nullptr),
      terminalArgumentLineEdit_(nullptr),
      compilerArgumentLineEdit_(nullptr),
      debugerArgumentLineEdit_(nullptr),
      lockConfigCheckBox_(nullptr),
      terminalPathPushButton_(nullptr),
      gccPathPushButton_(nullptr),
      gppPathPushButton_(nullptr),
      gdbPathPushButton_(nullptr),
      compileAndDebugGB_(nullptr),
      argumentOptionGB_(nullptr) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());

   setMinimumWidth(580);

   listWidget_ = new QListWidget(this);
   listWidget_->addItem(
       new QListWidgetItem(QIcon(":/images/editerPage.png"), tr(" 编辑设置")));
   listWidget_->addItem(
       new QListWidgetItem(QIcon(":/images/tinterPage.png"), tr(" 颜色样式")));
   listWidget_->addItem(new QListWidgetItem(QIcon(":/images/codeStylePage.png"),
                                            tr(" 代码风格")));
   listWidget_->addItem(new QListWidgetItem(QIcon(":/images/compilerPage.png"),
                                            tr(" 编译调试")));
   listWidget_->setFixedWidth(125);
   createEditerPage();
   createColorStylePage();
   createCodeStylePage();
   createCompilerPage();
   applyButton_ = new QPushButton(tr("应用"), this);
   cancelButton_ = new QPushButton(tr("取消"), this);
   okButton_ = new QPushButton(tr("确定"), this);

   QHBoxLayout* buttonHBLayout = new QHBoxLayout;
   buttonHBLayout->addStretch();
   buttonHBLayout->addWidget(applyButton_);
   buttonHBLayout->addWidget(cancelButton_);
   buttonHBLayout->addWidget(okButton_);

   stackedLayout_ = new QStackedLayout;
   stackedLayout_->addWidget(editerPage_);
   stackedLayout_->addWidget(colorStylePage_);
   stackedLayout_->addWidget(codeStylePage_);
   stackedLayout_->addWidget(compilerPage_);

   QGridLayout* mainLayout = new QGridLayout;
   mainLayout->setColumnStretch(0, 1);
   mainLayout->setColumnStretch(1, 3);
   mainLayout->addWidget(listWidget_, 0, 0);
   mainLayout->addLayout(stackedLayout_, 0, 1);
   mainLayout->addLayout(buttonHBLayout, 1, 0, 1, 2);
   setLayout(mainLayout);

   raise();
   activateWindow();
   setModal(true);
   optionSetup();
}

OptionDialog::~OptionDialog() {}

void OptionDialog::createEditerPage() {
   textFontLabel_ = new QLabel(tr("文本字体"), this);
   textFontPushButton_ = new QPushButton(this);
   textFontPushButton_->setPalette(QPalette(QColor("#F07746")));
   textFontPushButton_->setAutoFillBackground(true);
   textFontPushButton_->setFlat(true);

   tabWideLabel_ = new QLabel(tr("Tab宽度"), this);
   tabWideSpinBox_ = new QSpinBox(this);
   tabWideSpinBox_->setRange(0, 20);
   simpleTextCheckBox_ = new QCheckBox(tr("简单文本模式"), this);
   simpleTextCheckBox_->setToolTip(
       tr("简单文本模式将不会对编辑区的文本内容进行高亮样式渲染"));
   showRowNumberCheckBox_ = new QCheckBox(tr("显示行号"), this);
   highlightCurRowCheckBox_ = new QCheckBox(tr("高亮当前行"), this);
   autoIndentCheckBox_ = new QCheckBox(tr("代码智能缩进"), this);
   autoIndentCheckBox_->setEnabled(false);
   intelliSenseCheckBox_ = new QCheckBox(tr("代码智能提示"), this);
   intelliSenseCheckBox_->setEnabled(false);

   QHBoxLayout* textFontHBLayout = new QHBoxLayout;
   textFontHBLayout->addWidget(textFontLabel_);
   textFontHBLayout->addWidget(textFontPushButton_);

   QVBoxLayout* fontVBoxLayout = new QVBoxLayout;
   fontVBoxLayout->addLayout(textFontHBLayout);
   fontLayouGB_ = new QGroupBox(tr("字体样式"), this);
   fontLayouGB_->setLayout(fontVBoxLayout);

   QHBoxLayout* tabWideHBLayout = new QHBoxLayout;
   tabWideHBLayout->addWidget(tabWideLabel_);
   tabWideHBLayout->addWidget(tabWideSpinBox_);
   QGridLayout* editOptionGLayout = new QGridLayout;
   editOptionGLayout->addWidget(highlightCurRowCheckBox_, 0, 0);
   editOptionGLayout->addWidget(showRowNumberCheckBox_, 0, 1);
   editOptionGLayout->addWidget(intelliSenseCheckBox_, 1, 0);
   editOptionGLayout->addWidget(autoIndentCheckBox_, 1, 1);
   editOptionGLayout->addWidget(simpleTextCheckBox_, 2, 0);
   editOptionGLayout->addLayout(tabWideHBLayout, 2, 1);
   editOptionGB_ = new QGroupBox(tr("编辑选项"), this);
   editOptionGB_->setLayout(editOptionGLayout);

   QVBoxLayout* editerPageLayout = new QVBoxLayout;
   editerPageLayout->addWidget(fontLayouGB_);
   editerPageLayout->addWidget(editOptionGB_);
   editerPageLayout->addStretch();

   editerPage_ = new QWidget(this);
   editerPage_->setLayout(editerPageLayout);
}

void OptionDialog::createColorStylePage() {
   colorStyleLabel_ = new QLabel(tr("预设"), this);
   colorStyleComboBox_ = new QComboBox(this);
   QStringList items;
   items << "Default"
         << "DarkVim"
         << "Grayscale"
         << "Inkpot"
         << "Custom";
   colorStyleComboBox_->addItems(items);
   connect(colorStyleComboBox_, SIGNAL(currentTextChanged(QString)), this,
           SLOT(colorStyleChangedSlot(QString)));

   predefineAndMacroColorLabel_ = new QLabel(tr("预定义"), this);
   predefineAndMacroColorPushButton_ = new QPushButton(this);
   predefineAndMacroColorPushButton_->setFlat(true);
   predefineAndMacroColorPushButton_->setAutoFillBackground(true);
   connect(predefineAndMacroColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   keywordColorLabel_ = new QLabel(tr("关键字"), this);
   keywordColorPushButton_ = new QPushButton(this);
   keywordColorPushButton_->setFlat(true);
   keywordColorPushButton_->setAutoFillBackground(true);
   connect(keywordColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   functionColorLabel_ = new QLabel(tr("函数名"), this);
   functionColorPushButton_ = new QPushButton(this);
   functionColorPushButton_->setFlat(true);
   functionColorPushButton_->setAutoFillBackground(true);
   connect(functionColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   stringColorLabel_ = new QLabel(tr("字符串"), this);
   stringColorPushButton_ = new QPushButton(this);
   stringColorPushButton_->setFlat(true);
   stringColorPushButton_->setAutoFillBackground(true);
   connect(stringColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   headFileColorLabel_ = new QLabel(tr("头文件"), this);
   headFileColorPushButton_ = new QPushButton(this);
   headFileColorPushButton_->setFlat(true);
   headFileColorPushButton_->setAutoFillBackground(true);
   connect(headFileColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   commentColorLabel_ = new QLabel(tr("注释"), this);
   commentColorPushButton_ = new QPushButton(this);
   commentColorPushButton_->setFlat(true);
   commentColorPushButton_->setAutoFillBackground(true);
   connect(commentColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   numberColorLabel_ = new QLabel(tr("数字"), this);
   numberColorPushButton_ = new QPushButton(this);
   numberColorPushButton_->setFlat(true);
   numberColorPushButton_->setAutoFillBackground(true);
   connect(numberColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   lineNumberColorLabel_ = new QLabel(tr("行号"), this);
   lineNumberColorPushButton_ = new QPushButton(this);
   lineNumberColorPushButton_->setFlat(true);
   lineNumberColorPushButton_->setAutoFillBackground(true);
   connect(lineNumberColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   lineAreaBackgroundColorLabel_ = new QLabel(tr("行背景"), this);
   lineAreaBackgroundColorPushButton_ = new QPushButton(this);
   lineAreaBackgroundColorPushButton_->setFlat(true);
   lineAreaBackgroundColorPushButton_->setAutoFillBackground(true);
   connect(lineAreaBackgroundColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   curLineColorLabel_ = new QLabel(tr("当前行"), this);
   curLineColorPushButton_ = new QPushButton(this);
   curLineColorPushButton_->setFlat(true);
   curLineColorPushButton_->setAutoFillBackground(true);
   connect(curLineColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   backgroundColorLabel_ = new QLabel(tr("背景"), this);
   backgroundColorPushButton_ = new QPushButton(this);
   backgroundColorPushButton_->setFlat(true);
   backgroundColorPushButton_->setAutoFillBackground(true);
   connect(backgroundColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   textColorLabel_ = new QLabel(tr("文本"), this);
   textColorPushButton_ = new QPushButton(this);
   textColorPushButton_->setFlat(true);
   textColorPushButton_->setAutoFillBackground(true);
   connect(textColorPushButton_, SIGNAL(clicked()), this,
           SLOT(colorStylePagePushButtonClickedSlot()));

   QHBoxLayout* colorComboxBoxHBLayout = new QHBoxLayout;
   colorComboxBoxHBLayout->addWidget(colorStyleLabel_);
   colorComboxBoxHBLayout->addWidget(colorStyleComboBox_);
   // colorComboxBoxHBLayout->addStretch();  // 在行后加弹性填充，使控件往左靠
   colorComboxBoxHBLayout->setAlignment(Qt::AlignmentFlag::AlignLeft);

   QGridLayout* colorGridLayout = new QGridLayout;
   colorGridLayout->setHorizontalSpacing(10);

   // 第1行的6列
   colorGridLayout->addWidget(predefineAndMacroColorLabel_, 0, 0,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(predefineAndMacroColorPushButton_, 0, 1,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(keywordColorLabel_, 0, 2,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(keywordColorPushButton_, 0, 3,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(functionColorLabel_, 0, 4,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(functionColorPushButton_, 0, 5,
                              Qt::AlignmentFlag::AlignLeft);

   // 第2行的6列
   colorGridLayout->addWidget(stringColorLabel_, 1, 0,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(stringColorPushButton_, 1, 1,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(headFileColorLabel_, 1, 2,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(headFileColorPushButton_, 1, 3,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(commentColorLabel_, 1, 4,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(commentColorPushButton_, 1, 5,
                              Qt::AlignmentFlag::AlignLeft);

   // 第3行的6列
   colorGridLayout->addWidget(numberColorLabel_, 2, 0,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(numberColorPushButton_, 2, 1,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(lineNumberColorLabel_, 2, 2,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(lineNumberColorPushButton_, 2, 3,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(lineAreaBackgroundColorLabel_, 2, 4,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(lineAreaBackgroundColorPushButton_, 2, 5,
                              Qt::AlignmentFlag::AlignLeft);

   // 第4行的6列
   colorGridLayout->addWidget(curLineColorLabel_, 3, 0,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(curLineColorPushButton_, 3, 1,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(backgroundColorLabel_, 3, 2,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(backgroundColorPushButton_, 3, 3,
                              Qt::AlignmentFlag::AlignLeft);

   colorGridLayout->addWidget(textColorLabel_, 3, 4,
                              Qt::AlignmentFlag::AlignRight);
   colorGridLayout->addWidget(textColorPushButton_, 3, 5,
                              Qt::AlignmentFlag::AlignLeft);

   QVBoxLayout* colorVBoxLayout = new QVBoxLayout;
   colorVBoxLayout->addLayout(colorComboxBoxHBLayout);
   colorVBoxLayout->addSpacing(10);
   colorVBoxLayout->addLayout(colorGridLayout);
   // colorVBoxLayout->addStretch();  // 在最后一列后加弹性填充，使控件往上靠
   colorVBoxLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

   colorStyleGB_ = new QGroupBox(tr("配色方案"), this);
   colorStyleGB_->setLayout(colorVBoxLayout);

   QVBoxLayout* colorStylePageLayout = new QVBoxLayout;
   colorStylePageLayout->addWidget(colorStyleGB_);

   colorStylePage_ = new QWidget(this);
   colorStylePage_->setLayout(colorStylePageLayout);
}

void OptionDialog::createCodeStylePage() {
   codeStyleLabel_ = new QLabel(tr("预设方案"), this);
   QStringList items;
   items << "Qt"
         << "GNU"
         << "JAVA";
   codeStyleComboBox_ = new QComboBox(this);
   codeStyleComboBox_->addItems(items);
   codeStyleTextEdit_ = new QTextEdit("<h4>暂不实现</h4>", this);

   QHBoxLayout* codeComboBoxHBLayout = new QHBoxLayout;
   codeComboBoxHBLayout->addWidget(codeStyleLabel_);
   codeComboBoxHBLayout->addWidget(codeStyleComboBox_);
   codeComboBoxHBLayout->addStretch();

   QVBoxLayout* codeVBLayout = new QVBoxLayout;
   codeVBLayout->addLayout(codeComboBoxHBLayout);
   codeVBLayout->addWidget(codeStyleTextEdit_);
   codeStyleGB_ = new QGroupBox(tr("代码风格"), this);
   codeStyleGB_->setLayout(codeVBLayout);

   QVBoxLayout* codeStylePageVBLayout = new QVBoxLayout;
   codeStylePageVBLayout->addWidget(codeStyleGB_);
   codeStylePage_ = new QWidget(this);
   codeStylePage_->setLayout(codeStylePageVBLayout);
}

void OptionDialog::createCompilerPage() {
   terminalPathLabel_ = new QLabel(tr("终端"), this);
   gccPathLabel_ = new QLabel(tr("GCC"), this);
   gppPathLabel_ = new QLabel(tr("G++"), this);
   gdbPathLabel_ = new QLabel(tr("GDB"), this);
   terminalArgumentLabel_ = new QLabel(tr("终端"), this);
   compilerArgumentLabel_ = new QLabel(tr("编译器"), this);
   debugerArgumentLabel_ = new QLabel(tr("调试器"), this);

   terminalPathPushButton_ = new QPushButton(tr("浏览"), this);
   terminalPathPushButton_->setObjectName(TerminalPathPushButtonObjectNameC);
   gccPathPushButton_ = new QPushButton(tr("浏览"), this);
   gccPathPushButton_->setObjectName(GccPathPushButtonObjectNameC);
   gppPathPushButton_ = new QPushButton(tr("浏览"), this);
   gppPathPushButton_->setObjectName(GppPathPushButtonObjectNameC);
   gdbPathPushButton_ = new QPushButton(tr("浏览"), this);
   gdbPathPushButton_->setObjectName(GdbPathPushButtonObjectNameC);

   terminalPathLineEdit_ = new QLineEdit(this);
   gccPathLineEdit_ = new QLineEdit(this);
   gppPathLineEdit_ = new QLineEdit(this);
   gdbPathLineEdit_ = new QLineEdit(this);
   terminalPathLineEdit_->setReadOnly(true);
   gccPathLineEdit_->setReadOnly(true);
   gppPathLineEdit_->setReadOnly(true);
   gdbPathLineEdit_->setReadOnly(true);
   terminalArgumentLineEdit_ = new QLineEdit(this);
   compilerArgumentLineEdit_ = new QLineEdit(this);
   debugerArgumentLineEdit_ = new QLineEdit(this);

   lockConfigCheckBox_ = new QCheckBox(tr("锁定配置"), this);
   connect(lockConfigCheckBox_, SIGNAL(clicked(bool)), this,
           SLOT(lockConfigCheckBoxSlot(bool)));
   connect(terminalPathPushButton_, SIGNAL(clicked()), this,
           SLOT(compilerPagePushButtonClickedSlot()));
   connect(gppPathPushButton_, SIGNAL(clicked()), this,
           SLOT(compilerPagePushButtonClickedSlot()));
   connect(gccPathPushButton_, SIGNAL(clicked()), this,
           SLOT(compilerPagePushButtonClickedSlot()));
   connect(gdbPathPushButton_, SIGNAL(clicked()), this,
           SLOT(compilerPagePushButtonClickedSlot()));
   connect(terminalArgumentLineEdit_, SIGNAL(textChanged(QString)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(compilerArgumentLineEdit_, SIGNAL(textChanged(QString)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(debugerArgumentLineEdit_, SIGNAL(textChanged(QString)), this,
           SLOT(setApplyPushButtonEnableSlot()));

   QGridLayout* gridLayout1 = new QGridLayout;
   gridLayout1->setColumnStretch(1,
                                 100);  // 第二列可由窗口宽度大小进行宽度的伸缩

   // 路径设置 第1行
   gridLayout1->addWidget(terminalPathLabel_, 0, 0,
                          Qt::AlignmentFlag::AlignRight);
   gridLayout1->addWidget(terminalPathLineEdit_, 0, 1);
   gridLayout1->addWidget(terminalPathPushButton_, 0, 2);

   // 路径设置 第2行
   gridLayout1->addWidget(gccPathLabel_, 1, 0, Qt::AlignmentFlag::AlignRight);
   gridLayout1->addWidget(gccPathLineEdit_, 1, 1);
   gridLayout1->addWidget(gccPathPushButton_, 1, 2);

   // 路径设置 第3行
   gridLayout1->addWidget(gppPathLabel_, 2, 0, Qt::AlignmentFlag::AlignRight);
   gridLayout1->addWidget(gppPathLineEdit_, 2, 1);
   gridLayout1->addWidget(gppPathPushButton_, 2, 2);

   // 路径设置 第4行
   gridLayout1->addWidget(gdbPathLabel_, 3, 0, Qt::AlignmentFlag::AlignRight);
   gridLayout1->addWidget(gdbPathLineEdit_, 3, 1);
   gridLayout1->addWidget(gdbPathPushButton_, 3, 2);

   compileAndDebugGB_ = new QGroupBox(tr("路径设置"), this);
   compileAndDebugGB_->setLayout(gridLayout1);

   QGridLayout* gridLayout2 = new QGridLayout;
   gridLayout2->setColumnStretch(1,
                                 100);  // 第二列可由窗口宽度大小进行宽度的伸缩

   // 参数设置 第1行
   gridLayout2->addWidget(terminalArgumentLabel_, 0, 0,
                          Qt::AlignmentFlag::AlignRight);
   gridLayout2->addWidget(terminalArgumentLineEdit_, 0, 1);

   // 参数设置 第2行
   gridLayout2->addWidget(compilerArgumentLabel_, 1, 0,
                          Qt::AlignmentFlag::AlignRight);
   gridLayout2->addWidget(compilerArgumentLineEdit_, 1, 1);

   // 参数设置 第3行
   gridLayout2->addWidget(debugerArgumentLabel_, 2, 0,
                          Qt::AlignmentFlag::AlignRight);
   gridLayout2->addWidget(debugerArgumentLineEdit_, 2, 1);

   argumentOptionGB_ = new QGroupBox(tr("参数设置"), this);
   argumentOptionGB_->setLayout(gridLayout2);

   QVBoxLayout* compilerPageVBLayout = new QVBoxLayout;
   compilerPageVBLayout->addWidget(lockConfigCheckBox_);
   compilerPageVBLayout->addWidget(compileAndDebugGB_);
   compilerPageVBLayout->addWidget(argumentOptionGB_);
   compilerPage_ = new QWidget(this);
   compilerPage_->setLayout(compilerPageVBLayout);
}

void OptionDialog::optionSetup() {
   setWindowTitle(tr("首选项"));
   setObjectName("optionDialog");

   editerPageSetup();
   colorStylePageSetup();
   codeStylePageSetup();
   compilerPageSetup();
   applyButton_->setEnabled(false);

   connect(applyButton_, SIGNAL(clicked()), this, SLOT(applySlot()));
   connect(cancelButton_, SIGNAL(clicked()), this, SLOT(hide()));
   connect(okButton_, SIGNAL(clicked()), this, SLOT(okSlot()));
   connect(listWidget_, SIGNAL(currentRowChanged(int)), stackedLayout_,
           SLOT(setCurrentIndex(int)));
   connect(this, SIGNAL(applyPushButtonEnableSignal()), this,
           SLOT(setApplyPushButtonEnableSlot()));
}

void OptionDialog::editerPageSetup() {
   textFontPushButton_->setText(
       config_->textFont_.toString().split(",").first());
   textFontPushButton_->setFont(config_->textFont_);
   highlightCurRowCheckBox_->setChecked(config_->isHighlightCurrentLine_);
   showRowNumberCheckBox_->setChecked(config_->isShowRowNumber_);
   intelliSenseCheckBox_->setChecked(config_->isIntelliSense_);
   autoIndentCheckBox_->setChecked(config_->isAutoIndent_);
   simpleTextCheckBox_->setChecked(config_->isSimpleTextMode_);
   tabWideSpinBox_->setValue(config_->tabWide_);

   connect(textFontPushButton_, SIGNAL(clicked()), this,
           SLOT(editerPagePushButtonClickedSlot()));
   connect(highlightCurRowCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(showRowNumberCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(intelliSenseCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(autoIndentCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(simpleTextCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
   connect(tabWideSpinBox_, SIGNAL(valueChanged(int)), this,
           SLOT(setApplyPushButtonEnableSlot()));
}

void OptionDialog::colorStylePageSetup() {
   QString colorStyleName = config_->getColorStyle();

   if ("Default" == colorStyleName) {
      colorStyleComboBox_->setCurrentIndex(0);
   } else if ("DarkVim" == colorStyleName) {
      colorStyleComboBox_->setCurrentIndex(1);
   } else if ("Grayscale" == colorStyleName) {
      colorStyleComboBox_->setCurrentIndex(2);
   } else if ("Inkpot" == colorStyleName) {
      colorStyleComboBox_->setCurrentIndex(3);
   } else if ("Custom" == colorStyleName) {
      colorStyleComboBox_->setCurrentIndex(4);
   }

   const auto& curenntColorStyle = config_->getCurrentColorStyle();
   predefineAndMacroColorPushButton_->setPalette(
       curenntColorStyle->predefineAndMacroColor);
   keywordColorPushButton_->setPalette(curenntColorStyle->keywordColor);
   functionColorPushButton_->setPalette(curenntColorStyle->functionColor);
   stringColorPushButton_->setPalette(curenntColorStyle->stringColor);
   headFileColorPushButton_->setPalette(curenntColorStyle->headFileColor);
   commentColorPushButton_->setPalette(curenntColorStyle->commentColor);
   numberColorPushButton_->setPalette(curenntColorStyle->numberColor);
   lineNumberColorPushButton_->setPalette(curenntColorStyle->lineNumberColor);
   lineAreaBackgroundColorPushButton_->setPalette(
       curenntColorStyle->lineAreaBackgroundColor);
   curLineColorPushButton_->setPalette(curenntColorStyle->currentLineColor);
   backgroundColorPushButton_->setPalette(
       curenntColorStyle->textEditBackgroundColor);
   textColorPushButton_->setPalette(curenntColorStyle->textColor);
}

void OptionDialog::codeStylePageSetup() { codeStyleGB_->setEnabled(false); }

void OptionDialog::compilerPageSetup() {
   terminalPathLineEdit_->setText(config_->terminalPath_);
   gccPathLineEdit_->setText(config_->gccPath_);
   gppPathLineEdit_->setText(config_->gppPath_);
   gdbPathLineEdit_->setText(config_->gdbPath_);
   terminalArgumentLineEdit_->setText(config_->terminalArgument_);
   compilerArgumentLineEdit_->setText(config_->compilerArgument_);
   debugerArgumentLineEdit_->setText(config_->debugerArgument_);
   lockConfigCheckBox_->setChecked(config_->isLockCompilerPage_);
   compileAndDebugGB_->setEnabled(!(config_->isLockCompilerPage_));
   argumentOptionGB_->setEnabled(!(config_->isLockCompilerPage_));

   connect(lockConfigCheckBox_, SIGNAL(toggled(bool)), this,
           SLOT(setApplyPushButtonEnableSlot()));
}

void OptionDialog::editerPagePushButtonClickedSlot() {
   QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
   bool isOk;
   QFont font = QFontDialog::getFont(&isOk, clickedButton->font(), this,
                                     clickedButton->objectName());

   if (!isOk) {
      return;
   }

   clickedButton->setText(font.toString().split(",").first());
   clickedButton->setFont(font);
   emit applyPushButtonEnableSignal();
}

void OptionDialog::colorStylePagePushButtonClickedSlot() {
   QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
   QColor color =
       QColorDialog::getColor(clickedButton->palette().color(QPalette::Button),
                              this, clickedButton->objectName());

   if (!color.isValid()) return;

   clickedButton->setPalette(color);
   emit applyPushButtonEnableSignal();
}

void OptionDialog::compilerPagePushButtonClickedSlot() {
   QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

   QString filePath =
       QFileDialog::getOpenFileName(this, clickedButton->objectName());

   if (filePath.isEmpty()) {
      return;
   }

   QString objName = clickedButton->objectName();
   if (objName == TerminalPathPushButtonObjectNameC) {
      terminalPathLineEdit_->setText(filePath);
   } else if (objName == GccPathPushButtonObjectNameC) {
      gccPathLineEdit_->setText(filePath);
   } else if (objName == GppPathPushButtonObjectNameC) {
      gppPathLineEdit_->setText(filePath);
   } else if (objName == GdbPathPushButtonObjectNameC) {
      gdbPathLineEdit_->setText(filePath);
   } else {
      qWarning() << "Unknown object name" << objName;
   }

   emit applyPushButtonEnableSignal();
}

void OptionDialog::roadingColorConfig(const QString& layoutName) {
   const std::shared_ptr<ColorStyle> colorStyle =
       config_->getColorStyleByName(layoutName);

   if (colorStyle) {
      predefineAndMacroColorPushButton_->setPalette(
          colorStyle->predefineAndMacroColor);
      keywordColorPushButton_->setPalette(colorStyle->keywordColor);
      functionColorPushButton_->setPalette(colorStyle->functionColor);
      stringColorPushButton_->setPalette(colorStyle->stringColor);
      headFileColorPushButton_->setPalette(colorStyle->headFileColor);
      commentColorPushButton_->setPalette(colorStyle->commentColor);
      numberColorPushButton_->setPalette(colorStyle->numberColor);
      lineNumberColorPushButton_->setPalette(colorStyle->lineNumberColor);
      lineAreaBackgroundColorPushButton_->setPalette(
          colorStyle->lineAreaBackgroundColor);
      curLineColorPushButton_->setPalette(colorStyle->currentLineColor);
      backgroundColorPushButton_->setPalette(
          colorStyle->textEditBackgroundColor);
      textColorPushButton_->setPalette(colorStyle->textColor);
      // qInfo() << "Roading color style" << layoutName << "ok";
   } else {
      qWarning() << "No color style" << layoutName;
   }
}

void OptionDialog::colorStyleChangedSlot(const QString& styleName) {
   roadingColorConfig(styleName.toLocal8Bit());
   emit applyPushButtonEnableSignal();
}

void OptionDialog::lockConfigCheckBoxSlot(bool flags) {
   compileAndDebugGB_->setEnabled(!flags);
   argumentOptionGB_->setEnabled(!flags);
}

void OptionDialog::setApplyPushButtonEnableSlot() {
   applyButton_->setEnabled(true);
}

void OptionDialog::applySlot() {
   // EditerPage
   config_->textFont_ = textFontPushButton_->font();
   config_->isShowRowNumber_ = showRowNumberCheckBox_->isChecked();
   config_->isHighlightCurrentLine_ = highlightCurRowCheckBox_->isChecked();
   config_->isAutoIndent_ = autoIndentCheckBox_->isChecked();
   config_->isIntelliSense_ = intelliSenseCheckBox_->isChecked();
   config_->isSimpleTextMode_ = simpleTextCheckBox_->isChecked();
   config_->tabWide_ = tabWideSpinBox_->value();

   // ColorStylePage
   config_->setColorStyle(colorStyleComboBox_->currentText());
   const auto& curenntColorStyle = config_->getCurrentColorStyle();
   curenntColorStyle->predefineAndMacroColor =
       predefineAndMacroColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->keywordColor =
       keywordColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->functionColor =
       functionColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->stringColor =
       stringColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->headFileColor =
       headFileColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->commentColor =
       commentColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->numberColor =
       numberColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->lineNumberColor =
       lineNumberColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->lineAreaBackgroundColor =
       lineAreaBackgroundColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->currentLineColor =
       curLineColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->textEditBackgroundColor =
       backgroundColorPushButton_->palette().color(QPalette::Button);
   curenntColorStyle->textColor =
       textColorPushButton_->palette().color(QPalette::Button);

   // CodeStylePage

   // CompilerPage
   config_->isLockCompilerPage_ = lockConfigCheckBox_->isChecked();
   config_->terminalPath_ = terminalPathLineEdit_->text().toLocal8Bit();
   config_->gccPath_ = gccPathLineEdit_->text().toLocal8Bit();
   config_->gppPath_ = gppPathLineEdit_->text().toLocal8Bit();
   config_->gdbPath_ = gdbPathLineEdit_->text().toLocal8Bit();
   config_->terminalArgument_ = terminalArgumentLineEdit_->text().toLocal8Bit();
   config_->compilerArgument_ = compilerArgumentLineEdit_->text().toLocal8Bit();
   config_->debugerArgument_ = debugerArgumentLineEdit_->text().toLocal8Bit();

   emit configChangedSignal();
   applyButton_->setEnabled(false);
   okButton_->setFocus();
}

void OptionDialog::okSlot() {
   if (applyButton_->isEnabled()) {
      applySlot();
   }

   accept();
}
