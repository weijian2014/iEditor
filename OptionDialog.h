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
#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

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

class OptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog() { }
    
signals:
    void ConfigChangedSignal();
    void ApplyPushButtonEnableSignal();
    
private slots:
    void EditerPagePushButtonClicked();
    void ColorStyleChanged(const QString &styleName);
    void ColorStylePagePushButtonClicked();
    void CompilerPagePushButtonClicked();
    void Apply();
    void Ok();
    void LockConfigCheckBox(bool);
    void SetApplyPushButtonEnable();
    
private:
    void CreateEditerPage();
    void CreateTinterPage();
    void CreatecodeStylePage();
    void CreateCompilerPage();
    void CreateOtherPage();
    void OptionSetup();
    void EditerPageSetup();
    void TinterPageSetup();
    void CodeStylePageSetup();
    void CompilerPageSetup();
    void ReadColorConfig(const QString &layoutName);
    
    //main
    QStackedLayout *stackedLayout;
    QListWidget *listWidget;
    QWidget *editerPage;
    QWidget *tinterPage;
    QWidget *codeStylePage;
    QWidget *compilerPage;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QPushButton *okButton;
    
    //editerPage
    QLabel *textFontLabel;
    QLabel *tabWideLabel;
    QPushButton *textFontPushButton;
    QSpinBox *tabWideSpinBox;
    QCheckBox *simpleTextCheckBox;
    QCheckBox *autoIndentCheckBox;
    QCheckBox *showRowNumberCheckBox;
    QCheckBox *highlightCurRowCheckBox;
    QCheckBox *intelliSenseCheckBox;
    QGroupBox *fontLayouGB;
    QGroupBox *editOptionGB;
    
    //tinterPage
    QLabel *colorStyleLabel;
    QLabel *predefineAndMacroColorLabel;
    QLabel *keywordColorLabel;
    QLabel *functionColorLabel;
    QLabel *stringColorLabel;
    QLabel *headFileColorLabel;
    QLabel *commentColorLabel;
    QLabel *numberColorLabel;
    QLabel *lineNumberColorLabel;
    QLabel *lineAreaBackgroundColorLabel;
    QLabel *curLineColorLabel;
    QLabel *backgroundColorLabel;
    QLabel *textColorLabel;
    QPushButton *predefineAndMacroColorPushButton;
    QPushButton *keywordColorPushButton;
    QPushButton *functionColorPushButton;
    QPushButton *stringColorPushButton;
    QPushButton *headFileColorPushButton;
    QPushButton *commentColorPushButton;
    QPushButton *numberColorPushButton;
    QPushButton *lineNumberColorPushButton;
    QPushButton *lineAreaBackgroundColorPushButton;
    QPushButton *curLineColorPushButton;
    QPushButton *backgroundColorPushButton;
    QPushButton *textColorPushButton;
    QComboBox *colorStyleComboBox;
    QGroupBox *colorStyleGB;

    //codeStylePage
    QLabel *codeStyleLabel;
    QComboBox *codeStyleComboBox;
    QGroupBox *codeStyleGB;
    QTextEdit *codeStyleTextEdit;
    
    //compilerPage
    QLabel *terminalPathLabel;
    QLabel *gccPathLabel;
    QLabel *gppPathLabel;
    QLabel *gdbPathLabel;
    QLabel *terminalArgumentLabel;
    QLabel *compilerArgumentLabel;
    QLabel *debugerArgumentLabel;
    QLineEdit *terminalPathLineEdit;
    QLineEdit *gccPathLineEdit;
    QLineEdit *gppPathLineEdit;
    QLineEdit *gdbPathLineEdit;
    QLineEdit *terminalArgumentLineEdit;
    QLineEdit *compilerArgumentLineEdit;
    QLineEdit *debugerArgumentLineEdit;
    QCheckBox *lockConfigCheckBox;
    QPushButton *terminalPathPushButton;
    QPushButton *gccPathPushButton;
    QPushButton *gppPathPushButton;
    QPushButton *gdbPathPushButton;
    QGroupBox *compileAndDebugGB;
    QGroupBox *argumentOptionGB;
};

#endif // OPTIONDIALOG_H
