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
#ifndef _IEDITOR_PANEL_H_
#define _IEDITOR_PANEL_H_

#include <QtGui>
#include <QDockWidget>

class Config;
class Terminal;

class Panel final : public QDockWidget {
   Q_OBJECT

public:
   explicit Panel(QWidget* parent = nullptr,
                  Qt::WindowFlags flag = Qt::WindowType::Widget);
   Panel(const Panel&) = delete;
   Panel& operator=(const Panel&) = delete;
   Panel(Panel&&) = delete;
   ~Panel();

   bool execCommand(const QString& cmd);
   void showMessage(const QString& msg);
   void clear();
   bool isLinkEnable() const { return isLinkEnable_; }
   void settingsChanged();

private:
   int getLineNumber(const QString& lineText);

private:
   bool isLinkEnable_;
   Config* config_;
   Terminal* terminal_;
};

#endif  // _IEDITOR_PANEL_H_
