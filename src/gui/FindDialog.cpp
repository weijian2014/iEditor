/****************************************************************************
**
**      Datetime:   2013-03-22 12：21
**      Author  :   韦   健
**      Titles  :   这是查找对话框的类，派生于QDialog
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "FindDialog.h"

#include <QtGui>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDialogButtonBox>

FindDialog::FindDialog(QWidget* parent)
    : QDialog(parent),
      findLineEdit_(nullptr),
      replaceLineEdit_(nullptr),
      findFlags_(0),
      findLabel_(nullptr),
      replaceLabel_(nullptr),
      findForwardButton_(nullptr),
      findBackwardButton_(nullptr),
      closeButton_(nullptr),
      replaceButton_(nullptr),
      replaceAllButton_(nullptr),
      clearButton_(nullptr),
      advancedButton_(nullptr),
      documentBeginCheckBox_(nullptr),
      matchCaseCheckBox_(nullptr),
      wholeWordsMatchCheckBox_(nullptr),
      useRegExpCheckBox_(nullptr),
      advancedGroupBox_(nullptr),
      spacerItem_(nullptr) {
   findLabel_ = new QLabel(tr("查找"), this);
   replaceLabel_ = new QLabel(tr("替换"), this);

   findLineEdit_ = new QLineEdit(this);
   findLineEdit_->setFocus();
   findLineEdit_->setFixedWidth(200);
   replaceLineEdit_ = new QLineEdit(this);
   replaceLineEdit_->setFixedWidth(200);

   findForwardButton_ = new QPushButton(tr("向前(&P)"), this);
   findForwardButton_->setEnabled(false);
   findBackwardButton_ = new QPushButton(tr("向后(&N)"), this);
   findBackwardButton_->setEnabled(false);
   closeButton_ = new QPushButton(tr("关闭(&C)"), this);
   closeButton_->setDefault(true);
   replaceButton_ = new QPushButton(tr("替换(&R)"), this);
   replaceAllButton_ = new QPushButton(tr("全部替换(&T)"), this);
   clearButton_ = new QPushButton(tr("清空(&E)"), this);
   advancedButton_ = new QPushButton(tr("高级(&A)>>>"), this);
   advancedButton_->setCheckable(true);

   documentBeginCheckBox_ = new QCheckBox(tr("从文件开始处查找"), this);
   matchCaseCheckBox_ = new QCheckBox(tr("区分大小写"), this);
   wholeWordsMatchCheckBox_ = new QCheckBox(tr("全词匹配"), this);
   useRegExpCheckBox_ = new QCheckBox(tr("正则表达式"), this);

   advancedGroupBox_ = new QGroupBox(tr("高级选项"), this);

   connect(findLineEdit_, SIGNAL(textChanged(const QString&)), this,
           SLOT(enableFindButtonSlot(QString)));
   connect(findForwardButton_, SIGNAL(clicked()), this,
           SLOT(findForwardClickedSlot()));
   connect(findBackwardButton_, SIGNAL(clicked()), this,
           SLOT(findBackwardClickedSlot()));
   connect(closeButton_, SIGNAL(clicked()), this, SLOT(hide()));
   connect(replaceButton_, SIGNAL(clicked()), this, SLOT(replaceClickedSlot()));
   connect(replaceAllButton_, SIGNAL(clicked()), this,
           SLOT(replaceAllClickedSlot()));
   connect(clearButton_, SIGNAL(clicked()), this, SLOT(clearClickedSlot()));
   connect(advancedButton_, SIGNAL(toggled(bool)), advancedGroupBox_,
           SLOT(setVisible(bool)));
   connect(advancedButton_, SIGNAL(toggled(bool)), this,
           SLOT(advancedButtonClickedSlot(bool)));

   QGridLayout* topGridLayout = new QGridLayout;
   topGridLayout->addWidget(findLabel_, 0, 0, Qt::AlignmentFlag::AlignRight);
   topGridLayout->addWidget(findLineEdit_, 0, 1);
   topGridLayout->addWidget(findForwardButton_, 0, 2);
   topGridLayout->addWidget(findBackwardButton_, 0, 3);
   topGridLayout->addWidget(closeButton_, 0, 4);
   topGridLayout->addWidget(replaceLabel_, 1, 0, Qt::AlignmentFlag::AlignRight);
   topGridLayout->addWidget(replaceLineEdit_, 1, 1);
   topGridLayout->addWidget(replaceButton_, 1, 2);
   topGridLayout->addWidget(replaceAllButton_, 1, 3);
   topGridLayout->addWidget(clearButton_, 1, 4);
   topGridLayout->addWidget(documentBeginCheckBox_, 2, 1);
   topGridLayout->addWidget(advancedButton_, 2, 4);

   QGridLayout* advancedGridLayout = new QGridLayout;
   advancedGridLayout->addWidget(matchCaseCheckBox_, 0, 0);
   advancedGridLayout->addWidget(wholeWordsMatchCheckBox_, 1, 0);
   advancedGridLayout->addWidget(useRegExpCheckBox_, 0, 1);
   advancedGroupBox_->setLayout(advancedGridLayout);
   advancedGroupBox_->hide();

   QVBoxLayout* mainVBLayout = new QVBoxLayout;
   mainVBLayout->addLayout(topGridLayout);
   // todo weijian
   spacerItem_ =
       new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
   mainVBLayout->addSpacerItem(spacerItem_);
   mainVBLayout->addWidget(advancedGroupBox_);
   setLayout(mainVBLayout);
   layout()->setSizeConstraint(QLayout::SetFixedSize);

   raise();
   activateWindow();
   setWindowTitle(tr("查找和替换"));
   setTabOrder();
   setObjectName("findDialog");
}

FindDialog::~FindDialog() {}

void FindDialog::setTabOrder() {
   QWidget::setTabOrder(findLineEdit_, replaceLineEdit_);
   QWidget::setTabOrder(replaceLineEdit_, findForwardButton_);
   QWidget::setTabOrder(findForwardButton_, findBackwardButton_);
   QWidget::setTabOrder(findBackwardButton_, replaceButton_);
   QWidget::setTabOrder(replaceButton_, replaceAllButton_);
   QWidget::setTabOrder(replaceAllButton_, clearButton_);
   QWidget::setTabOrder(clearButton_, documentBeginCheckBox_);
   QWidget::setTabOrder(documentBeginCheckBox_, advancedButton_);
   QWidget::setTabOrder(advancedButton_, matchCaseCheckBox_);
   QWidget::setTabOrder(matchCaseCheckBox_, wholeWordsMatchCheckBox_);
   QWidget::setTabOrder(wholeWordsMatchCheckBox_, useRegExpCheckBox_);
   QWidget::setTabOrder(useRegExpCheckBox_, closeButton_);
}

void FindDialog::enableFindButtonSlot(const QString text) {
   findForwardButton_->setEnabled(!text.isEmpty());
   findBackwardButton_->setEnabled(!text.isEmpty());
}

void FindDialog::findForwardClickedSlot() {
   findFlags_ = 0;
   findFlags_ += FindForward;
   if (documentBeginCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::DocumentBegin;
   }
   if (matchCaseCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::FindCaseSensitively;
   }
   if (wholeWordsMatchCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::FindWholeWords;
   }
   if (useRegExpCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::UsingRegularExpression;
   }

   emit findForwardSignal();
}

void FindDialog::findBackwardClickedSlot() {
   findFlags_ = 0;
   findFlags_ += MyFindFlag::FindBackward;
   if (documentBeginCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::DocumentBegin;
   }
   if (matchCaseCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::FindCaseSensitively;
   }
   if (wholeWordsMatchCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::FindWholeWords;
   }
   if (useRegExpCheckBox_->isChecked()) {
      findFlags_ += MyFindFlag::UsingRegularExpression;
   }

   emit findBackwardSignal();
}

void FindDialog::replaceClickedSlot() { emit replaceSignal(); }

void FindDialog::replaceAllClickedSlot() { emit replaceAllSignal(); }

void FindDialog::clearClickedSlot() {
   findLineEdit_->clear();
   replaceLineEdit_->clear();
   findLineEdit_->setFocus();
}

void FindDialog::advancedButtonClickedSlot(bool isChange) {
   if (isChange) {
      advancedButton_->setText(tr("高级(&A)<<<"));
   } else {
      advancedButton_->setText(tr("高级(&A)>>>"));
   }
}

RemoveDialog::RemoveDialog(const QString& filePath, QWidget* parent)
    : QDialog(parent), filePath_(filePath) {
   setWindowTitle(tr("删除确认"));

   messageLabel_ =
       new QLabel(tr("<h4>从项目中删除:<h4><p>%1</p>").arg(filePath), this);
   shiftDeleteCB_ = new QCheckBox(tr("要彻底删除文件吗？"), this);
   buttonBox_ = new QDialogButtonBox(
       QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

   QVBoxLayout* mainVBLayout = new QVBoxLayout;
   mainVBLayout->addWidget(messageLabel_);
   mainVBLayout->addWidget(shiftDeleteCB_);
   mainVBLayout->addWidget(buttonBox_);

   setLayout(mainVBLayout);
   raise();
   activateWindow();
   setModal(true);

   setFixedSize(sizeHint());
   connect(buttonBox_->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
           this, SLOT(close()));
   connect(buttonBox_->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this,
           SLOT(okButtonClickedSlot()));
}

RemoveDialog::~RemoveDialog() {}

void RemoveDialog::okButtonClickedSlot() {
   emit okButtonClickedSignal(filePath_, shiftDeleteCB_->isChecked());
   close();
}
