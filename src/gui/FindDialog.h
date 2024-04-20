/****************************************************************************
**
**      Datetime:   2013-03-22 12：21
**      Author  :   韦   健
**      Titles  :   这是查找对话框的类，派生于QDialog
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_FINDDIALOG_H_
#define _IEDITOR_FINDDIALOG_H_

#include <QDialog>

class QLabel;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QGroupBox;
class QSpacerItem;
class QDialogButtonBox;

enum MyFindFlag {
   FindBackward = 1,             // 向后查找
   FindForward = 2,              // 向前查找
   FindCaseSensitively = 4,      // 大小写敏感
   FindWholeWords = 8,           // 全词匹配
   UsingRegularExpression = 16,  // 使用正则表达式
   DocumentBegin = 32,           // 从文件开始处开始查找
};

class FindDialog final : public QDialog {
   Q_OBJECT

public:
   explicit FindDialog(QWidget* parent = nullptr);
   FindDialog(const FindDialog&) = delete;
   FindDialog& operator=(const FindDialog&) = delete;
   FindDialog(FindDialog&&) = delete;
   virtual ~FindDialog();

   unsigned getFindFlags() const { return findFlags_; }

   QLineEdit* findLineEdit_;
   QLineEdit* replaceLineEdit_;

signals:
   void findForwardSignal();
   void findBackwardSignal();
   void replaceSignal();
   void replaceAllSignal();

private slots:
   void enableFindButtonSlot(const QString text);
   void findForwardClickedSlot();
   void findBackwardClickedSlot();
   void replaceClickedSlot();
   void replaceAllClickedSlot();
   void clearClickedSlot();
   void advancedButtonClickedSlot(bool);

private:
   void setTabOrder();

private:
   unsigned findFlags_;
   QLabel* findLabel_;
   QLabel* replaceLabel_;
   QPushButton* findForwardButton_;
   QPushButton* findBackwardButton_;
   QPushButton* closeButton_;
   QPushButton* replaceButton_;
   QPushButton* replaceAllButton_;
   QPushButton* clearButton_;
   QPushButton* advancedButton_;
   QCheckBox* documentBeginCheckBox_;
   QCheckBox* matchCaseCheckBox_;
   QCheckBox* wholeWordsMatchCheckBox_;
   QCheckBox* useRegExpCheckBox_;
   QGroupBox* advancedGroupBox_;
   QSpacerItem* spacerItem_;
};

//////////////////////////////////////////////////////////////////////////////////////

class RemoveDialog final : public QDialog {
   Q_OBJECT

public:
   explicit RemoveDialog(const QString&, QWidget* parent = nullptr);
   RemoveDialog(const RemoveDialog&) = delete;
   RemoveDialog& operator=(const RemoveDialog&) = delete;
   RemoveDialog(RemoveDialog&&) = delete;
   ~RemoveDialog();

signals:
   void okButtonClickedSignal(const QString& filePath, bool);

private slots:
   void okButtonClickedSlot();

private:
   QString filePath_;
   QLabel* messageLabel_;
   QCheckBox* shiftDeleteCB_;
   QDialogButtonBox* buttonBox_;
};

#endif  // _IEDITOR_FINDDIALOG_H_
