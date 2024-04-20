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

#include "Panel.h"
#include "Config.h"
#include "Define.h"
#include "Terminal.h"

Panel::Panel(QWidget* parent, Qt::WindowFlags flag)
    : QDockWidget(parent, flag),
      isLinkEnable_(false),
      config_(common::Singleton<Config>::instancePointer()),
      terminal_(nullptr) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
   setWindowTitle(tr("面板"));
   setObjectName(ObjectNameC_Panel);
   setFeatures(QDockWidget::DockWidgetMovable |
               QDockWidget::DockWidgetClosable);
   setAllowedAreas(Qt::BottomDockWidgetArea);
   setMinimumHeight(50);

   terminal_ = new Terminal(this);
   terminal_->setReadOnly(true);
   terminal_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   setWidget(terminal_);
}

Panel::~Panel() {}

bool Panel::execCommand(const QString& cmd) {
   return terminal_->execCommand(cmd);
}

void Panel::showMessage(const QString& msg) { terminal_->output(msg); }

void Panel::clear() { terminal_->clear(); }

void Panel::settingsChanged() { terminal_->settingsChangedSlot(); }

int Panel::getLineNumber(const QString& lineText) {
   return (-1 == lineText.indexOf("before"))
              ? lineText.split(":")[1].toInt()
              : lineText.split(":")[1].toInt() - 1;
}
