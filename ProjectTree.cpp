/****************************************************************************
**
**      Datetime:   2013年04月20日 星期六 19时32分54秒
**      Author  :   韦   健
**      Titles  :   用于项目的显示，每个项目是一个Project对象，该类为主窗体提供
**                  了一些操作的接口，与主窗体进行信息和槽的一些关联
**
**      Guangxi Normal University 2013
**
****************************************************************************/

#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QMessageBox>

#include "ProjectTree.h"
#include "Project.h"


ProjectTree::ProjectTree(QWidget *parent) : QTreeWidget(parent)
{
    currentProject = 0;
    
    header()->hide();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setObjectName(tr("projectTree"));
    
    CreteaTopContextMenu();
    CreteaFileContextMenu();
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentProjectChanged()));
}

ProjectTree::~ProjectTree()
{
    for (int i = 0; i < projectList.count(); ++i)
    {
        delete projectList[i];
        projectList[i] = 0;
    }

    if (0 != currentProject)
        delete currentProject;
}

void ProjectTree::NewProject(const QString &projectName, const QString &fileDir, bool rojectType)
{
    Project *project = new Project(projectName, fileDir, rojectType);
    addTopLevelItem(project->projcetFolderItem);
    currentProject = project;
    projectList.append(project);
    project = 0;
}

void ProjectTree::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        closePersistentEditor(currentItem(), 0);
        QString newFileName = currentItem()->text(0).split(".").first();
        
        if (IsCProject())
        {
            newFileName.append(".c");
        } else
        {
            newFileName.append(".cpp");
        }
        
        if (!ItemNameExist(newFileName))
        {
            QString originalfilePath = currentProject->projectDir + originalfileName;
            QString newFilePath = currentProject->projectDir + newFileName;
            
            if (QFile::rename(originalfilePath, newFilePath))
            {
                currentItem()->setText(0, newFileName);
                emit RenameSignal(originalfilePath, newFilePath);
            }
            
        } else {
            QMessageBox::warning(this, tr("命名提示-iEditor"), tr("文件名已经存在！"));
            currentItem()->setText(0, originalfileName);
        }
    }
    
    QTreeWidget::keyReleaseEvent(event);
}

bool ProjectTree::IsCProject()
{
    return currentProject->isCProject;
}

bool ProjectTree::ItemNameExist(const QString &fileName)
{
    int hCount = GetHeadFileCount();
    int sCount = GetSourceFileCount();
    for (int i = 0; i < hCount; ++i)
    {
        if (fileName == GetHeadFolderItem()->child(i)->text(0))
        {
            return true;
        }
    }
    
    for (int i = 0; i < sCount; ++i)
    {
        if (fileName == GetSourceFolderItem()->child(i)->text(0))
        {
            return true;
        }
    }
    
    return false;
}

bool ProjectTree::ProjectExist(const QString &projectFullPath)
{
    foreach (Project *p, projectList)
    {
        if ((p->projectDir + p->projectName.first() + ".ieditor") == projectFullPath)
            return true;
    }
    
    return false;
}

void ProjectTree::CurrentProjectChanged()
{
    QTreeWidgetItem *curItem = currentItem();
    int level = ItemLevel(curItem);
    
    if (0 == level) //0
    {
        for (int i = 0; i < projectList.count(); ++i)
        {
            if (curItem == projectList[i]->projcetFolderItem)
            {
                currentProject = projectList[i];
                break;
            }
        }
    } else if (1 == level)    //1
    {
        for (int i = 0; i < projectList.count(); ++i)
        {
            if (curItem->parent() == projectList[i]->projcetFolderItem)
            {
                currentProject = projectList[i];
                break;
            }
        }
    } else                                     //2
    {
        for (int i = 0; i < projectList.count(); ++i)
        {
            if (curItem->parent()->parent() == projectList[i]->projcetFolderItem)
            {
                currentProject = projectList[i];
                break;
            }
        }
    }
}

QString ProjectTree::GetCurrentPojectFullPath()
{
    return GetCurrentProjectDir() + GetCurrentProjectName() + ".ieditor";
}

QString ProjectTree::GetCurrentProjectName()
{
    return currentProject->projectName.first();
}

QString ProjectTree::GetCurrentProjectDir()
{
    return currentProject->projectDir;
}

void ProjectTree::CreteaTopContextMenu()
{
    addNewFileAction = new QAction(tr("添加新文件"), this);
    addExistingFileAction = new QAction(tr("添加现有文件"), this);
    closeProjectAction = new QAction(tr("关闭项目"), this);
    
    topContextMenu = new QMenu(this);
    topContextMenu->addAction(addNewFileAction);
    topContextMenu->addAction(addExistingFileAction);
    topContextMenu->addSeparator();
    topContextMenu->addAction(closeProjectAction);
    
    connect(topContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(TopContextMenuClicked(QAction*)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu()));
}

