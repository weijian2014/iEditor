/****************************************************************************
**
**      Datetime:   2013-03-22 12：21
**      Author  :   韦   健
**      Titles  :   这是查找对话框的类，派生于QDialog
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QtGui>

#include "FindDialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
    findFlags = 0;
    isFirst = true;
    findLabel = new QLabel(tr("查　找:"));
    replaceLabel = new QLabel(tr("替换为:"));
    
    findLineEdit = new QLineEdit;
    findLineEdit->setFocus();
    findLineEdit->setFixedWidth(200);
    replaceLineEdit = new QLineEdit;
    replaceLineEdit->setFixedWidth(200);
    
    findForwardButton = new QPushButton(tr("向前(&P)"));
    findForwardButton->setEnabled(false);
    findBackwardButton = new QPushButton(tr("向后(&N)"));
    findBackwardButton->setEnabled(false);
    closeButton = new QPushButton(tr("关闭(&C)"));
    closeButton->setDefault(true);
    replaceButton = new QPushButton(tr("替换(&R)"));
    replaceAllButton = new QPushButton(tr("全部(&T)"));
    clearButton = new QPushButton(tr("清空(&E)"));
    advancedButton = new QPushButton(tr("高级(&A)>>>"));
    advancedButton->setCheckable(true);
    
    documentStartCheckBox = new QCheckBox(tr("从文件开始处查找"));
    matchCaseCheckBox = new QCheckBox(tr("区分大小写"));
    allWordMatchCheckBox = new QCheckBox(tr("全词匹配"));
    useRegExpCheckBox = new QCheckBox(tr("正则表达式"));
    
    advancedGroupBox = new QGroupBox(tr("高级选项"));
    
    connect(findLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(EnableFindButton(QString)));
    connect(findForwardButton, SIGNAL(clicked()), this, SLOT(FindForwardClicked()));
    connect(findBackwardButton, SIGNAL(clicked()), this, SLOT(FindBackwardClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(ReplaceClicked()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(ReplaceAllClicked()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(ClearClicked()));
    connect(advancedButton, SIGNAL(toggled(bool)), advancedGroupBox, SLOT(setVisible(bool)));
    connect(advancedButton, SIGNAL(toggled(bool)), this, SLOT(AdvancedButtonClicked(bool)));
    
    QHBoxLayout *findHBLayout = new QHBoxLayout;
    findHBLayout->addWidget(findLabel);
    findHBLayout->addWidget(findLineEdit);
    findHBLayout->addWidget(findForwardButton);
    findHBLayout->addWidget(findBackwardButton);
    findHBLayout->addWidget(closeButton);
    
    QHBoxLayout *replaceHBLayout = new QHBoxLayout;
    replaceHBLayout->addWidget(replaceLabel);
    replaceHBLayout->addWidget(replaceLineEdit);
    replaceHBLayout->addWidget(replaceButton);
    replaceHBLayout->addWidget(replaceAllButton);
    replaceHBLayout->addWidget(clearButton);
    
    QHBoxLayout *advancedHBLayout = new QHBoxLayout;
    advancedHBLayout->addWidget(documentStartCheckBox);
    advancedHBLayout->addStretch();
    advancedHBLayout->addWidget(advancedButton);
    
    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addLayout(findHBLayout);
    topLayout->addLayout(replaceHBLayout);
    topLayout->addLayout(advancedHBLayout);
    
    QGridLayout *advancedGLayout = new QGridLayout;
    advancedGLayout->addWidget(matchCaseCheckBox, 0, 0);
    advancedGLayout->addWidget(allWordMatchCheckBox, 1, 0);
    advancedGLayout->addWidget(useRegExpCheckBox, 0, 1);
    advancedGroupBox->setLayout(advancedGLayout);
    advancedGroupBox->hide();
    
    QVBoxLayout *mainVBLayout = new QVBoxLayout;
    mainVBLayout->addLayout(topLayout);
    spacerItem = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainVBLayout->addSpacerItem(spacerItem);
    mainVBLayout->addWidget(advancedGroupBox);
    setLayout(mainVBLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    
    raise();
    activateWindow();
    setWindowTitle(tr("查找&替换-iEditor"));
    SetTabOrder();
    setObjectName(tr("findDialog"));
}

void FindDialog::SetTabOrder()
{
    setTabOrder(findLineEdit, replaceLineEdit);
    setTabOrder(replaceLineEdit, findForwardButton);
    setTabOrder(findForwardButton, findBackwardButton);
    setTabOrder(findBackwardButton, replaceButton);
    setTabOrder(replaceButton, replaceAllButton);
    setTabOrder(replaceAllButton, clearButton);
    setTabOrder(clearButton, documentStartCheckBox);
    setTabOrder(documentStartCheckBox, advancedButton);
    setTabOrder(advancedButton, matchCaseCheckBox);
    setTabOrder(matchCaseCheckBox, allWordMatchCheckBox);
    setTabOrder(allWordMatchCheckBox, useRegExpCheckBox);
    setTabOrder(useRegExpCheckBox, closeButton);
}

void FindDialog::EnableFindButton(const QString text)
{
    findForwardButton->setEnabled(!text.isEmpty());
    findBackwardButton->setEnabled(!text.isEmpty());
}

void FindDialog::FindForwardClicked()
{
    findFlags = 0;
    if (true == documentStartCheckBox->isChecked())
        findFlags += 1;
    if (true == matchCaseCheckBox->isChecked())
        findFlags += 2;
    if (true == allWordMatchCheckBox->isChecked())
        findFlags += 4;
    if (true == useRegExpCheckBox->isChecked())
        findFlags += 8;
    
    findFlags += 16;
    
    emit FindSignal();
}

void FindDialog::FindBackwardClicked()
{
    findFlags = 0;
    if (true == documentStartCheckBox->isChecked())
        findFlags += 1;
    if (true == matchCaseCheckBox->isChecked())
        findFlags += 2;
    if (true == allWordMatchCheckBox->isChecked())
        findFlags += 4;
    if (true == useRegExpCheckBox->isChecked())
        findFlags += 8;
    emit FindSignal();
}

void FindDialog::ReplaceClicked()
{
    emit ReplaceSignal();
}

void FindDialog::ReplaceAllClicked()
{
    emit ReplaceAllSignal();
}

void FindDialog::ClearClicked()
{
    findLineEdit->clear();
    replaceLineEdit->clear();
    findLineEdit->setFocus();
}

void FindDialog::AdvancedButtonClicked(bool isChange)
{
    if (isChange)
    {
        advancedButton->setText(tr("高级(&A)<<<"));
    } else
    {
        advancedButton->setText(tr("高级(&A)>>>"));
    }
}

RemoveDialog::RemoveDialog(const QString &filePath, QWidget *parent) : QDialog(parent), filePath(filePath)
{
    setWindowTitle(tr("删除确认-iEditor"));
    
    messageLabel = new QLabel(tr("<h4>从项目中删除:<h4><p>%1</p>").arg(filePath));
    shiftDeleteCB = new QCheckBox(tr("要彻底从磁盘中删除文件吗？"));
    buttonBox = new QDialogButtonBox( QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    
    QVBoxLayout *mainVBLayout = new QVBoxLayout;
    mainVBLayout->addWidget(messageLabel);
    mainVBLayout->addWidget(shiftDeleteCB);
    mainVBLayout->addWidget(buttonBox);
    
    setLayout(mainVBLayout);
    raise();
    activateWindow();
    setModal(true);
    
    setFixedSize(sizeHint());
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(close()));
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(OkButtonClicked()));
}

RemoveDialog::~RemoveDialog()
{
    
}

void RemoveDialog::OkButtonClicked()
{
    emit OkButtonClickedSignal(filePath, shiftDeleteCB->isChecked());
    close();
}
