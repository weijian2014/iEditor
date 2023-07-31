/****************************************************************************
**
**      Datetime:   2013年03月21日 星期四 15时43分13秒 
**      Author  :   韦   健
**      Titles  :   主窗体类，派生于QMainWindow,iEditor主要的类
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>
#include <QTextDocument>
#include <QListWidgetItem>
#include <QList>
#include "ConfigStruct.h"

class QAction;
class QMenu;
class QString;
class QLabel;
class NewWizard;
class TextEditTab;
class TextEdit;
class FindDialog;
class OptionDialog;
class SaveDialog;
class OutputDock;
class ProjectDock;
class ProjectTree;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void New();
    void NewTab();
    void Open(const QString &fileFullPath = "");
    bool Save();
    void SaveAs();
    void OpenRecentFile();
    void Close();
    
    void Revoke();
    void Recover();
    void Cut();
    void Copy();
    void Paste();
    void SelectAll();
    void Find();
    
    bool Compile();
    bool Link();
    void Run();
    void StopRun();
    
    void BeginDebug();
    void SingleRun();
    void SingleSkin();
    void SetBreakPoint();
    void DeleteBreakPoint();
    void StopDebug();
    
    void WordCount();
    void FullScreen(bool flags);
    void LockTools(bool flags);
    void Options();
    
    void AboutIEditor();

    void ShowPosition();
    void RevokeAvailable(bool flags);
    void RecoverAvailable(bool flags);
    void CutAvailable(bool flags);
    void CopyAvailable(bool flags);
    void TextChanged();
    void IndexChanged(int index);
    void TabClose(int index, bool isShowWarning = true);
    bool OkToContinue(int index, bool isShowWarning);
    void FindMach();
    void Replace();
    void ReplaceAll();
    void SetTextEdit();
    void DefineDockWidgetArea(Qt::DockWidgetArea area);
    void SetIsToContinue(const QString &action);
    void SaveFilesByStringList(QList<QListWidgetItem *> items);
    void ChangeTabIndex(int index);
    bool CreateNewTab(bool isNew);
    void CreateNewFile(const QString &, int projectType = 0);
    void RemoveProjectFile(const QString &filePath);
    void RenameProjectFile(const QString &originalilePath, const QString &newFilePath);
    void DeleteProjectFile(const QString &filePath, bool isDelete);
    bool RemoveFile(const QString &filePath);
    void AddNewFileToProject(const QString &fileDir, bool isCProject);
    void AddExistingToProject(const QString &fileDir, bool isCProject);
    void CloseProject();
    
private:
    void MainWindowSetup();
    void CreateAction();
    void CreateMenu();
    void CreateTools();
    void CreateStatus();
    void CreateProjectDock();
    void CreateOutputDock();
    void CreateProjectTree();
    void SetContextAvailable(bool flags);
    void UpdateRecentFileActions();
    void UpdateRecentProjectFileActions();
    int GetNewTabCount();
    void RemoveNewTabCount(int fileNumber);
    
    void SetNewTextEdit(int index, int newTabCount);
    void CreateTabWidget();
    void SetEnableAction();
    bool LoadFile(const QString &filePath);
    bool SaveFile(const QString &filePath);
    QString StrippedFileName(const QString &filePath);
    QString StrippedFileDir(const QString &filePath);
    QString StrippedExecName(const QString &filePath);
    void SetCurrentFile(const QString &filePath);
    void FindString(const QTextCursor &cursor, QTextDocument::FindFlags findFlag);
    void FindRegExp(const QTextCursor &cursor, QTextDocument::FindFlags findFlag);
    void WriteMainWindowConfig();
    void ReadMainWindowConfig();
    void SaveFilesByClose();
    int FindIndexByFileName(const QString &fileName);
    int FindIndexByFilePath(const QString &filePath);
    void SetBuildToolBarEnable(bool flags);
    void IsBuildToolBarEnable();
    void SetBuildStruct(bool isGCC);
    void ReadProject(const QString &projectPath);
    bool IsHeadFile(const QString &);
    void AddFileToProject(const QString &);
    void WriteProject();
    void CreateProject(const QString &, const QString &, const QString , int);
    
private:
    enum { MaxTabs = 20 };
    enum { MaxRecentFiles = 8 };
    enum { MaxRecentProjectFiles = 8};
    
    QAction *newAction;
    QAction *newTabAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *recentFileAction[MaxRecentFiles];
    QAction *recentProjectFilesAction[MaxRecentProjectFiles];
    QAction *closeAction;
    QAction *exitAction;
    
    QAction *revokeAction;
    QAction *recoverAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *selectAllAction;
    QAction *findAction;
    
    QAction *compileAction;
    QAction *linkAction;
    QAction *runAction;
    QAction *stopRunAction;
    
    QAction *beginDebugAction;
    QAction *singleRunAction;
    QAction *singleSkinAction;
    QAction *setBreakPointAction;
    QAction *deleteBreakPointAction;
    QAction *stopDebugAction;
    
    QAction *projectManageAction;
    QAction *outputDockAction;
    QAction *fileToolsBarAction;
    QAction *editToolsBarAction;
    QAction *buildToolsBarAction;
    QAction *debugToolsBarAction;
    QAction *statusBarAction;
    
    QAction *wordCountAction;
    QAction *fullScreenAction;
    QAction *lockToolsAction;
    QAction *optionsAction;
    
    QAction *aboutQtAction;
    QAction *aboutIEditorAction;
    
    QMenu *fileMenu;
    QMenu *recentFilesMenu;
    QMenu *recentProjectFilesMenu;
    QMenu *editMenu;
    QMenu *buildMenu;
    QMenu *debugMenu;
    QMenu *windowMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *buildToolBar;
    QToolBar *debugToolBar;
    
    QLabel *positionLabel;  //cusor positoin
    
    NewWizard *newWizard;
    TextEditTab *tabWidget;
    FindDialog *findDialog;
    OptionDialog *optionDialog;
    SaveDialog *saveDialog;
    ProjectDock *projectDock;
    OutputDock *outputDock;
    ProjectTree *projectTree;
    TextEdit *currentTextEdit;
    QList<TextEdit *> textEditList;
    
    QStringList recentFiles;
    QStringList recentProjectFiles;
    
    int currentTabIndex;
    bool isToContinue;  //usr close false
    QString currentFileName;
    QString projectDockPosition;
    QTextCursor textCursor;
    QList<int> fileNumberList;

    QString recentDir;
    
    //iEditor project arg
    quint32 magic;
    quint32 version;
    QString headFile;
    QString sourceFile;
    QString cFile;
    QString cppFile;
    
    //usr for build
    struct BuildStruct
    {
        //compile
        QString objName;
        QString exeName;    
        QString objFilePath;
        QString exeFilePath;
        
        QString fileName;   //just file name
        QString fileDir;    //no include file name
        QString fileExecName;    //current file extension name
        QString terminalTitle;  //terminal title for run
        bool isGCC;
    } build;
    
};//MainWindow

#endif // MAINWINDOW_H
