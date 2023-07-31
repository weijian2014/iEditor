/****************************************************************************
**
**      Datetime:   2013年04月20日 星期六 13时11分10秒
**      Author  :   韦   健
**      Titles  :   该类用于多标签使用的部件，对左键双击进行了重载，双击标签页可以
**                  关闭标签，双击该部件的空白区域可以新建一个标签
**
**      Guangxi Normal University 2013
**
****************************************************************************/

#include <QMouseEvent>
#include <QTabBar>

#include "TextEditTab.h"

TextEditTab::TextEditTab(QWidget *parent) : QTabWidget(parent)
{
    setTabShape(QTabWidget::Rounded);    //Rounded | Triangular
    setTabsClosable(true);
    tabBar()->installEventFilter(this);
    setObjectName(tr("textEditTab"));
}

void TextEditTab::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        emit LeftMouseDoubleClickOnTextEditTabSignal(true);
    }
    QTabWidget::mouseDoubleClickEvent(event);
}

bool TextEditTab::eventFilter(QObject *o, QEvent *e)
{
	if (o == tabBar() && e->type() == QEvent::MouseButtonDblClick)
    {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
		emit LeftMouseDoubleClickOnTabSignal(tabBar()->tabAt(mouseEvent->pos()));
	}
	return QTabWidget::eventFilter(o, e);
}
