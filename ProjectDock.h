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
#ifndef PROJECTDOCK_H
#define PROJECTDOCK_H

#include <QDockWidget>

class QTabWidget;
class QListWidget;
class QListWidgetItem;

class ProjectDock : public QDockWidget
{
    Q_OBJECT

public:
    ProjectDock(QWidget *parent = nullptr, Qt::WindowFlags flag = Qt::WindowType::Widget);
    ~ProjectDock() {}

public:

    //List
    void AddItemToOpenFileManagementListWidget(const QString &filePath);
    void RemoveItemByOpenFileManagementListWidget(int row);
    void SetCurrentRowByOpenFileManagementListWidget(int row);
    void ChangeRowTextByOpenFileManagementListWidget(int row, const QString text);

    //Tab
    void AddTabToProjectDock(QWidget *widget, const QString &tabName);
    void SetTabCurrentIndex(int);

signals:
    void ListWidgetRowChangedSignal(int index);

private slots:
    void ListWidgetRowChanged(int);

private:
    QTabWidget *projectDockTabWidget;
    QListWidget *openFileManagementListWidget;
};

#endif // PROJECTDOCK_H
