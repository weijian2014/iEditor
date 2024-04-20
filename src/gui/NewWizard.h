/****************************************************************************
**
**      Datetime:   2013年04月21日 星期日 14时37分53秒
**      Author  :   韦   健
**      Titles  :   新建文件或项目的导航类，派生于QWizard
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_NEWWIZARD_H_
#define _IEDITOR_NEWWIZARD_H_

#include <QWizard>

class QWizardPage;
class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class Config;

class NewWizard final : public QWizard {
   Q_OBJECT
public:
   explicit NewWizard(QWidget* parent = nullptr,
                      Qt::WindowFlags flag = Qt::WindowType::Widget,
                      int type = 0, const QString& fileDir = "");
   NewWizard(const NewWizard&) = delete;
   NewWizard& operator=(const NewWizard&) = delete;
   NewWizard(NewWizard&&) = delete;
   virtual ~NewWizard();

signals:
   void finishButtonClickedSignal(const QString& createFileFullPath,
                                  int projectType);

private slots:
   void leftListItemChangedSlot(int row);
   void currentPageChangedSlot(int page);
   void browsePushButtonClickedSlot();
   void fileNameLineEditTextChangedSlot(const QString& text);
   void finishButtonClickedSlot();
   void setFullPathMessageSlot();

private:
   Config* config_;

   QWizardPage* firstWizardPage_;
   QWizardPage* secondWizardPage_;
   QWizardPage* thirdWizardPage_;

   QListWidget* leftListWidget_;
   QListWidget* rightListWidget_;

   QLabel* fileMessage_;
   QLabel* fullPathMessage_;
   QLabel* filePathLabel_;
   QLineEdit* fileNameLineEdit_;
   QLineEdit* filePathLineEdit_;
   QPushButton* browsePushButton_;

   int fileTypes_;
   QString execName_;

   QStringList execFileType_;
   QStringList execProjectType_;
   QStringList leftList_;
};

#endif  // _IEDITOR_NEWWIZARD_H_
