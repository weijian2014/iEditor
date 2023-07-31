/****************************************************************************
**
**      Datetime:   2013-4-15
**      Author  :   韦   健
**      Titles  :   用于关闭iEditor时的提示已经被修改过的文件进行保存，该部件中
**                  可以选择性的保存想要保存的文件，可以不进行保存而直接关闭软件
**
**      Guangxi Normal University 2013
**
****************************************************************************/
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

#include "SaveDialog.h"

SaveDialog::SaveDialog(QWidget *parent) : QDialog(parent)
{
    topMassageLable = new QLabel(this);
    midMassageLable = new QLabel(tr("选择您想要保存的文档"), this);
    bottomMassegeLable = new QLabel(tr("如果您不保存，您所做的全部更改将永久丢失！"), this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard | QDialogButtonBox::Cancel | QDialogButtonBox::SaveAll);

    saveListWidget = new QListWidget(this);
    saveListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topMassageLable);
    mainLayout->addWidget(midMassageLable);
    mainLayout->addWidget(saveListWidget);
    mainLayout->addWidget(bottomMassegeLable);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    
    raise();
    activateWindow();
    setModal(true);
    setObjectName(tr("saveDialog"));
    
    connect(saveListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ItemSelectionChang()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(Click(QAbstractButton*)));
}

SaveDialog::~SaveDialog()
{
    
}

void SaveDialog::ItemSelectionChang()
{
    buttonBox->button(QDialogButtonBox::Save)->setEnabled(0 != saveListWidget->selectedItems().count());
}

void SaveDialog::Click(QAbstractButton *button)
{
    if (buttonBox->button(QDialogButtonBox::Save) == button)
    {
        emit SvaeSignal(saveListWidget->selectedItems());
    } else if (buttonBox->button(QDialogButtonBox::SaveAll) == button)
    {
        saveListWidget->selectAll();
        emit SvaeSignal(saveListWidget->selectedItems());
    } else if (buttonBox->button(QDialogButtonBox::Discard) == button)
    {
        emit NoSaveSignal("Close");
        
    } else if (buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        emit NoSaveSignal("Cancel");
    }
    hide();
}

void SaveDialog::SetFileNameList(QStringList &fileNameList)
{
    topMassageLable->setText(tr("<strong>有 %1 个文档已被更改，是否在关闭前保存更改？</strong>").arg(fileNameList.length()));
    saveListWidget->addItems(fileNameList);

    setFixedSize(sizeHint());
}
