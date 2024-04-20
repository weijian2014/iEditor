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
#include "SaveDialog.h"

#include <QDialog>
#include <QLabel>
#include <QStringList>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QAbstractButton>
#include <QListWidgetItem>

SaveDialog::SaveDialog(QWidget* parent)
    : QDialog(parent),
      topMassageLable_(nullptr),
      midMassageLable_(nullptr),
      bottomMassegeLable_(nullptr),
      buttonBox_(nullptr),
      saveListWidget_(nullptr),
      mainLayout_(nullptr) {
   topMassageLable_ = new QLabel(this);
   midMassageLable_ = new QLabel(tr("选择您想要保存的文档"), this);
   bottomMassegeLable_ =
       new QLabel(tr("如果您不保存，您所做的全部更改将永久丢失！"), this);
   buttonBox_ = new QDialogButtonBox(
       QDialogButtonBox::Save | QDialogButtonBox::Discard |
           QDialogButtonBox::Cancel | QDialogButtonBox::SaveAll,
       this);

   saveListWidget_ = new QListWidget(this);
   saveListWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
   buttonBox_->button(QDialogButtonBox::Save)->setEnabled(false);

   mainLayout_ = new QVBoxLayout(this);
   mainLayout_->addWidget(topMassageLable_);
   mainLayout_->addWidget(midMassageLable_);
   mainLayout_->addWidget(saveListWidget_);
   mainLayout_->addWidget(bottomMassegeLable_);
   mainLayout_->addWidget(buttonBox_);
   setLayout(mainLayout_);

   raise();
   activateWindow();
   setModal(true);
   setObjectName("saveDialog");

   connect(saveListWidget_, SIGNAL(itemSelectionChanged()), this,
           SLOT(itemSelectionChangedSlot()));
   connect(buttonBox_, SIGNAL(clicked(QAbstractButton*)), this,
           SLOT(clickSlot(QAbstractButton*)));
}

SaveDialog::~SaveDialog() {}

void SaveDialog::itemSelectionChangedSlot() {
   buttonBox_->button(QDialogButtonBox::Save)
       ->setEnabled(0 != saveListWidget_->selectedItems().count());
}

void SaveDialog::clickSlot(QAbstractButton* button) {
   if (buttonBox_->button(QDialogButtonBox::Save) == button) {
      emit svaeSignal(saveListWidget_->selectedItems());
   } else if (buttonBox_->button(QDialogButtonBox::SaveAll) == button) {
      saveListWidget_->selectAll();
      emit svaeSignal(saveListWidget_->selectedItems());
   } else if (buttonBox_->button(QDialogButtonBox::Discard) == button) {
      emit noSaveSignal("Close");

   } else if (buttonBox_->button(QDialogButtonBox::Cancel) == button) {
      emit noSaveSignal("Cancel");
   }
   hide();
}

void SaveDialog::setFileNameList(QStringList& fileNameList) {
   topMassageLable_->setText(
       tr("<strong>有 %1 个文档已被更改，是否在关闭前保存更改？</strong>")
           .arg(fileNameList.length()));
   saveListWidget_->addItems(fileNameList);

   setFixedSize(sizeHint());
}