void ProjectTree::CreteaFileContextMenu()
{
    openFileAction = new QAction(tr("打开文件"), this);
    removeFileAction = new QAction(tr("删除文件"), this);
    renameFileAction = new QAction(tr("重命名"), this);
    foldAction = new QAction(tr("折叠"), this);
    
    fileContextMenu = new QMenu(this);
    fileContextMenu->addAction(openFileAction);
    fileContextMenu->addAction(removeFileAction);
    fileContextMenu->addSeparator();
    fileContextMenu->addAction(renameFileAction);
    fileContextMenu->addSeparator();
    fileContextMenu->addAction(foldAction);
    
    connect(fileContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(FileContextMenuClicked(QAction*)));
}

void ProjectTree::takeTopLevelItem(int index)
{
    Project * p = currentProject;
    
    projectList.removeAt(index);
    if (!projectList.isEmpty())
    {
        currentProject = projectList.first();
    }
    disconnect(this, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentProjectChanged()));
    QTreeWidget::takeTopLevelItem(index);
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentProjectChanged()));
    delete p;   //childItem delete deal with Project class
}

void ProjectTree::TopContextMenuClicked(QAction *action)
{
    if (tr("添加新文件") == action->text())
    {
        emit AddNewFileSignal(GetCurrentProjectDir(), IsCProject());
    } else if (tr("添加现有文件") == action->text())
    {
        emit AddExistingFileSignal(currentProject->projectDir, currentProject->isCProject);
    } else if ("关闭项目" == action->text())
    {
        emit CloseProjectSignal();
    }
}

void ProjectTree::FileContextMenuClicked(QAction *action)
{
    QString filePath =  currentProject->projectDir + currentItem()->text(0);
    if (tr("打开文件") == action->text())
    {
        emit OpenFileSignal(filePath);
    } else if (tr("删除文件") == action->text())
    {
        emit RemoveFileSignal(filePath);
    } else if ("重命名" == action->text())
    {
        originalfileName = currentItem()->text(0);
        openPersistentEditor(currentItem(), 0);
    }else if ("折叠" == action->text())
    {
        currentItem()->parent()->setExpanded(false);
    }
}

void ProjectTree::ShowContextMenu()
{
        int level = ItemLevel(currentItem());
        
        if (0 == level)
            topContextMenu->exec(QCursor::pos());
        
        if (2 == level)
            fileContextMenu->exec(QCursor::pos());

}

int ProjectTree::ItemLevel(QTreeWidgetItem *item)
{
    if (NULL == item->parent()) //0
        return 0;
    
    if (NULL == item->parent()->parent())   //1
        return 1;
    
    return 2;
}

void ProjectTree::AddHeadFile(const QString &fileName)
{
    QStringList fileNameList;
    fileNameList << fileName;
    QTreeWidgetItem *headFile = new QTreeWidgetItem(currentProject->headFolderItem, fileNameList);
    headFile->setIcon(0, QIcon(":/images/hFile.png"));
    currentProject->headFolderItem->addChild(headFile);
    headFile = 0;
}

void ProjectTree::AddSourceFile(const QString &fileName)
{
    QStringList fileNameList;
    fileNameList << fileName;
    QTreeWidgetItem *sourceFile = new QTreeWidgetItem(currentProject->sourceFolderItem, fileNameList);
    
    if (IsCProject())
    {
        sourceFile->setIcon(0, QIcon(":/images/cFile.png"));
    } else
    {
        sourceFile->setIcon(0, QIcon(":/images/cppFile.png"));
    }
    
    currentProject->sourceFolderItem->addChild(sourceFile);
    sourceFile = 0;
}

void ProjectTree::RemoveHeadFile(QTreeWidgetItem *item)
{
    QTreeWidgetItem * i = item;
    currentProject->headFolderItem->removeChild(item);
    delete i;
}

void ProjectTree::RemoveSourceFile(QTreeWidgetItem *item)
{
    QTreeWidgetItem * i = item;
    currentProject->sourceFolderItem->removeChild(item);
    delete i;
}

void ProjectTree::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        if (2 == ItemLevel(currentItem()))
        {
            emit OpenFileSignal(currentProject->projectDir + currentItem()->text(0));
        }
    }
    
    QTreeWidget::mouseDoubleClickEvent(event);
}

Project* ProjectTree::GetCurrentProject()
{
    return currentProject;
}

QTreeWidgetItem* ProjectTree::GetCurrentProjectTopLevelItem()
{
    return currentProject->projcetFolderItem;
}

QTreeWidgetItem* ProjectTree::GetHeadFolderItem()
{
    return currentProject->headFolderItem;
}

QTreeWidgetItem* ProjectTree::GetSourceFolderItem()
{
    return currentProject->sourceFolderItem;
}

int ProjectTree::GetCurrentProjectTopLevelItemIndex()
{
    return indexOfTopLevelItem(currentItem());
}

int ProjectTree::GetProjectCount()
{
    return projectList.count();
}

int ProjectTree::GetHeadFileCount()
{
    return currentProject->headFolderItem->childCount();
}

int ProjectTree::GetSourceFileCount()
{
    return currentProject->sourceFolderItem->childCount();
}
