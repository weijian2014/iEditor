/****************************************************************************
**
**      Datetime:   2013年04月18日 星期四 13时41分04秒
**      Author  :   韦   健
**      Titles  :   用于编译、链接和运行时的标准流输出，派生于QDockWidget，
**                  有一个QPlainTextEdit成员，该类可以在后续的工程中进行
**                  扩展
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QPlainTextEdit>

#include "OutputDock.h"

OutputDock::OutputDock(QWidget *parent, Qt::WindowFlags flag) : QDockWidget(parent, flag)
{
    setWindowTitle(tr("信息管理窗体"));
    setObjectName(tr("outputDockWidget"));
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setMinimumHeight(50);
    
    buildOutputPlainTextEdit = new QPlainTextEdit;
    buildOutputPlainTextEdit->setReadOnly(true);
    buildOutputPlainTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    setWidget(buildOutputPlainTextEdit);
}

void OutputDock::ShowBuildMessage(const QString &filePath)
{
    QFile file(filePath);
    
	if(file.open(QFile::ReadOnly | QFile::Text)){
		QTextStream in(&file);
        
#ifndef QT_NO_CURSOR
		QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        isLinkEnable = true;
        QString messageText;
        while (!in.atEnd()) {
            messageText = in.readLine();
            
            if (!messageText.isEmpty()) {
                if (-1 != messageText.indexOf(tr("错误：")) || -1 != messageText.indexOf("error")) {
                    
                    isLinkEnable = false;
                    buildOutputPlainTextEdit->appendHtml(tr("<p><font color=#FF0000>第%1行\t\t%2</font></p>").arg(GetLineNumber(messageText)).arg(messageText));
                    
                } else if (-1 != messageText.indexOf(tr("警告："))){
                    
                    buildOutputPlainTextEdit->appendHtml(tr("<p><font color=#C16F6F>第%1行\t\t%2</font></p>").arg(GetLineNumber(messageText)).arg(messageText));
                    
                } else {
                    buildOutputPlainTextEdit->appendHtml(tr("<p><font color=#643C3C>%1</font></p>").arg(messageText));
                }
            }
        }
        
#ifndef QT_NO_CURSOR
		QApplication::restoreOverrideCursor();
#endif
	}
    file.close();
}

void OutputDock::AppendHtmlToBuildOutputPlainTextEdit(const QString &htmlText)
{
    buildOutputPlainTextEdit->appendHtml(htmlText);
}

void OutputDock::ClearBuildOutput()
{
    buildOutputPlainTextEdit->clear();
}

int OutputDock::GetLineNumber(const QString &lineText)
{
    return (-1 == lineText.indexOf("before")) ? lineText.split(":")[1].toInt() : lineText.split(":")[1].toInt() - 1;
}
