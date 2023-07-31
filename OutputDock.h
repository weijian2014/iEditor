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

#ifndef OUTPUTDOCK_H
#define OUTPUTDOCK_H

#include <QtGui>
#include <QDockWidget>

class QPlainTextEdit;

class OutputDock : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit OutputDock(QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~OutputDock() {}
    
    void ShowBuildMessage(const QString &filePath);
    void AppendHtmlToBuildOutputPlainTextEdit(const QString &htmlText);
    void ClearBuildOutput();
    
public:
    bool isLinkEnable;
    
private:
    int GetLineNumber(const QString &lineText);
    
private:
    QPlainTextEdit *buildOutputPlainTextEdit;
};

#endif
