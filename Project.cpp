/****************************************************************************
**
**      Datetime:   2013年04月22日 星期一 21时16分08秒 
**      Author  :   韦   健
**      Titles  :   项目类，作为TreeWidget的成员，一个项目就是一个类目类的对象
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QTreeWidgetItem>
#include <QMessageBox>

#include "Project.h"

Project::Project(const QString &pName, const QString &pDir, bool pType) : isCProject(pType), projectDir(pDir), projectName(pName)
{
    projectName << pName;
    headFolderName << QObject::tr("头文件");
    sourceFolderName << QObject::tr("源文件");
    
    projcetFolderItem = new QTreeWidgetItem(projectName);
    projcetFolderItem->setIcon(0, QIcon(":/images/projectFolder.png"));
    
    headFolderItem = new QTreeWidgetItem(projcetFolderItem, headFolderName);
    headFolderItem->setIcon(0, QIcon(":/images/headFolder.png"));

    sourceFolderItem = new QTreeWidgetItem(projcetFolderItem, sourceFolderName);
    sourceFolderItem->setIcon(0, QIcon(":/images/sourceFolder.png"));
    
    projcetFolderItem->addChild(headFolderItem);
    projcetFolderItem->addChild(sourceFolderItem);
}

Project::~Project()
{
    int hCount = headFolderItem->childCount();
    int sCount = sourceFolderItem->childCount();
    
    
    for (int i = 0; i < hCount; ++i)
    {
        delete headFolderItem->child(i);
    }
    
    for (int i = 0; i < sCount; ++i)
    {
        delete sourceFolderItem->child(i);
    }
    
    delete headFolderItem;
    delete sourceFolderItem;    //topLevelItem deleted! so this line is done! don't delete topLevelItem
}
