/****************************************************************************
**
**      Datetime:   2013年04月18日 星期四 18时12分17秒
**      Author  :   韦   健
**      Titles  :   侧边栏，派生于QDockWidget,该类有一个QTabWidget成员，
**                  可以作为今后工程的扩展，在该部件中子部件
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_SIDEBAR_H_
#define _IEDITOR_SIDEBAR_H_

#include <QDockWidget>
#include <QTabWidget>

class SideBar final : public QDockWidget {
   Q_OBJECT

public:
   SideBar(QWidget* parent = nullptr,
           Qt::WindowFlags flag = Qt::WindowType::Widget);
   SideBar(const SideBar&) = delete;
   SideBar& operator=(const SideBar&) = delete;
   SideBar(SideBar&&) = delete;
   virtual ~SideBar();

public:
   void addTabWidget(QWidget* widget, const QString& tabName);
   void setTabCurrentIndex(int);
   void setTabPosition(QTabWidget::TabPosition position);

signals:
   void listWidgetRowChangedSignal(int index);

private slots:
   void listWidgetRowChangedSlot(int);

private:
   int tabIndex_;
   QTabWidget* tabWidget_;
};

#endif  // _IEDITOR_SIDEBAR_H_
