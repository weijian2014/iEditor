/****************************************************************************
**
**      Datetime:   2013-4-15
**      Author  :   韦   健
**      Titles  :   用于关闭ieditor时的提示已经被修改过的文件进行保存，该部件中
**                  可以选择性的保存想要保存的文件，可以不进行保存而直接关闭软件
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_SAVEDIALOG_H_
#define _IEDITOR_SAVEDIALOG_H_

#include <QDialog>
#include <QStringList>
#include <QString>

class QLabel;
class QDialogButtonBox;
class QListWidget;
class QAbstractButton;
class QListWidgetItem;
class QVBoxLayout;

class SaveDialog final : public QDialog {
   Q_OBJECT
public:
   explicit SaveDialog(QWidget* parent = nullptr);
   SaveDialog(const SaveDialog&) = delete;
   SaveDialog& operator=(const SaveDialog&) = delete;
   SaveDialog(SaveDialog&&) = delete;
   virtual ~SaveDialog();

   void setFileNameList(QStringList& fileNameList);

signals:
   void svaeSignal(QList<QListWidgetItem*>);
   void noSaveSignal(const QString&);

private slots:
   void itemSelectionChangedSlot();
   void clickSlot(QAbstractButton*);

private:
   QLabel* topMassageLable_;
   QLabel* midMassageLable_;
   QLabel* bottomMassegeLable_;
   QDialogButtonBox* buttonBox_;
   QListWidget* saveListWidget_;
   QVBoxLayout* mainLayout_;
};

#endif  // _IEDITOR_SAVEDIALOG_H_
