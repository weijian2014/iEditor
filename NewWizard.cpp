/****************************************************************************
**
**      Datetime:   2013年04月21日 星期日 14时37分53秒 
**      Author  :   韦   健
**      Titles  :   新建文件或项目的导航类，派生于QWizard
**
**      Guangxi Normal University 2013
**
****************************************************************************/
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

#include "NewWizard.h"
#include "ConfigStruct.h"

NewWizard::NewWizard(int types, const QString fileDir, QWidget *parent, Qt::WindowFlags flag) : QWizard(parent, flag), fileTypes(types)
{
    ////////////////////////////////////////////////////////////////////0
    if (0 == fileTypes) //other
    {
        execFileType << tr("txt文件文件") << tr("h头文件") << tr("c源文件") << tr("cpp源文件");
    } else if (1 == fileTypes)   //C
    {
        execFileType << tr("c源文件") << tr("h头文件");
    } else              //CPP
    {
        execFileType << tr("cpp源文件") << tr("h头文件");
    }
    
    execProjectType << tr("iEditor项目");
        
    if (fileDir.isEmpty())
    {
        leftList << tr("文件") << tr("项目");
    } else
    {
        leftList << tr("文件");
    }
    
    leftListWidget = new QListWidget(this);
    rightListWidget = new QListWidget(this);
    leftListWidget->addItems(leftList);
    leftListWidget->setCurrentRow(0);
    rightListWidget->addItems(execFileType);
    rightListWidget->setCurrentRow(0);
    QLabel *leftLabel = new QLabel(tr("1、选择新建类型:"));
    QLabel *rightLabel = new QLabel(tr("2、选择文件类型:"));
    
    connect(leftListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(LeftListItemChanged(int)));
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(CurrentPageChanged(int)));
    
    QVBoxLayout *leftVBLayout = new QVBoxLayout;
    leftVBLayout->addWidget(leftLabel);
    leftVBLayout->addWidget(leftListWidget);
    
    QVBoxLayout *rightVBLayout = new QVBoxLayout;
    rightVBLayout->addWidget(rightLabel);
    rightVBLayout->addWidget(rightListWidget);
    
    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addLayout(leftVBLayout);
    firstLayout->addStretch();
    firstLayout->addLayout(rightVBLayout);
    
    firstWizardPage = new QWizardPage(this);
    firstWizardPage->setTitle(tr("新建选择"));
    firstWizardPage->setLayout(firstLayout);
    ////////////////////////////////////////////////////////////////////1
    
    QLabel *fileNameL = new QLabel(tr("名称:"));
    QLabel *filePath = new QLabel(tr("目录:"));
    fileMessage = new QLabel;
    QLabel *fullPath = new QLabel(tr("<h4>完整路径：</h4>"));
    fullPathMessage = new QLabel;
    fullPathMessage->setEnabled(false);
    fileNameLineEdit = new QLineEdit;
    filePathLineEdit = new QLineEdit;
    filePathLineEdit->setReadOnly(true);
    browsePushButton = new QPushButton(tr("浏览"));
    
    if (!fileDir.isEmpty())
    {    
        filePathLineEdit->setText(fileDir);
        browsePushButton->setEnabled(false);
    } else
    {
        filePathLineEdit->setText(ConfigStruct::GetConfig()->recentDir);
    }
    
    connect(browsePushButton, SIGNAL(clicked()), this, SLOT(BrowsePushButtonClicked()));
    connect(fileNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(FileNameLineEditTextChanged(QString)));
    
    QHBoxLayout *nameHBLayout = new QHBoxLayout;
    nameHBLayout->addWidget(fileNameL);
    nameHBLayout->addWidget(fileNameLineEdit);
    
    QHBoxLayout *pathHBLayout = new QHBoxLayout;
    pathHBLayout->addWidget(filePath);
    pathHBLayout->addWidget(filePathLineEdit);
    pathHBLayout->addWidget(browsePushButton);
    
    QVBoxLayout *secondVBLayout = new QVBoxLayout;
    secondVBLayout->addWidget(fileMessage);
    secondVBLayout->addLayout(nameHBLayout);
    secondVBLayout->addLayout(pathHBLayout);
    secondVBLayout->addWidget(fullPath);
    secondVBLayout->addWidget(fullPathMessage);
    
    secondWizardPage = new QWizardPage(this);
    secondWizardPage->setTitle(tr("名称和目录"));
    secondWizardPage->setLayout(secondVBLayout);
    ////////////////////////////////////////////////////////////////////2
    
    QLabel *fileNameLabel = new QLabel(tr("<h4>iEditor将要创建以下文件:</h4>"));
    filePathLabel = new QLabel;
    
    QVBoxLayout *summaryVBLayout = new QVBoxLayout;
    summaryVBLayout->addWidget(fileNameLabel);
    summaryVBLayout->addWidget(filePathLabel);
    
    QHBoxLayout *thirdHBLayout = new QHBoxLayout;
    thirdHBLayout->addLayout(summaryVBLayout);
    
    thirdWizardPage = new QWizardPage(this);
    thirdWizardPage->setTitle(tr("新建汇总"));
    thirdWizardPage->setLayout(thirdHBLayout);
    ////////////////////////////////////////////////////////////////////
    
    connect(button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(FinishButtonClicked()));
    addPage(firstWizardPage);
    addPage(secondWizardPage);
    addPage(thirdWizardPage);
    
    setWindowTitle(tr("新建-iEditor"));
    raise();
    activateWindow();
    setModal(true);
    setFixedSize(sizeHint());
    setObjectName(tr("newWizard"));
}

