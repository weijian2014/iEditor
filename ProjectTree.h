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
#ifndef ProjectTree_H
#define ProjectTree_H

#include <QTreeWidget>
#include <QStringList>

class QTreeWidgetItem;
class Project;
class QMenu;
class QAction;

class ProjectTree : public QTreeWidget
{
    Q_OBJECT
    
public:
    explicit ProjectTree(QWidget *parent = 0);
    ~ProjectTree();
    
    void NewProject(const QString &projectName, const QString &fileDir, bool rojectType);
    void AddHeadFile(const QString &fileName);
    void AddSourceFile(const QString &fileName);
    void RemoveHeadFile(QTreeWidgetItem *item);
    void RemoveSourceFile(QTreeWidgetItem *item);
    void takeTopLevelItem(int index);
    int GetProjectCount();
    int GetHeadFileCount();
    int GetSourceFileCount();
    int GetCurrentProjectTopLevelItemIndex();
    bool ProjectExist(const QString &projectFullPath);
    bool ItemNameExist(const QString &fileName);
    bool IsCProject();
    QString GetCurrentProjectName();
    QString GetCurrentPojectFullPath();
    QString GetCurrentProjectDir();
    Project* GetCurrentProject();
    QTreeWidgetItem* GetCurrentProjectTopLevelItem();
    QTreeWidgetItem* GetHeadFolderItem();
    QTreeWidgetItem* GetSourceFolderItem();
    
protected:
    void keyReleaseEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    
signals:
    void OpenFileSignal(const QString &filePath);
    void RemoveFileSignal(const QString &filePath);
    void RenameSignal(const QString &originalfilePath, const QString &newFilePath);
    void AddNewFileSignal(const QString &fileDir, bool isCProject);
    void AddExistingFileSignal(const QString &fileDir, bool isCProject);
    void CloseProjectSignal();
    
private slots:
    void CurrentProjectChanged();
    void ShowContextMenu();
    void TopContextMenuClicked(QAction *);
    void FileContextMenuClicked(QAction *);
    
private:
    void CreteaTopContextMenu();
    void CreteaFileContextMenu();
    int ItemLevel(QTreeWidgetItem *item);
    
private:
    QList<Project *> projectList;
    
    Project *currentProject;
    
    QMenu *topContextMenu;
    QMenu *fileContextMenu;
    
    QAction *addNewFileAction;
    QAction *addExistingFileAction;
    QAction *closeProjectAction;
    
    QAction *openFileAction;
    QAction *removeFileAction;
    QAction *renameFileAction;
    QAction *foldAction;
    
    QString originalfileName;
};

#endif // ProjectTree_H
