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
#ifndef _IEDITOR_OPTIONDIALOG_H_
#define _IEDITOR_OPTIONDIALOG_H_

#include <QDialog>

class QPushButton;
class QGroupBox;
class QRadioButton;
class QComboBox;
class QSpinBox;
class QSlider;
class QCheckBox;
class QLabel;
class QLineEdit;
class QTextEdit;
class QListWidget;
class QStackedLayout;
class QColorDialog;
class QFontDialog;
class QFont;
class Config;

class OptionDialog final : public QDialog {
   Q_OBJECT
public:
   explicit OptionDialog(QWidget* parent = nullptr);
   OptionDialog(const OptionDialog&) = delete;
   OptionDialog& operator=(const OptionDialog&) = delete;
   OptionDialog(OptionDialog&&) = delete;
   virtual ~OptionDialog();

signals:
   void configChangedSignal();
   void applyPushButtonEnableSignal();

private slots:
   void editerPagePushButtonClickedSlot();
   void colorStyleChangedSlot(const QString&);
   void colorStylePagePushButtonClickedSlot();
   void compilerPagePushButtonClickedSlot();
   void applySlot();
   void okSlot();
   void lockConfigCheckBoxSlot(bool);
   void setApplyPushButtonEnableSlot();

private:
   void createEditerPage();
   void createColorStylePage();
   void createCodeStylePage();
   void createCompilerPage();
   void createOtherPage();
   void optionSetup();
   void editerPageSetup();
   void colorStylePageSetup();
   void codeStylePageSetup();
   void compilerPageSetup();
   void roadingColorConfig(const QString& layoutName);

   Config* config_;

   // main
   QStackedLayout* stackedLayout_;
   QListWidget* listWidget_;
   QWidget* editerPage_;
   QWidget* colorStylePage_;
   QWidget* codeStylePage_;
   QWidget* compilerPage_;
   QPushButton* applyButton_;
   QPushButton* cancelButton_;
   QPushButton* okButton_;

   // EditerPage
   QLabel* textFontLabel_;
   QLabel* tabWideLabel_;
   QPushButton* textFontPushButton_;
   QSpinBox* tabWideSpinBox_;
   QCheckBox* simpleTextCheckBox_;
   QCheckBox* autoIndentCheckBox_;
   QCheckBox* showRowNumberCheckBox_;
   QCheckBox* highlightCurRowCheckBox_;
   QCheckBox* intelliSenseCheckBox_;
   QGroupBox* fontLayouGB_;
   QGroupBox* editOptionGB_;

   // ColorStylePage
   QLabel* colorStyleLabel_;
   QLabel* predefineAndMacroColorLabel_;
   QLabel* keywordColorLabel_;
   QLabel* functionColorLabel_;
   QLabel* stringColorLabel_;
   QLabel* headFileColorLabel_;
   QLabel* commentColorLabel_;
   QLabel* numberColorLabel_;
   QLabel* lineNumberColorLabel_;
   QLabel* lineAreaBackgroundColorLabel_;
   QLabel* curLineColorLabel_;
   QLabel* backgroundColorLabel_;
   QLabel* textColorLabel_;
   QPushButton* predefineAndMacroColorPushButton_;
   QPushButton* keywordColorPushButton_;
   QPushButton* functionColorPushButton_;
   QPushButton* stringColorPushButton_;
   QPushButton* headFileColorPushButton_;
   QPushButton* commentColorPushButton_;
   QPushButton* numberColorPushButton_;
   QPushButton* lineNumberColorPushButton_;
   QPushButton* lineAreaBackgroundColorPushButton_;
   QPushButton* curLineColorPushButton_;
   QPushButton* backgroundColorPushButton_;
   QPushButton* textColorPushButton_;
   QComboBox* colorStyleComboBox_;
   QGroupBox* colorStyleGB_;

   // CodeStylePage
   QLabel* codeStyleLabel_;
   QComboBox* codeStyleComboBox_;
   QGroupBox* codeStyleGB_;
   QTextEdit* codeStyleTextEdit_;

   // CompilerPage
   QLabel* terminalPathLabel_;
   QLabel* gccPathLabel_;
   QLabel* gppPathLabel_;
   QLabel* gdbPathLabel_;
   QLabel* terminalArgumentLabel_;
   QLabel* compilerArgumentLabel_;
   QLabel* debugerArgumentLabel_;
   QLineEdit* terminalPathLineEdit_;
   QLineEdit* gccPathLineEdit_;
   QLineEdit* gppPathLineEdit_;
   QLineEdit* gdbPathLineEdit_;
   QLineEdit* terminalArgumentLineEdit_;
   QLineEdit* compilerArgumentLineEdit_;
   QLineEdit* debugerArgumentLineEdit_;
   QCheckBox* lockConfigCheckBox_;
   QPushButton* terminalPathPushButton_;
   QPushButton* gccPathPushButton_;
   QPushButton* gppPathPushButton_;
   QPushButton* gdbPathPushButton_;
   QGroupBox* compileAndDebugGB_;
   QGroupBox* argumentOptionGB_;
};

#endif  // _IEDITOR_OPTIONDIALOG_H_
