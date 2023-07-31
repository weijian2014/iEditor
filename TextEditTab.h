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
#ifndef TEXTEDITTAB_H
#define TEXTEDITTAB_H

#include <QTabWidget>

class TextEditTab : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit TextEditTab(QWidget *parent = 0);
    ~TextEditTab() { }
    
signals:
    void LeftMouseDoubleClickOnTextEditTabSignal(bool);
    void LeftMouseDoubleClickOnTabSignal(int index);
    
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    bool eventFilter(QObject *, QEvent *);
};

#endif // TEXTEDITTAB_H
