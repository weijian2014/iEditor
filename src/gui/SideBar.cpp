/****************************************************************************
**
**      Datetime:   2013年04月18日 星期四 18时12分17秒
**      Author  :   韦   健
**      Titles  :   项目管理类，派生于QDockWidget,该类有一个QTabWidget成员，
**                  可以作为今后工程的扩展，在该部件中子部件
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "SideBar.h"
#include "Define.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QFileInfo>

SideBar::SideBar(QWidget* parent, Qt::WindowFlags flag)
    : QDockWidget(parent, flag), tabIndex_(-1) {
   setWindowTitle(tr("侧边栏"));
   setObjectName(ObjectNameC_SideBar);
   setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
   setFeatures(QDockWidget::DockWidgetMovable |
               QDockWidget::DockWidgetClosable);
   setMinimumWidth(150);

   tabWidget_ = new QTabWidget(this);
   tabWidget_->setTabShape(QTabWidget::Triangular);  // tab形状-三角形
   tabWidget_->setTabsClosable(false);

   setWidget(tabWidget_);
}

SideBar::~SideBar() { tabWidget_->removeTab(tabIndex_); }

void SideBar::listWidgetRowChangedSlot(int index) {
   emit listWidgetRowChangedSignal(index);
}

void SideBar::addTabWidget(QWidget* widget, const QString& tabName) {
   tabWidget_->addTab(widget, tabName);
}

void SideBar::setTabCurrentIndex(int index) {
   tabWidget_->setCurrentIndex(index);
}

void SideBar::setTabPosition(QTabWidget::TabPosition position) {
   tabWidget_->setTabPosition(position);
}