NewWizard::~NewWizard()
{
    
}

void NewWizard::LeftListItemChanged(int row)
{
    if (0 == row)
    {
        rightListWidget->clear();
        rightListWidget->addItems(execFileType);
    } else if (1 == row)
    {
        rightListWidget->clear();
        rightListWidget->addItems(execProjectType);
    }
    
    rightListWidget->setCurrentRow(0);
}

void NewWizard::CurrentPageChanged(int page)
{
    if (1 == page)
    {
        fileNameLineEdit->setFocus();
        QString fileClass = rightListWidget->currentItem()->text();

        if (tr("c源文件") == fileClass)
        {
            execName = ".c";
        } else if (tr("cpp源文件") == fileClass)
        {
            execName = ".cpp";
        } else if (tr("h头文件") == fileClass)
        {
            execName = ".h";
        } else if (tr("iEditor项目") == fileClass)
        {
            execName = ".ieditor";
        } else if (tr("txt文件文件") == fileClass)
        {
            execName = ".txt";
        }

        fileMessage->setText(tr("<h4>新建：%1</h4>").arg(fileClass));
        
        if (!fileNameLineEdit->text().split(".").first().isEmpty())
        {
            SetFullPathMessage();
        } else
        {
            button(QWizard::NextButton)->setEnabled(false);
            fullPathMessage->setText(tr("<font color=red>文件名不能为空!</font>"));
        }
    } else if (2 == page)
    {
        filePathLabel->setText(fullPathMessage->text());
    }
}

void NewWizard::SetFullPathMessage()
{
    QString fileFPath;
    QString fileNa = fileNameLineEdit->text().split(".").first();
    if (".ieditor" == execName)
    {
        if ("/" == filePathLineEdit->text().at(filePathLineEdit->text().count()-1))
            fileFPath = tr("%1%2/%3%4").arg(filePathLineEdit->text()).arg(fileNa).arg(fileNa).arg(execName);
        else
            fileFPath = tr("%1/%2/%3%4").arg(filePathLineEdit->text()).arg(fileNa).arg(fileNa).arg(execName);
    } else
    {
        if ("/" == filePathLineEdit->text().at(filePathLineEdit->text().count()-1))
            fileFPath = tr("%1%2%3").arg(filePathLineEdit->text()).arg(fileNa).arg(execName);
        else
            fileFPath = tr("%1/%2%3").arg(filePathLineEdit->text()).arg(fileNa).arg(execName);
    }
    
    QFile file(fileFPath);
    if (file.exists())
    {
        button(QWizard::NextButton)->setEnabled(false);
        fullPathMessage->setText(tr("<font color=red>不能创建,文件已经存在!</font>"));
    } else
    {
        button(QWizard::NextButton)->setEnabled(true);
        fullPathMessage->setText(fileFPath);
    }
}

void NewWizard::BrowsePushButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择目录-iEditor"), ConfigStruct::GetConfig()->recentDir, QFileDialog::ShowDirsOnly);
    
    if (dir.isEmpty())
        return;
    
    filePathLineEdit->setText(dir);
    ConfigStruct::GetConfig()->recentDir = dir;
    
    QString fileNameLEText = fileNameLineEdit->text().split(".").first();
    if (fileNameLEText.isEmpty())
    {
        button(QWizard::NextButton)->setEnabled(false);
        fullPathMessage->setText(tr("<font color=red>文件名不能为空!</font>"));
    } else
    {
        SetFullPathMessage();
    }
    
    fileNameLineEdit->setFocus();
}

void NewWizard::FileNameLineEditTextChanged(const QString &fileN)
{
    QString fileNa = fileN.split(".").first();
    if (fileNa.isEmpty())
    {
        button(QWizard::NextButton)->setEnabled(false);
        fullPathMessage->setText(tr("<font color=red>文件名不能为空!</font>"));
    } else
    {
        SetFullPathMessage();
    }
}

void NewWizard::FinishButtonClicked()
{
    emit FinishButtonClickedSignal(filePathLabel->text(), fileTypes);
}
