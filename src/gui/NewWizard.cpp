/****************************************************************************
**
**      Datetime:   2013年04月21日 星期日 14时37分53秒
**      Author  :   韦   健
**      Titles  :   新建文件或项目的导航类，派生于QWizard
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "NewWizard.h"
#include "Config.h"

#include <QWizardPage>
#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QApplication>
#include <QSettings>
#include <QFile>

NewWizard::NewWizard(QWidget* parent, Qt::WindowFlags flag, int type,
                     const QString& fileDir)
    : QWizard(parent, flag),
      config_(common::Singleton<Config>::instancePointer()),
      firstWizardPage_(nullptr),
      secondWizardPage_(nullptr),
      thirdWizardPage_(nullptr),
      leftListWidget_(nullptr),
      rightListWidget_(nullptr),
      fileMessage_(nullptr),
      fullPathMessage_(nullptr),
      filePathLabel_(nullptr),
      fileNameLineEdit_(nullptr),
      filePathLineEdit_(nullptr),
      browsePushButton_(nullptr),
      fileTypes_(type),
      execName_("") {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());

   if (0 == fileTypes_)  // other
   {
      execFileType_ << tr("txt文件") << tr("h头文件") << tr("c源文件")
                    << tr("cpp源文件");
   } else if (1 == fileTypes_)  // C
   {
      execFileType_ << tr("c源文件") << tr("h头文件");
   } else  // CPP
   {
      execFileType_ << tr("cpp源文件") << tr("h头文件");
   }

   execProjectType_ << tr("ieditor项目");

   if (fileDir.isEmpty()) {
      leftList_ << tr("文件") << tr("项目");
   } else {
      leftList_ << tr("文件");
   }

   leftListWidget_ = new QListWidget(this);
   rightListWidget_ = new QListWidget(this);
   leftListWidget_->addItems(leftList_);
   leftListWidget_->setCurrentRow(0);
   rightListWidget_->addItems(execFileType_);
   rightListWidget_->setCurrentRow(0);
   QLabel* leftLabel = new QLabel(tr("1、选择新建类型:"), this);
   QLabel* rightLabel = new QLabel(tr("2、选择文件类型:"), this);

   connect(leftListWidget_, SIGNAL(currentRowChanged(int)), this,
           SLOT(leftListItemChangedSlot(int)));
   connect(this, SIGNAL(currentIdChanged(int)), this,
           SLOT(currentPageChangedSlot(int)));

   QVBoxLayout* leftVBLayout = new QVBoxLayout;
   leftVBLayout->addWidget(leftLabel);
   leftVBLayout->addWidget(leftListWidget_);

   QVBoxLayout* rightVBLayout = new QVBoxLayout;
   rightVBLayout->addWidget(rightLabel);
   rightVBLayout->addWidget(rightListWidget_);

   QHBoxLayout* firstLayout = new QHBoxLayout;
   firstLayout->addLayout(leftVBLayout);
   firstLayout->addStretch();
   firstLayout->addLayout(rightVBLayout);

   firstWizardPage_ = new QWizardPage(this);
   firstWizardPage_->setTitle(tr("新建选择"));
   firstWizardPage_->setLayout(firstLayout);
   ////////////////////////////////////////////////////////////////////1

   QLabel* fileNameL = new QLabel(tr("名称:"), this);
   QLabel* filePath = new QLabel(tr("目录:"), this);
   fileMessage_ = new QLabel(this);
   QLabel* fullPath = new QLabel(tr("<h4>完整路径：</h4>"), this);
   fullPathMessage_ = new QLabel(this);
   fullPathMessage_->setEnabled(false);
   fileNameLineEdit_ = new QLineEdit(this);
   filePathLineEdit_ = new QLineEdit(this);
   filePathLineEdit_->setReadOnly(true);
   browsePushButton_ = new QPushButton(tr("浏览"), this);

   if (!fileDir.isEmpty()) {
      filePathLineEdit_->setText(fileDir);
      browsePushButton_->setEnabled(false);
   } else {
      filePathLineEdit_->setText(config_->recentDir_);
   }

   connect(browsePushButton_, SIGNAL(clicked()), this,
           SLOT(browsePushButtonClickedSlot()));
   connect(fileNameLineEdit_, SIGNAL(textChanged(QString)), this,
           SLOT(fileNameLineEditTextChangedSlot(QString)));

   QHBoxLayout* nameHBLayout = new QHBoxLayout;
   nameHBLayout->addWidget(fileNameL);
   nameHBLayout->addWidget(fileNameLineEdit_);

   QHBoxLayout* pathHBLayout = new QHBoxLayout;
   pathHBLayout->addWidget(filePath);
   pathHBLayout->addWidget(filePathLineEdit_);
   pathHBLayout->addWidget(browsePushButton_);

   QVBoxLayout* secondVBLayout = new QVBoxLayout;
   secondVBLayout->addWidget(fileMessage_);
   secondVBLayout->addLayout(nameHBLayout);
   secondVBLayout->addLayout(pathHBLayout);
   secondVBLayout->addWidget(fullPath);
   secondVBLayout->addWidget(fullPathMessage_);

   secondWizardPage_ = new QWizardPage(this);
   secondWizardPage_->setTitle(tr("名称和目录"));
   secondWizardPage_->setLayout(secondVBLayout);
   ////////////////////////////////////////////////////////////////////2

   QLabel* fileNameLabel =
       new QLabel(tr("<h4>ieditor将要创建以下文件:</h4>"), this);
   filePathLabel_ = new QLabel(this);

   QVBoxLayout* summaryVBLayout = new QVBoxLayout;
   summaryVBLayout->addWidget(fileNameLabel);
   summaryVBLayout->addWidget(filePathLabel_);

   QHBoxLayout* thirdHBLayout = new QHBoxLayout;
   thirdHBLayout->addLayout(summaryVBLayout);

   thirdWizardPage_ = new QWizardPage(this);
   thirdWizardPage_->setTitle(tr("新建汇总"));
   thirdWizardPage_->setLayout(thirdHBLayout);
   ////////////////////////////////////////////////////////////////////

   connect(button(QWizard::FinishButton), SIGNAL(clicked()), this,
           SLOT(finishButtonClickedSlot()));
   addPage(firstWizardPage_);
   addPage(secondWizardPage_);
   addPage(thirdWizardPage_);

   setWindowTitle(tr("新建"));
   raise();
   activateWindow();
   setModal(true);
   setFixedSize(sizeHint());
   setObjectName("newWizard");
}

NewWizard::~NewWizard() {}

void NewWizard::leftListItemChangedSlot(int row) {
   if (0 == row) {
      rightListWidget_->clear();
      rightListWidget_->addItems(execFileType_);
   } else if (1 == row) {
      rightListWidget_->clear();
      rightListWidget_->addItems(execProjectType_);
   }

   rightListWidget_->setCurrentRow(0);
}

void NewWizard::currentPageChangedSlot(int page) {
   if (1 == page) {
      fileNameLineEdit_->setFocus();
      QString fileClass = rightListWidget_->currentItem()->text();

      if (tr("c源文件") == fileClass) {
         execName_ = ".c";
      } else if (tr("cpp源文件") == fileClass) {
         execName_ = ".cpp";
      } else if (tr("h头文件") == fileClass) {
         execName_ = ".h";
      } else if (tr("ieditor项目") == fileClass) {
         execName_ = ".ieditor";
      } else if (tr("txt文件") == fileClass) {
         execName_ = ".txt";
      }

      fileMessage_->setText(tr("<h4>新建：%1</h4>").arg(fileClass));

      if (!fileNameLineEdit_->text().split(".").first().isEmpty()) {
         fullPathMessage_->setText("");
         setFullPathMessageSlot();
      } else {
         button(QWizard::NextButton)->setEnabled(false);
         fullPathMessage_->setText(
             tr("<font color=red>文件名不能为空!</font>"));
      }
   } else if (2 == page) {
      filePathLabel_->setText(fullPathMessage_->text());
   }
}

void NewWizard::setFullPathMessageSlot() {
   QString fileFPath;
   QString fileNa = fileNameLineEdit_->text().split(".").first();
   if (".ieditor" == execName_) {
      if ('/' ==
          filePathLineEdit_->text().at(filePathLineEdit_->text().size() - 1))
         fileFPath = QString("%1%2/%3%4")
                         .arg(filePathLineEdit_->text())
                         .arg(fileNa)
                         .arg(fileNa)
                         .arg(execName_);
      else
         fileFPath = QString("%1/%2/%3%4")
                         .arg(filePathLineEdit_->text())
                         .arg(fileNa)
                         .arg(fileNa)
                         .arg(execName_);
   } else {
      if ('/' ==
          filePathLineEdit_->text().at(filePathLineEdit_->text().size() - 1))
         fileFPath = QString("%1%2%3")
                         .arg(filePathLineEdit_->text())
                         .arg(fileNa)
                         .arg(execName_);
      else
         fileFPath = QString("%1/%2%3")
                         .arg(filePathLineEdit_->text())
                         .arg(fileNa)
                         .arg(execName_);
   }

   QFile file(fileFPath);
   if (file.exists()) {
      button(QWizard::NextButton)->setEnabled(false);
      fullPathMessage_->setText(
          tr("<font color=red>不能创建,文件已经存在!</font>"));
   } else {
      button(QWizard::NextButton)->setEnabled(true);
      fullPathMessage_->setText(fileFPath);
   }
}

void NewWizard::browsePushButtonClickedSlot() {
   QString dir = QFileDialog::getExistingDirectory(
       this, tr("选择目录"), config_->recentDir_, QFileDialog::ShowDirsOnly);

   if (dir.isEmpty()) return;

   filePathLineEdit_->setText(dir);
   config_->recentDir_ = dir;

   QString fileNameLEText = fileNameLineEdit_->text().split(".").first();
   if (fileNameLEText.isEmpty()) {
      button(QWizard::NextButton)->setEnabled(false);
      fullPathMessage_->setText(tr("<font color=red>文件名不能为空!</font>"));
   } else {
      fullPathMessage_->setText("");
      setFullPathMessageSlot();
   }

   fileNameLineEdit_->setFocus();
}

void NewWizard::fileNameLineEditTextChangedSlot(const QString& text) {
   if (text.split('.').first().isEmpty()) {
      button(QWizard::NextButton)->setEnabled(false);
      fullPathMessage_->setText(tr("<font color=red>文件名不能为空!</font>"));
   } else {
      fullPathMessage_->setText("");
      setFullPathMessageSlot();
   }
}

void NewWizard::finishButtonClickedSlot() {
   emit finishButtonClickedSignal(filePathLabel_->text(), fileTypes_);
}
