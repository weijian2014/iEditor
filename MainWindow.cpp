/****************************************************************************
**
**      Datetime:   2013年03月21日 星期四 15时43分13秒
**      Author  :   韦   健
**      Titles  :   主窗体类，派生于QMainWindow,iEditor主要的类
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include <QtGui>
#include <QApplication>
#include <MainWindow.h>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QLineEdit>
#include <QFileDialog>
#include <QProcess>

#include "TextEditTab.h"
#include "TextEdit.h"
#include "FindDialog.h"
#include "OptionDialog.h"
#include "SaveDialog.h"
#include "OutputDock.h"
#include "ProjectDock.h"
#include "NewWizard.h"
#include "ProjectTree.h"
#include "Project.h"

//using C/C++ lib  exec GCC\G++\GDB\terminal(system open dup2...)
//#include<stdio.h>
//#include<iostream>
//#include<unistd.h>
//#include<fcntl.h>
//#include<sys/wait.h>

MainWindow::MainWindow()
{
    MainWindowSetup();
    CreateAction();
    CreateMenu();
    CreateTools();
    CreateStatus();
    CreateTabWidget();
    CreateProjectDock();
    CreateOutputDock();
    ReadMainWindowConfig();

    //open recentFile and recentProject
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        if (recentFileAction[i]->isVisible())
        {
            Open(recentFileAction[i]->data().toString());
            break;
        }
    }

    for (int i = 0; i < MaxRecentProjectFiles; ++i)
    {
        if (recentProjectFilesAction[i]->isVisible())
        {
            Open(recentProjectFilesAction[i]->data().toString());
            break;
        }
    }

    //setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::MainWindowSetup()
{
    findDialog = 0;
    optionDialog = 0;
    tabWidget = 0;
    isToContinue = true;

    magic = 0x8F66BC81;
    version = 11;
    headFile = "头文件";
    sourceFile = "源文件";
    cFile = "main.c";
    cppFile = "main.cpp";

#ifdef Q_WS_X11
    build.objName = ".o";
    build.exeName = "";
    //chPath = 47;
#endif

#ifdef Q_WS_WIN
    //chPath = 92;
    build.objName = ".obj";
    build.exeName = ".exe";
#endif

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    setWindowIcon(QIcon(":/images/iEditor.png"));
    setWindowTitle("iEditor v1.1");
}

void MainWindow::CreateAction()
{
    newAction = new QAction(QIcon(":/images/iEditor.png"),tr("新建文件或项目(&N)"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setToolTip(tr("新建文件或项目(Ctrl+N)"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(New()));

    newTabAction = new QAction(QIcon(":/images/new.png"),tr("新建标签页(&T)"), this);
    newTabAction->setShortcut(tr("Ctrl+T"));
    newTabAction->setToolTip(tr("新建一个标签(Ctrl+T)"));
    connect(newTabAction, SIGNAL(triggered()), this, SLOT(NewTab()));

    openAction = new QAction(QIcon(":/images/open.png"), tr("打开文件或项目(&O)"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setToolTip(tr("打开文件或项目(Ctrl+O)"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(Open()));

    saveAction = new QAction(QIcon(":/images/save.png"), tr("保存"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setToolTip(tr("保存(Ctrl+S)"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(Save()));

    saveAsAction = new QAction(QIcon(":/images/saveAs.png"), tr("另存为"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setToolTip(tr("另存为(Ctrl+Shift+S)"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(SaveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileAction[i] = new QAction(this);
        recentFileAction[i]->setVisible(false);
        connect(recentFileAction[i], SIGNAL(triggered()), this, SLOT(OpenRecentFile()));
    }

    for (int i = 0; i < MaxRecentProjectFiles; ++i)
    {
        recentProjectFilesAction[i] = new QAction(this);
        recentProjectFilesAction[i]->setVisible(false);
        connect(recentProjectFilesAction[i], SIGNAL(triggered()), this, SLOT(OpenRecentFile()));
    }

    closeAction = new QAction(QIcon(":/images/close.png"), tr("关闭当前文件"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setToolTip(tr("关闭当前文件(Ctrl+W)"));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(Close()));

    exitAction = new QAction(QIcon(":/images/exit.png"), tr("退出"), this);
    exitAction->setShortcut(tr("Alt+F4"));
    exitAction->setToolTip(tr("退出iEditor(Alt+F4)"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));



    revokeAction = new QAction(QIcon(":/images/revoke.png"), tr("撤消"), this);
    revokeAction->setShortcut(QKeySequence::Undo);
    revokeAction->setToolTip(tr("撤消(Ctrl+Z)"));
    connect(revokeAction, SIGNAL(triggered()), this, SLOT(Revoke()));

    recoverAction = new QAction(QIcon(":/images/recover.png"), tr("恢复"), this);
    recoverAction->setShortcut(QKeySequence::Redo);
    recoverAction->setToolTip(tr("恢复(Ctrl+Shift+Z)"));
    connect(recoverAction, SIGNAL(triggered()), this, SLOT(Recover()));

    cutAction = new QAction(QIcon(":/images/cut.png"), tr("剪切"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setToolTip(tr("剪切(Ctrl+X)"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(Cut()));

    copyAction = new QAction(QIcon(":/images/copy.png"), tr("复制"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setToolTip(tr("复制(Ctrl+C)"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(Copy()));

    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("粘贴"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setToolTip(tr("粘贴(Ctrl+V)"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(Paste()));

    selectAllAction = new QAction(QIcon(":/images/selectAll.png"), tr("全选"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setToolTip(tr("全选(Ctrl+A)"));
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(SelectAll()));

    findAction = new QAction(QIcon(":/images/find.png"), tr("查找|替换"), this);
    findAction->setShortcut(QKeySequence::Find);
    findAction->setToolTip(tr("查找|替换(Ctrl+F)"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(Find()));




    compileAction = new QAction(QIcon(":/images/compile.png"), tr("编译"), this);
    compileAction->setShortcut(tr("Ctrl+B"));
    compileAction->setToolTip(tr("编译(Ctrl+B)"));
    connect(compileAction,SIGNAL(triggered()),this,SLOT(Compile()));

    linkAction = new QAction(QIcon(":/images/link.png"), tr("链接"), this);
    linkAction->setShortcut(tr("Ctrl+L"));
    linkAction->setToolTip(tr("链接(Ctrl+L)"));
    connect(linkAction, SIGNAL(triggered()), this, SLOT(Link()));

    runAction = new QAction(QIcon(":/images/run.png"), tr("运行"), this);
    runAction->setShortcut(tr("Ctrl+R"));
    runAction->setToolTip(tr("运行(Ctrl+R)"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(Run()));

    stopRunAction = new QAction(QIcon(":/images/stop.png"), tr("停止运行"), this);
    stopRunAction->setShortcut(tr("Ctrl+Shift+R"));
    stopRunAction->setToolTip(tr("停止运行(Ctrl+Shift+R)"));
    connect(stopRunAction, SIGNAL(triggered()), this, SLOT(StopRun()));



    beginDebugAction = new QAction(QIcon(":/images/beginDebug.png"), tr("开始调试"), this);
    beginDebugAction->setToolTip(tr("开始调试"));
    connect(beginDebugAction, SIGNAL(triggered()), this, SLOT(BeginDebug()));

    singleRunAction = new QAction(QIcon(":/images/singleRun.png"), tr("单步执行"), this);
    singleRunAction->setToolTip(tr("单步执行"));
    connect(singleRunAction, SIGNAL(triggered()), this, SLOT(SingleRun()));

    singleSkinAction = new QAction(QIcon(":/images/singleSkin.png"), tr("单步跳过"), this);
    singleSkinAction->setToolTip(tr("单步跳过"));
    connect(singleSkinAction, SIGNAL(triggered()), this, SLOT(SingleSkin()));

    setBreakPointAction = new QAction(QIcon(":/images/setBreakPoint.png"), tr("设置断点"), this);
    setBreakPointAction->setToolTip(tr("设置断点"));
    connect(setBreakPointAction, SIGNAL(triggered()), this, SLOT(SetBreakPoint()));

    deleteBreakPointAction = new QAction(QIcon(":/images/deleteBreakPoint.png"), tr("删除断点"), this);
    deleteBreakPointAction->setToolTip(tr("删除断点"));
    connect(deleteBreakPointAction, SIGNAL(triggered()), this, SLOT(DeleteBreakPoint()));

    stopDebugAction = new QAction(QIcon(":/images/stop.png"), tr("停止调试"), this);
    stopDebugAction->setToolTip(tr("停止调试"));
    connect(stopDebugAction, SIGNAL(triggered()), this, SLOT(StopDebug()));


    projectManageAction = new QAction(tr("项目管理(&P)"), this);
    projectManageAction->setShortcut(tr("Ctrl+Shift+P"));
    projectManageAction->setCheckable(true);
    projectManageAction->setToolTip(tr("显示|隐藏项目管理窗口(Ctrl+Shift+P)"));

    outputDockAction = new QAction(tr("信息管理(&M)"), this);
    outputDockAction->setShortcut(tr("Ctrl+Shift+M"));
    outputDockAction->setCheckable(true);
    outputDockAction->setToolTip(tr("显示|隐藏信息管理窗口(Ctrl+Shift+M)"));

    fileToolsBarAction = new QAction(tr("文件工具栏(&F)"), this);
    fileToolsBarAction->setShortcut(tr("Ctrl+Shift+F"));
    fileToolsBarAction->setCheckable(true);
    fileToolsBarAction->setToolTip(tr("显示|隐藏文件工具栏(Ctrl+Shift+F)"));

    editToolsBarAction = new QAction(tr("编辑工具栏(&E)"), this);
    editToolsBarAction->setShortcut(tr("Ctrl+Shift+E"));
    editToolsBarAction->setCheckable(true);
    editToolsBarAction->setToolTip(tr("显示|隐藏编辑工具栏(Ctrl+Shift+E)"));

    buildToolsBarAction = new QAction(tr("构建工具栏(&B)"), this);
    buildToolsBarAction->setShortcut(tr("Ctrl+Shift+B"));
    buildToolsBarAction->setCheckable(true);
    buildToolsBarAction->setToolTip(tr("显示|隐藏构建工具栏(Ctrl+Shift+B)"));

    debugToolsBarAction = new QAction(tr("调试工具栏(&D)"), this);
    debugToolsBarAction->setShortcut(tr("Ctrl+Shift+D"));
    debugToolsBarAction->setCheckable(true);
    debugToolsBarAction->setToolTip(tr("显示|隐藏调试工具栏(Ctrl+Shift+D)"));

    statusBarAction = new QAction(tr("状态栏(&S)"), this);
    statusBarAction->setShortcut(tr("Ctrl+Shift+K"));
    statusBarAction->setCheckable(true);
    statusBarAction->setToolTip(tr("显示|隐藏状态栏(Ctrl+Shift+K)"));


    wordCountAction = new QAction(QIcon(":/images/wordCount.png"), tr("字数统计"), this);
    wordCountAction->setShortcut(tr("Ctrl+Shift+W"));
    wordCountAction->setToolTip(tr("字数统计(Ctrl+Shift+W)"));
    connect(wordCountAction, SIGNAL(triggered()), this, SLOT(WordCount()));

    lockToolsAction = new QAction(QIcon(":/images/lockTools.png"), tr("锁定工具栏(&L)"), this);
    lockToolsAction->setShortcut(tr("Ctrl+L"));
    lockToolsAction->setCheckable(true);
    lockToolsAction->setToolTip(tr("锁定工具栏(Ctrl+L)"));
    connect(lockToolsAction, SIGNAL(toggled(bool)), this, SLOT(LockTools(bool)));

    fullScreenAction = new QAction(QIcon(":/images/fullScreen.png"), tr("全屏显示(&F)"), this);
    fullScreenAction->setShortcut(Qt::Key_F11);
    fullScreenAction->setCheckable(true);
    fullScreenAction->setToolTip(tr("全屏显示(F11)"));
    connect(fullScreenAction, SIGNAL(toggled(bool)), this, SLOT(FullScreen(bool)));

    optionsAction = new QAction(QIcon(":/images/options.png"), tr("首选项(&O)"), this);
    optionsAction->setShortcut(Qt::Key_F5);
    optionsAction->setToolTip(tr("首选项(F5)"));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(Options()));



    aboutQtAction = new QAction(QIcon(":/images/qt.png"), tr("关于QT"), this);
    aboutQtAction->setToolTip(tr("关于QT"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutIEditorAction = new QAction(QIcon(":/images/iEditor.png"), tr("关于iEditor"), this);
    aboutIEditorAction->setToolTip(tr("关于iEditor"));
    connect(aboutIEditorAction, SIGNAL(triggered()), this, SLOT(AboutIEditor()));

    //开始时禁止要禁止的菜单项
    SetEnableAction();
}

void MainWindow::CreateMenu()
{
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(newTabAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    fileMenu->addSeparator();
    recentFilesMenu = fileMenu->addMenu(tr("最近打开的文件..."));
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFilesMenu->addAction(recentFileAction[i]);
    }

    recentProjectFilesMenu = fileMenu->addMenu(tr("最近打开的工程..."));
    for (int i = 0; i < MaxRecentProjectFiles; ++i)
    {
        recentProjectFilesMenu->addAction(recentProjectFilesAction[i]);
    }
    fileMenu->addSeparator();

    fileMenu->addAction(closeAction);
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(revokeAction);
    editMenu->addAction(recoverAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);

    buildMenu = menuBar()->addMenu(tr("构建(&B)"));
    buildMenu->addAction(compileAction);
    buildMenu->addAction(linkAction);
    buildMenu->addAction(runAction);
    buildMenu->addSeparator();
    buildMenu->addAction(stopRunAction);

    debugMenu = menuBar()->addMenu(tr("调试(&D)"));
    debugMenu->addAction(beginDebugAction);
    debugMenu->addAction(singleRunAction);
    debugMenu->addAction(singleSkinAction);
    debugMenu->addSeparator();
    debugMenu->addAction(setBreakPointAction);
    debugMenu->addAction(deleteBreakPointAction);
    debugMenu->addSeparator();
    debugMenu->addAction(stopDebugAction);

    windowMenu = menuBar()->addMenu(tr("窗口(&W)"));
    windowMenu->addAction(projectManageAction);
    windowMenu->addAction(outputDockAction);
    windowMenu->addSeparator();
    windowMenu->addAction(fileToolsBarAction);
    windowMenu->addAction(editToolsBarAction);
    windowMenu->addAction(buildToolsBarAction);
    windowMenu->addAction(debugToolsBarAction);
    windowMenu->addSeparator();
    windowMenu->addAction(statusBarAction);

    toolsMenu = menuBar()->addMenu(tr("工具(&T)"));
    toolsMenu->addAction(wordCountAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(fullScreenAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(lockToolsAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(optionsAction);

    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutIEditorAction);
}

void MainWindow::CreateTools()
{
    fileToolBar = addToolBar(tr("文件工具栏"));
    fileToolBar->setObjectName(tr("fileToolBar"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(newTabAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(closeAction);

    editToolBar = addToolBar(tr("编辑工具栏"));
    editToolBar->setObjectName(tr("editToolBar"));
    editToolBar->addAction(revokeAction);
    editToolBar->addAction(recoverAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);

    buildToolBar = addToolBar(tr("构建工具栏"));
    buildToolBar->setObjectName(tr("buildToolBar"));
    buildToolBar->addAction(compileAction);
    buildToolBar->addAction(linkAction);
    buildToolBar->addAction(runAction);
    buildToolBar->addSeparator();
    buildToolBar->addAction(stopRunAction);

    debugToolBar = addToolBar(tr("调试工具栏"));
    debugToolBar->setObjectName(tr("debugToolBar"));
    debugToolBar->addAction(beginDebugAction);
    debugToolBar->addAction(singleRunAction);
    debugToolBar->addAction(singleSkinAction);
    debugToolBar->addSeparator();
    debugToolBar->addAction(setBreakPointAction);
    debugToolBar->addAction(deleteBreakPointAction);
    debugToolBar->addSeparator();
    debugToolBar->addAction(stopDebugAction);

    connect(fileToolsBarAction, SIGNAL(toggled(bool)), fileToolBar, SLOT(setVisible(bool)));
    connect(fileToolBar, SIGNAL(visibilityChanged(bool)), fileToolsBarAction, SLOT(setChecked(bool)));
    connect(editToolsBarAction, SIGNAL(toggled(bool)), editToolBar, SLOT(setVisible(bool)));
    connect(editToolBar, SIGNAL(visibilityChanged(bool)), editToolsBarAction, SLOT(setChecked(bool)));
    connect(buildToolsBarAction, SIGNAL(toggled(bool)), buildToolBar, SLOT(setVisible(bool)));
    connect(buildToolBar, SIGNAL(visibilityChanged(bool)), buildToolsBarAction, SLOT(setChecked(bool)));
    connect(debugToolsBarAction, SIGNAL(toggled(bool)), debugToolBar, SLOT(setVisible(bool)));
    connect(debugToolBar, SIGNAL(visibilityChanged(bool)), debugToolsBarAction, SLOT(setChecked(bool)));
}

void MainWindow::CreateStatus()
{
    positionLabel = new QLabel(tr("光标位置"));
    positionLabel->setAlignment(Qt::AlignHCenter);
    positionLabel->setMinimumSize(positionLabel->sizeHint());
    positionLabel->setFrameShadow(QFrame::Sunken);
    positionLabel->setFrameShape(QFrame::Panel);
    statusBar()->addPermanentWidget(positionLabel);
    connect(statusBarAction, SIGNAL(toggled(bool)), statusBar(), SLOT(setVisible(bool)));
}

void MainWindow::CreateTabWidget()
{
    tabWidget = new TextEditTab;
    setCentralWidget(tabWidget);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(IndexChanged(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(TabClose(int)));
    connect(tabWidget, SIGNAL(LeftMouseDoubleClickOnTextEditTabSignal(bool)), this, SLOT(CreateNewTab(bool)));
    connect(tabWidget, SIGNAL(LeftMouseDoubleClickOnTabSignal(int)), this, SLOT(TabClose(int)));
}

void MainWindow::CreateProjectDock()
{
    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindowConfig");
    projectDockPosition = settings.value("projectDockPosition").toString();
    settings.endGroup();

    projectDock = new ProjectDock(this);

    if ("Left" == projectDockPosition)
        addDockWidget(Qt::LeftDockWidgetArea, projectDock);
    else
        addDockWidget(Qt::RightDockWidgetArea, projectDock);

    connect(projectManageAction, SIGNAL(toggled(bool)), projectDock, SLOT(setVisible(bool)));
    connect(projectDock, SIGNAL(visibilityChanged(bool)), projectManageAction, SLOT(setChecked(bool)));
    connect(projectDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(DefineDockWidgetArea(Qt::DockWidgetArea)));
    connect(projectDock, SIGNAL(ListWidgetRowChangedSignal(int)), this, SLOT(ChangeTabIndex(int)));
    CreateProjectTree();
}

void MainWindow::CreateProjectTree()
{
    projectTree = new ProjectTree;
    projectDock->AddTabToProjectDock(projectTree, tr("项目管理"));

    connect(projectTree, SIGNAL(OpenFileSignal(QString)), this, SLOT(Open(QString)));
    connect(projectTree, SIGNAL(RemoveFileSignal(QString)), this, SLOT(RemoveProjectFile(QString)));
    connect(projectTree, SIGNAL(RenameSignal(QString, QString)), this, SLOT(RenameProjectFile(QString, QString)));
    connect(projectTree, SIGNAL(RenameSignal(QString,QString)), this, SLOT(RenameProjectFile(QString,QString)));
    connect(projectTree, SIGNAL(AddNewFileSignal(QString, bool)), this, SLOT(AddNewFileToProject(QString, bool)));
    connect(projectTree, SIGNAL(AddExistingFileSignal(QString, bool)), this, SLOT(AddExistingToProject(QString, bool)));
    connect(projectTree, SIGNAL(CloseProjectSignal()), this, SLOT(CloseProject()));
}

void MainWindow::CreateOutputDock()
{
    outputDock = new OutputDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, outputDock);

    connect(outputDockAction, SIGNAL(toggled(bool)), outputDock, SLOT(setVisible(bool)));
    connect(outputDock, SIGNAL(visibilityChanged(bool)), outputDockAction, SLOT(setChecked(bool)));
}

//////////////////////////////  up is mainwindow setup

void MainWindow::UpdateRecentFileActions()
{
    QMutableStringListIterator iter(recentFiles);

    while (iter.hasNext())
    {
        if (!QFile::exists(iter.next()))
        {
            iter.remove();
        }
    }

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        if (i < recentFiles.count())
        {
            QString text = tr("&%1 %2").arg( i+1 ).arg(recentFiles[i]);
            recentFileAction[i]->setText(text);
            recentFileAction[i]->setData(recentFiles[i]);
            recentFileAction[i]->setVisible(true);
        } else
        {
            recentFileAction[i]->setVisible(false);
        }
    }
}

void MainWindow::UpdateRecentProjectFileActions()
{
    QMutableStringListIterator iter(recentProjectFiles);

    while (iter.hasNext())
    {
        if (!QFile::exists(iter.next()))
        {
            iter.remove();
        }
    }

    for (int i = 0; i < MaxRecentProjectFiles; ++i)
    {
        if (i < recentProjectFiles.count())
        {
            QString text = tr("&%1 %2").arg( i+1 ).arg(recentProjectFiles[i]);
            recentProjectFilesAction[i]->setText(text);
            recentProjectFilesAction[i]->setData(recentProjectFiles[i]);
            recentProjectFilesAction[i]->setVisible(true);
        } else
        {
            recentProjectFilesAction[i]->setVisible(false);
        }
    }
}

void MainWindow::OpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        Open(action->data().toString());
    }
}

bool MainWindow::CreateNewTab(bool isNew)
{
    if (textEditList.count() < MaxTabs)
    {
        currentTabIndex = textEditList.count();
        int newTabCount = 0;
        if (isNew)
            newTabCount = GetNewTabCount();
        currentFileName = tr("新建标签%1").arg(newTabCount);
        textEditList.append(new TextEdit());
        currentTextEdit = textEditList[currentTabIndex];
        tabWidget->addTab(currentTextEdit , tr("新建标签%1").arg(newTabCount));
        tabWidget->setCurrentIndex(currentTabIndex);
        tabWidget->setTabToolTip(currentTabIndex, tr("新建标签%1").arg(newTabCount));
        SetNewTextEdit(currentTabIndex, newTabCount);
        if (isNew)
        {
            projectDock->AddItemToOpenFileManagementListWidget(tr("新建标签%1").arg(newTabCount));
            projectDock->SetCurrentRowByOpenFileManagementListWidget(currentTabIndex);
        }
        return true;
    } else
    {
        QMessageBox::warning(this, tr("提示-iEditor"), tr("已经超过最大标签数"));
        return false;
    }
}

void MainWindow::SetNewTextEdit(int index, int newTabCount)
{
    textEditList[index]->setDocumentTitle(tr("新建标签%1").arg(newTabCount));
    setWindowTitle(tr("新建标签%1-iEditor").arg(newTabCount));

    currentTextEdit->setFocus();
    connect(textEditList[index], SIGNAL(cursorPositionChanged()), this, SLOT(ShowPosition()));
    connect(textEditList[index], SIGNAL(textChanged()), this, SLOT(TextChanged()));
    connect(textEditList[index], SIGNAL(undoAvailable(bool)), this, SLOT(RevokeAvailable(bool)));
    connect(textEditList[index], SIGNAL(redoAvailable(bool)), this, SLOT(RecoverAvailable(bool)));
    connect(textEditList[index], SIGNAL(copyAvailable(bool)), this, SLOT(CutAvailable(bool)));
    connect(textEditList[index], SIGNAL(copyAvailable(bool)), this, SLOT(CopyAvailable(bool)));

    //新建时设置可用的菜单
    closeAction->setEnabled(true);
    findAction->setEnabled(true);
    saveAsAction->setEnabled(true);
    pasteAction->setEnabled(true);
    selectAllAction->setEnabled(true);
    wordCountAction->setEnabled(true);
    SetContextAvailable(false);
    currentTextEdit->propertyAction->setEnabled(false);
}

int MainWindow::GetNewTabCount()
{
    if (fileNumberList.isEmpty())
    {
        fileNumberList.append(1);
        return 1;
    }

    for (int i = 1; i < MaxTabs + 1; ++i)
    {
        if (!fileNumberList.contains(i))
        {
            fileNumberList.append(i);
            return i;
        }
    }
    return 1;
}

void MainWindow::RemoveNewTabCount(int fileNumber)
{
    for (QList<int>::iterator iter = fileNumberList.begin(); iter < fileNumberList.end(); ++iter)
    {
        if (fileNumber == (*iter))
        {
            fileNumberList.erase(iter);
            break;
        }
    }
}

void MainWindow::IndexChanged(int index)
{
    if (index < 0)
        return;
    currentTabIndex = index;
    currentTextEdit = textEditList[index];
    currentFileName = currentTextEdit->documentTitle();
    setWindowTitle(currentFileName + "-iEditor");
    tabWidget->setCurrentIndex(index);
    currentTextEdit->setFocus();
    ShowPosition();
    IsBuildToolBarEnable();
    projectDock->SetCurrentRowByOpenFileManagementListWidget(index);
    saveAction->setEnabled(currentTextEdit->isChange);
}

void MainWindow::ChangeTabIndex(int index)
{
    tabWidget->setCurrentIndex(index);
}

void MainWindow::TextChanged()
{
    if (currentTextEdit->document()->isModified())
    {
        currentTextEdit->isChange = true;
        saveAction->setEnabled(true);
        tabWidget->setTabText(currentTabIndex, currentTextEdit->documentTitle() + "*");
        setWindowTitle(currentTextEdit->documentTitle() + "-iEditor");
        SetContextAvailable(true);
    }
}

void MainWindow::TabClose(int index, bool isShowWarning)
{
    IndexChanged(index);
    if (OkToContinue(index, isShowWarning))
    {
        textEditList.removeAt(index);

        if ("" == currentTextEdit->filePath)
        {
            RemoveNewTabCount(currentFileName.replace("新建标签", "").toInt());
        }

        delete currentTextEdit;
        currentFileName = "";
        currentTextEdit = 0;

        projectDock->RemoveItemByOpenFileManagementListWidget(index);

        if (0 == textEditList.count())
        {
            SetEnableAction();//设置不能使用的菜单
            statusBar()->clearMessage();
            positionLabel->setText(tr("光标位置"));
            return;
        } else
        {
            if(index == textEditList.count())
                IndexChanged(index-1);
            else
                IndexChanged(index);

            statusBar()->showMessage(tr("文件已关闭"), 1000);
            ShowPosition();
        }
    }
}

bool MainWindow::OkToContinue(int index, bool isShowWarning)
{
    if (true == textEditList[index]->isChange)
    {
        int result = QMessageBox::Yes;
        if (isShowWarning)
        {
            result = QMessageBox::warning(this, tr("保存提示-iEditor"), tr("<p> [%1] 已经改变!</p><p>您需要保存此文件吗？</p>").arg(textEditList[index]->documentTitle()), QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
        }

        if (QMessageBox::Yes == result)
        {
            if (!textEditList[index]->filePath.isEmpty())
            {
                SaveFile(textEditList[index]->filePath);
                tabWidget->setTabToolTip(currentTabIndex, textEditList[index]->filePath);
                SetCurrentFile(textEditList[index]->filePath);
                statusBar()->showMessage(tr("文件保存成功！"), 2000);
                return true;
            }

            QString filePath = QFileDialog::getSaveFileName(this, tr("保存文件到-iEditor"), tr("%1%2").arg(recentDir).arg(textEditList[index]->documentTitle()), tr("所有文件 (*.*);;iEditor项目(*.ieditor);;C/C++文件(*.c *.C *.cpp *.CPP *.h *.H);;文本文件 (*.txt)"));

            if (!filePath.isEmpty())
            {
                recentDir = StrippedFileDir(filePath);
                SaveFile(filePath);
                currentTextEdit->filePath = filePath;
                SetCurrentFile(filePath);
                tabWidget->setTabToolTip(currentTabIndex, currentTextEdit->filePath);
                projectDock->ChangeRowTextByOpenFileManagementListWidget(currentTabIndex, currentTextEdit->documentTitle());
                statusBar()->showMessage(tr("文件保存成功！"), 2000);
                return true;
            } else
            {
                return false;
            }
        } else if (QMessageBox::Cancel == result)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::LoadFile(const QString &filePath)
{
    return currentTextEdit->ReadDocument(filePath);
}

bool MainWindow::SaveFile(const QString &filePath)
{
    return currentTextEdit->WriteDocument(filePath);
}

//////////////////////////////////// donw is main slot

void MainWindow::New()
{
    newWizard = new NewWizard;
    connect(newWizard, SIGNAL(FinishButtonClickedSignal(QString, int)), this, SLOT(CreateNewFile(QString, int)));
    newWizard->show();
    newWizard->exec();
    delete newWizard;
    newWizard = 0;
}

void MainWindow::CreateNewFile(const QString &fileFullPath, int projectType)
{
    QString fileDir = StrippedFileDir(fileFullPath);
    QString fileNames = StrippedFileName(fileFullPath);
    QString fileName = fileNames.split(".").first();
    QString fileExec = StrippedExecName(fileFullPath);

    if ("ieditor" == fileExec)      //create project
    {
        CreateProject(fileFullPath, fileDir, fileName, projectType);

    } else                          //create file
    {
        QFile file1(fileFullPath);
        file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file1.close();
        Open(fileFullPath);

        if (0 != projectType)
        {
            AddFileToProject(StrippedFileName(fileFullPath));
        }
    }
}

void MainWindow::NewTab()
{
    if (CreateNewTab(true))
    {
        statusBar()->showMessage(tr("新标签已创建"), 2000);
    }
}

void MainWindow::Open(const QString &fileFullPath)
{
    QString filePath = fileFullPath;
    if (filePath.isEmpty())
    {
        filePath = QFileDialog::getOpenFileName(this, tr("打开文件-iEditor"), recentDir, tr("所有文件 (*.*);;iEditor项目(*.ieditor);;C/C++文件(*.c *.C *.cpp *.CPP *.h *.H);;文本文件 (*.txt)"));
    }

    if (!filePath.isEmpty())
    {
        recentDir = StrippedFileDir(filePath);

        if ("ieditor" == StrippedFileName(filePath).split(".").last())
        {
            if (projectTree->ProjectExist(filePath))
            {
                QMessageBox::warning(this, tr("打开提示-iEditor"), tr("<p>%1</p>已在项目管理中打开").arg(filePath));
                return;
            }

            ReadProject(filePath);
            recentProjectFiles.removeAll(filePath);
            recentProjectFiles.prepend(filePath);
            UpdateRecentProjectFileActions();
        } else
        {
            int index = FindIndexByFilePath(filePath);
            if (-1 != index)
            {
                tabWidget->setCurrentIndex(index);
                return;
            }

            if (CreateNewTab(false))
            {
                if (LoadFile(filePath))
                {
                    statusBar()->showMessage(tr("文件加载成功"), 2000);
                    currentTextEdit->filePath = filePath;
                    SetCurrentFile(filePath);
                    tabWidget->setTabToolTip(currentTabIndex, filePath);
                    projectDock->AddItemToOpenFileManagementListWidget(filePath);
                    projectDock->SetCurrentRowByOpenFileManagementListWidget(currentTabIndex);
                    currentTextEdit->isChange = false;
                    saveAction->setEnabled(false);
                    SetContextAvailable(true);
                    currentTextEdit->propertyAction->setEnabled(true);
                    IsBuildToolBarEnable();
                }
            }
        }
    }
}

bool MainWindow::Save()
{
    if (true == currentTextEdit->isChange)
    {

        if (!OkToContinue(currentTabIndex, false))
            return false;

        statusBar()->showMessage(tr("文件保存成功！"), 2000);
        saveAction->setEnabled(false);
        currentTextEdit->isChange = false;
        SetContextAvailable(true);
        currentTextEdit->propertyAction->setEnabled(true);
        IsBuildToolBarEnable();
    }
    return true;
}

void MainWindow::SaveAs()
{
    QString saveAsFilePath = QFileDialog::getSaveFileName(this, tr("文件另存为-iEditor"), tr("%1%2").arg(recentDir).arg(currentTextEdit->documentTitle()), tr("所有文件 (*.*);;iEditor项目(*.ieditor);;C/C++文件(*.c *.C *.cpp *.CPP *.h *.H);;文本文件 (*.txt)"));

    if (!saveAsFilePath.isEmpty())
    {
        recentDir = StrippedFileDir(saveAsFilePath);
        if (SaveFile(saveAsFilePath)) {
            statusBar()->showMessage(tr("文件成功保存至%1！").arg(saveAsFilePath), 2000);
            currentTextEdit->propertyAction->setEnabled(true);
            currentTextEdit->filePath = saveAsFilePath;
            SetCurrentFile(saveAsFilePath);
            tabWidget->setTabToolTip(currentTabIndex, currentTextEdit->filePath);
            projectDock->ChangeRowTextByOpenFileManagementListWidget(currentTabIndex, currentTextEdit->documentTitle());
            saveAction->setEnabled(false);
            currentTextEdit->isChange = false;
            SetContextAvailable(true);
            IsBuildToolBarEnable();
        } else
        {
            statusBar()->showMessage(tr("文件未保存！"), 2000);
        }
    }
}

void MainWindow::Close()
{
    TabClose(currentTabIndex);
}

void MainWindow::Revoke()
{
    currentTextEdit->undo();
}

void MainWindow::Recover()
{
    currentTextEdit->redo();
}

void MainWindow::Cut()
{
    currentTextEdit->cut();
}

void MainWindow::Copy()
{
    currentTextEdit->copy();
}

void MainWindow::Paste()
{
    currentTextEdit->paste();
}

void MainWindow::SelectAll()
{
    currentTextEdit->selectAll();
}

void MainWindow::Find()
{
    if (!findDialog)
    {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(FindSignal()), this, SLOT(FindMach()));
        connect(findDialog, SIGNAL(ReplaceSignal()), this, SLOT(Replace()));
        connect(findDialog, SIGNAL(ReplaceAllSignal()), this, SLOT(ReplaceAll()));
    }

    findDialog->show();
}

void MainWindow::FindString(const QTextCursor &cursor, QTextDocument::FindFlags findFlag)
{
    textCursor = currentTextEdit->document()->find(findDialog->findLineEdit->text(), cursor, findFlag);

    if (textCursor.isNull())
    {
        QApplication::beep();
        statusBar()->showMessage(tr("查找到文件结尾，请反向查找！"), 1500);
    } else
    {
        currentTextEdit->setTextCursor(textCursor);
    }
}

void MainWindow::FindRegExp(const QTextCursor &cursor,
                            QTextDocument::FindFlags findFlag) {
  textCursor = currentTextEdit->document()->find(
      QRegularExpression(findDialog->findLineEdit->text()), cursor, findFlag);

  if (textCursor.isNull()) {
    QApplication::beep();
    statusBar()->showMessage(tr("查找到文件结尾，请反向查找！"), 1500);
  } else {
    currentTextEdit->setTextCursor(textCursor);
  }
}

void MainWindow::FindMach()
{
    if (findDialog->isFirst){
        textCursor = QTextCursor(currentTextEdit->document());
        findDialog->isFirst = false;
    }

    switch (findDialog->findFlags)
    {
        case 1 :    //从文件开始处查找
          FindString(textCursor, QTextDocument::FindFlag::FindBackward);
          break;

        case 2:  // 区分大小写
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively);
          break;

        case 3:  // 从文件开始处查找\区分大小写
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively);
          break;

        case 4:  // 全词匹配
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 5:  // 从文件开始处查找\全词匹配
          FindString(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 6:  // 区分大小写\全词匹配
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindWholeWords);
          break;

        case 7:  // 从文件开始处查找\区分大小写\全词匹配
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 8:  // 使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindBackward);
          break;
        case 9 :    //从文件开始处查找\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindBackward);
          break;

        case 10:  // 区分大小写\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindCaseSensitively);
          break;

        case 11:  // 从文件开始处查找\区分大小写\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindCaseSensitively);
          break;

        case 12:  // 全词匹配\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 13:  // 从文件开始处查找\全词匹配\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 14:  // 区分大小写\全词匹配\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 15:  // 从文件开始处查找\区分大小写\全词匹配\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 16:  // 向前查找
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindBackward);
          break;

        case 17:  // 向前查找\从文件开始处查找
          FindString(textCursor, QTextDocument::FindFlag::FindBackward);
          break;

        case 18:  // 向前查找\区分大小写
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 19:  // 向前查找\从文件开始处查找\区分大小写
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 20:  // 向前查找\全词匹配
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindWholeWords |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 21:  // 向前查找\全词匹配
          FindString(textCursor, QTextDocument::FindWholeWords |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 22:  // 向前查找\区分大小写\全词匹配
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindWholeWords |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 23:  // 向前查找\从文件开始处查找\区分大小写\全词匹配
          FindString(textCursor, QTextDocument::FindFlag::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords |
                                     QTextDocument::FindFlag::FindBackward);
          break;

        case 24:  // 向前查找\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 25:  // 向前查找\从文件开始处查找\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords);
          break;

        case 26:  // 向前查找\区分大小写\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 27:  // 向前查找\从文件开始处查找\区分大小写\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 28:  // 向前查找\全词匹配\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 29:  // 向前查找\从文件开始处查找\全词匹配\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindFlag::FindWholeWords |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 30:  // 向前查找\区分大小写\全词匹配\使用正则表达式
          textCursor = currentTextEdit->textCursor();
          FindRegExp(textCursor, QTextDocument::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        case 31:  // 向前查找\从文件开始处查找\区分大小写\全词匹配\使用正则表达式
          FindRegExp(textCursor, QTextDocument::FindCaseSensitively |
                                     QTextDocument::FindFlag::FindWholeWords |
                                     QTextDocument::FindFlag::FindWholeWords);
          break;

        default:  // 使用默认查找方式
          textCursor = currentTextEdit->textCursor();
          FindString(textCursor, QTextDocument::FindFlag::FindBackward);
          break;
    }
}

void MainWindow::Replace()
{
    if (currentTextEdit->textCursor().hasSelection())
        currentTextEdit->insertPlainText(findDialog->replaceLineEdit->text());
}

void MainWindow::ReplaceAll()
{
    textCursor = QTextCursor(currentTextEdit->document());
    while (!textCursor.isNull() && !textCursor.atEnd()){
        textCursor = currentTextEdit->document()->find(findDialog->findLineEdit->text(), textCursor, QTextDocument::FindCaseSensitively);
        currentTextEdit->setTextCursor(textCursor);
        currentTextEdit->insertPlainText(findDialog->replaceLineEdit->text());
    }
}

void MainWindow::IsBuildToolBarEnable()
{
    QString extensionName = currentFileName.split(".").last().toLower();
    if ("c" == extensionName)
    {
        SetBuildStruct(true);
        SetBuildToolBarEnable(true);
    } else if ("cpp" == extensionName)
    {
        SetBuildStruct(false);
        SetBuildToolBarEnable(true);
    } else
    {
        SetBuildToolBarEnable(false);
    }
}

void MainWindow::SetBuildStruct(bool isGCC)
{
    if ("" != currentTextEdit->filePath)
    {
        QStringList fileNameAndExecName = currentFileName.split(".");
        QString filePath = currentTextEdit->filePath;
        build.fileDir = filePath.replace(currentFileName, "");
        build.fileName = fileNameAndExecName.first();
        build.fileExecName = fileNameAndExecName.last();
        build.terminalTitle = build.fileName + "-iEditor";
        build.objFilePath = build.fileDir + build.fileName + build.objName;
        build.exeFilePath = build.fileDir + build.fileName + build.exeName;
        build.isGCC = isGCC;
    }
}

void MainWindow::SetBuildToolBarEnable(bool flags)
{
    buildToolBar->setEnabled(flags);
    compileAction->setEnabled(flags);
    runAction->setEnabled(flags);
}

bool MainWindow::Compile()
{
    if (!outputDock->isVisible())
        outputDock->setVisible(true);

    outputDock->ClearBuildOutput();

    if (currentTextEdit->isChange)
    {
        if (!Save())
        {
            QApplication::beep();
            outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>文件 %1 自动保存失败，编译终止！</font></p>").arg(currentTextEdit->filePath));
            return false;
        }
        statusBar()->showMessage(tr("文件 %1 已经自动保存！").arg(currentFileName), 2000);
    }

    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=blue>清理文件</font></p>"));

    if (!RemoveFile(build.objFilePath))
    {
        QApplication::beep();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>清理 %1 失败，编译终止！</font></p>").arg(build.objFilePath));
        return false;
    }

    if (!RemoveFile(build.exeFilePath))
    {
        QApplication::beep();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>清理 %1 失败，编译终止！</font></p>").arg(build.exeFilePath));
        return false;
    }

    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=blue>清理完成</font></p>"));

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setStandardOutputFile("../iEditor/config/buildmessage.wj");

    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#7687F1>********************************开始编译********************************</font></p>"));

    if (build.isGCC)
    {
        process.start(ConfigStruct::GetConfig()->gccPath + " -c " + ConfigStruct::GetConfig()->compilerArgument + " " + currentTextEdit->filePath + " -o " + build.objFilePath);
    } else
    {
        process.start(ConfigStruct::GetConfig()->gppPath + " -c " + ConfigStruct::GetConfig()->compilerArgument + " " + currentTextEdit->filePath + " -o " + build.objFilePath);
    }

    if (!process.waitForFinished())
    {
        QApplication::beep();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>调用编译器超时,编译无法完成，请重新进行编译操作！</font></p>"));
        return false;
    }

    process.close();

    outputDock->ShowBuildMessage("../iEditor/config/buildmessage.wj");
    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#7687F1>********************************编译完成********************************</font></p>"));

    linkAction->setEnabled(outputDock->isLinkEnable);
    return true;
}

bool MainWindow::Link()
{
    if (!outputDock->isLinkEnable)
        return false;
    if (!outputDock->isVisible())
        outputDock->setVisible(true);

    QFile file(build.objFilePath);
    if (!file.exists())
    {
        QApplication::beep();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>目标文件[%1]不存在，请编译后再进行链接操作</font></p>").arg(build.objFilePath));
        return false;
    }

    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#8F54A5>********************************开始链接********************************</font></p>"));

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setStandardOutputFile("../iEditor/config/buildmessage.wj");

    if (build.isGCC)
    {
        process.start(ConfigStruct::GetConfig()->gccPath + " -o " + build.exeFilePath + " " + build.objFilePath);
    } else
    {
        process.start(ConfigStruct::GetConfig()->gppPath + " -o " + build.exeFilePath + " " + build.objFilePath);
    }

    if (!process.waitForFinished(10000))
    {
        QApplication::beep();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>调用编译器超时,链接无法完成，请重新进行编译操作！</font></p>"));
        return false;
    }

    process.close();

    outputDock->ShowBuildMessage(":/iEditor/config/buildmessage.wj");
    linkAction->setEnabled(false);
    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#8F54A5>********************************链接完成********************************</font></p>"));
    return true;
}

void MainWindow::Run()
{
    if (!outputDock->isVisible())
        outputDock->setVisible(true);

    QFile file(build.exeFilePath);
    if (!file.exists())
    {
        QApplication::beep();
        if (!Compile())
            return;
        if (!Link())
            return;
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=red>可执行文件[%1]不存在，iEditor将会执行编译和链接操作</font></p>").arg(build.exeFilePath));
    }


    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#FF8817>********************************开始运行********************************</font></p>"));

    int wait = 0;
    while(wait != 2) {
        ++wait;
        QCoreApplication::processEvents();
    }

    QProcess  process;
    QString titleArg = ConfigStruct::GetConfig()->terminalArgument.split(" ").first() + build.terminalTitle;

    if (0 > process.execute(tr("%1 %2 -x bash -c %3;read line").arg(ConfigStruct::GetConfig()->terminalPath).arg(titleArg).arg(build.exeFilePath)))
    {
        QApplication::beep ();
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("\n<p><font color=red>系统错误：%1，运行终止！</font></p>").arg(process.errorString()));
        return;
    }

    QProcess::ExitStatus exitStatus = process.exitStatus();
    if (QProcess::NormalExit == exitStatus)
    {
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#FF8817>程序正常退出，退出代码：%1</font></p>").arg(process.exitCode()));
    } else if (QProcess::CrashExit == exitStatus)
    {
        outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#FF8817>程序崩溃，退出代码：%1</font></p>").arg(process.exitCode()));
    }

    process.close();
    outputDock->AppendHtmlToBuildOutputPlainTextEdit(tr("<p><font color=#FF8817>********************************运行结束********************************</font></p>"));
}

void MainWindow::StopRun()
{

}

void MainWindow::BeginDebug()
{

}

void MainWindow::SingleRun()
{

}

void MainWindow::SingleSkin()
{

}

void MainWindow::SetBreakPoint()
{

}

void MainWindow::DeleteBreakPoint()
{

}

void MainWindow::StopDebug()
{

}

void MainWindow::WordCount()
{
    QMessageBox::information(this, tr("字数统计-iEditor"), tr("<h4>文件数字:  %1 字</h4><h4>文件块数: %2 块</h4><h4>文件行数: %3 行</h4>").arg(currentTextEdit->document()->characterCount()).arg(currentTextEdit->document()->blockCount()).arg(currentTextEdit->document()->lineCount()));
}

void MainWindow::FullScreen(bool flags)
{
    if (flags)
        showFullScreen();
    else
        showNormal();
}

void MainWindow::LockTools(bool flags)
{
    bool islock = !flags;
    fileToolBar->setMovable(islock);
    editToolBar->setMovable(islock);
    buildToolBar->setMovable(islock);
    debugToolBar->setMovable(islock);
}

void MainWindow::Options()
{
    if (!optionDialog)
    {
        optionDialog = new OptionDialog(this);
        connect(optionDialog, SIGNAL(ConfigChangedSignal()), this, SLOT(SetTextEdit()));
    }

    optionDialog->show();
}

void MainWindow::AboutIEditor()
{
    QMessageBox::about(this, tr("关于iEditor"), tr("<h1>iEditor v1.1</h1><h4><font color=darkred>This is a simple C/C++ IDE for Qt4.8</font><h4><p>Academic advisor     黄建民</p><p>Guangxi Normal University   2013  韦健</p>"));
}

void MainWindow::ShowPosition()
{
    QTextCursor cursor = currentTextEdit->textCursor();
	int column = cursor.columnNumber();
	int row = cursor.blockNumber();
    QString position(tr("\t第%1行,第%2列").arg(row+1).arg(column+1));
	positionLabel->setText(position);
}

void MainWindow::RevokeAvailable(bool flags)
{
    revokeAction->setEnabled(flags);
}

void MainWindow::RecoverAvailable(bool flags)
{
    recoverAction->setEnabled(flags);
}

void MainWindow::CutAvailable(bool flags)
{
    cutAction->setEnabled(flags);
}

void MainWindow::CopyAvailable(bool flags)
{
    copyAction->setEnabled(flags);
}

void MainWindow::SetEnableAction()
{
    saveAction->setEnabled(false);
    saveAsAction->setEnabled(false);
    closeAction->setEnabled(false);
    revokeAction->setEnabled(false);
    recoverAction->setEnabled(false);
    cutAction->setEnabled(false);
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
    selectAllAction->setEnabled(false);
    findAction->setEnabled(false);
    compileAction->setEnabled(false);
    linkAction->setEnabled(false);
    runAction->setEnabled(false);
    stopRunAction->setEnabled(false);
    beginDebugAction->setEnabled(false);
    singleRunAction->setEnabled(false);
    singleSkinAction->setEnabled(false);
    setBreakPointAction->setEnabled(false);
    deleteBreakPointAction->setEnabled(false);
    stopDebugAction->setEnabled(false);
    wordCountAction->setEnabled(false);
}

void MainWindow::SetContextAvailable(bool flags)
{
    currentTextEdit->commentAction->setEnabled(flags);
    currentTextEdit->unCommentAction->setEnabled(flags);
}

void  MainWindow::SetIsToContinue(const QString &action)
{
    if ("Cancel" == action)
    {
        isToContinue = false;
    } else
    {
        isToContinue = true;
    }
}

void MainWindow::SaveFilesByStringList(QList<QListWidgetItem *> items)
{
    QString filePath;
    QFileInfo fileInfo;
    int index;
    foreach (QListWidgetItem *item, items)
    {
        filePath = item->text();
        fileInfo.setFile(filePath);

        if (fileInfo.isFile())  //the textEdit have a filePath
        {
            index = FindIndexByFilePath(filePath);
            if (-1 != index)
            {
                SaveFile(textEditList[index]->filePath);
            }
        } else                  //is a newTab
        {
            index = FindIndexByFileName(filePath);
            if (-1 != index)
            {
                QString tabPath = QFileDialog::getSaveFileName(this, tr("保存文件到-iEditor"), tr("../%1").arg(filePath), tr("所有文件 (*.*);;iEditor项目(*.ieditor);;C/C++文件(*.c *.C *.cpp *.CPP *.h *.H);;文本文件 (*.txt)"));

                if (!tabPath.isEmpty())
                    SaveFile(tabPath);
            }
        }
    }
}

int MainWindow::FindIndexByFileName(const QString &fileName)
{
    for (int i = 0; i < textEditList.count(); ++i)
    {
        if (textEditList[i]->documentTitle() == fileName)
            return i;
    }
    return -1;
}

int MainWindow::FindIndexByFilePath(const QString &filePath)
{
    for (int i = 0; i < textEditList.count(); ++i)
    {
        if (filePath == textEditList[i]->filePath)
            return i;
    }
    return -1;
}

void MainWindow::SaveFilesByClose()
{
    QStringList fileNameList;

    foreach (TextEdit *te, textEditList)
    {
        if (te->isChange)
        {
            if (te->filePath.isEmpty())
            {
                fileNameList << te->documentTitle();
            } else
            {
                fileNameList << te->filePath;
            }

        }
    }

    if (fileNameList.isEmpty())
        return;

    saveDialog = new SaveDialog(this);
    saveDialog->SetFileNameList(fileNameList);

    connect(saveDialog, SIGNAL(SvaeSignal(QList<QListWidgetItem*>)), this, SLOT(SaveFilesByStringList(QList<QListWidgetItem*>)));
    connect(saveDialog, SIGNAL(NoSaveSignal(QString)), this, SLOT(SetIsToContinue(QString)));
    saveDialog->show();
    saveDialog->exec();
}

void MainWindow::SetCurrentFile(const QString &filePath)
{
    if (!filePath.isEmpty())
    {
        currentFileName = StrippedFileName(filePath);
        setWindowTitle(currentFileName + "-iEditor");
        currentTextEdit->setDocumentTitle(currentFileName);
        tabWidget->setTabText(currentTabIndex, currentFileName);
        recentFiles.removeAll(filePath);
        recentFiles.prepend(filePath);
        UpdateRecentFileActions();
    }
}

//////////////////////// down project management

void MainWindow::CreateProject(const QString &projectFullPath, const QString &projectDir, const QString projectName, int projectType)
{
    QDir dir;
    bool isCProject = false;
    dir.mkdir(projectDir);
    QFile file1(projectFullPath);
    file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream out(&file1);
    out.setVersion(QDataStream::Qt_4_8);
    if (1 == projectType)
        isCProject = true;

    out << magic;
    out << version;
    out << projectName;
    out << isCProject;
    out << headFile;
    out << sourceFile;

    if(1 == projectType)
    {
        out << cFile;
        QFile file2(tr("%1%2").arg(projectDir).arg(cFile));
        file2.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file2.close();
    } else
    {
        out << cppFile;
        QFile file2(tr("%1%2").arg(projectDir).arg(cppFile));
        file2.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file2.close();
    }

    file1.close();

    Open(projectFullPath);

    statusBar()->showMessage(tr("项目已经成功创建!"), 2000);
}

void MainWindow::ReadProject(const QString &projectPath)
{
    QFile file(projectPath);
    QString projectDir = StrippedFileDir(projectPath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        quint32 magicNumber;
        quint32 ver;
        QString projectName;
        bool isCProject;
        QString head;
        QString source;
        QStringList headFileList, sourceFileList;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_8);
        in >> magicNumber;

        if (magicNumber == magic)
        {
            in >> ver;
            if (ver == version)
            {
                in >> projectName;
                in >> isCProject;
                in >> head;
                if (headFile == head)
                {
                    while (!in.atEnd())
                    {
                        in >> head;
                        if (sourceFile != head)
                        {
                            if (QFile::exists(projectDir + head))
                                headFileList << head;
                        } else
                        {
                            break;
                        }
                    }

                    while (!in.atEnd())
                    {
                        in >> source;
                        if (QFile::exists(projectDir + source))
                            sourceFileList << source;
                    }

                    QString fileDir = StrippedFileDir(projectPath);

                    //init currentProject
                    projectTree->NewProject(projectName, fileDir, isCProject);

                    //add Project::projectFolderItem to TreeWidget, just add topItem
                    projectTree->addTopLevelItem(projectTree->GetCurrentProjectTopLevelItem());
                    projectTree->setCurrentItem(projectTree->GetCurrentProjectTopLevelItem());
                    projectTree->expandAll();

                    foreach (const QString &headFileName, headFileList)
                    {
                        projectTree->AddHeadFile(headFileName);
                    }

                    foreach (const QString &sourceFileName, sourceFileList)
                    {
                        projectTree->AddSourceFile(sourceFileName);
                    }
                    projectDock->SetTabCurrentIndex(1);
                }
            }

        } else
        {
            QMessageBox::warning(this, tr("警告-iEditor"), tr("%1不是iEditor的项目文件").arg(projectPath));
        }
    }
    file.close();
}

void MainWindow::CloseProject()
{
    QString projectDir = projectTree->GetCurrentProjectDir();
    int index;
    int childCount = projectTree->GetHeadFileCount();
    QTreeWidgetItem *item = projectTree->GetHeadFolderItem();

    for (int i = 0; i < childCount; ++i)
    {
        index = FindIndexByFilePath(projectDir + item->child(i)->text(0));
        if (-1 != index)
        {
            TabClose(index, false);
        }
    }

    childCount = projectTree->GetSourceFileCount();
    item = projectTree->GetSourceFolderItem();

    for (int i = 0; i < childCount; ++i)
    {
        index = FindIndexByFilePath(projectDir + item->child(i)->text(0));
        if (-1 != index)
        {
            TabClose(index, false);
        }
    }

    WriteProject();

    projectTree->takeTopLevelItem(projectTree->GetCurrentProjectTopLevelItemIndex());
}

void MainWindow::WriteProject()
{
    QString projectName = projectTree->GetCurrentProjectName();
    bool isCProject = projectTree->IsCProject();

    QFile file(projectTree->GetCurrentPojectFullPath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_8);

        out << magic;
        out << version;
        out << projectName;
        out << isCProject;
        out << headFile;

        int childCount = projectTree->GetHeadFileCount();
        QTreeWidgetItem *item = projectTree->GetHeadFolderItem();

        for (int i = 0; i < childCount; ++i)
        {
            out << item->child(i)->text(0);
        }

        childCount = projectTree->GetSourceFileCount();
        item = projectTree->GetSourceFolderItem();

        out << sourceFile;

        for (int i = 0; i < childCount; ++i)
        {
            out << item->child(i)->text(0);
        }
    }
    file.close();
}

void MainWindow::RemoveProjectFile(const QString &filePath)
{
    RemoveDialog *removeDialog = new RemoveDialog(filePath, this);
    connect(removeDialog, SIGNAL(OkButtonClickedSignal(QString,bool)), SLOT(DeleteProjectFile(QString, bool)));
    removeDialog->show();
}

void MainWindow::RenameProjectFile(const QString &originalilePath, const QString &newFilePath)
{
    int index = FindIndexByFilePath(originalilePath);
    if (-1 != index)
    {
        IndexChanged(index);
        currentTextEdit->filePath = newFilePath;
        SetCurrentFile(newFilePath);
        if (currentTextEdit->isChange)
        {
            TextChanged();
        }
    }
}

void MainWindow::DeleteProjectFile(const QString &filePath, bool isDelete)
{
    int index = FindIndexByFilePath(filePath);

    if (-1 != index)
    {
        TabClose(index);
    }

    if (isDelete)
    {
        if (!RemoveFile(filePath))
        {
            QMessageBox::warning(this, tr("删除提示-iEditor"), tr("文件无法从磁盘中删除！"));
            return;
        }
    }

    if (IsHeadFile(filePath))
        projectTree->RemoveHeadFile(projectTree->currentItem());
    else
        projectTree->RemoveSourceFile(projectTree->currentItem());
}

bool MainWindow::RemoveFile(const QString &filePath)
{
    QFile file(filePath);

    if (file.exists())
    {
        return file.remove();
    }

    return true;
}

bool MainWindow::IsHeadFile(const QString &fileName)
{
    return ("h" == fileName.split(".").last());
}

void MainWindow::AddFileToProject(const QString &fileName)
{
    if (IsHeadFile(fileName))
    {
        if (!projectTree->ItemNameExist(fileName))
            projectTree->AddHeadFile(fileName);
        else
            QMessageBox::warning(this, tr("打开提示-iEditor"), tr("文件已经存在项目中！"));
    } else
    {
        if (!projectTree->ItemNameExist(fileName))
            projectTree->AddSourceFile(fileName);
        else
            QMessageBox::warning(this, tr("打开提示-iEditor"), tr("文件已经存在项目中！"));
    }
}

void MainWindow::AddNewFileToProject(const QString &fileDir, bool isCProject)
{
    if (isCProject)
    {
        newWizard = new NewWizard(1, fileDir, this);
    } else
    {
        newWizard = new NewWizard(2, fileDir, this);
    }
    connect(newWizard, SIGNAL(FinishButtonClickedSignal(QString, int)), this, SLOT(CreateNewFile(QString, int)));
    newWizard->show();
    newWizard->exec();
    delete newWizard;
    newWizard = 0;
}

void MainWindow::AddExistingToProject(const QString &fileDir, bool isCProject)
{
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    QString exisingFilePath;

    if (isCProject)
    {
        exisingFilePath = fileDialog->getOpenFileName(this, tr("选择项目的现在文件-iEditor"), fileDir, tr("C文件(*.c *.C *.h *.H)"));
    } else
    {
        exisingFilePath = fileDialog->getOpenFileName(this, tr("选择项目的现在文件-iEditor"), fileDir, tr("C++文件(*.cpp *.CPP *.h *.H)"));
    }
    if (!exisingFilePath.isEmpty())
    {
        if (fileDir != StrippedFileDir(exisingFilePath))
        {
            QMessageBox::warning(this, tr("打开文件提示-iEditor"), tr("该文件与目项不在同一个目录!"));
            return;
        } else
        {
            Open(exisingFilePath);
            AddFileToProject(StrippedFileName(exisingFilePath));
        }
    }
}

////////////////////////// up project management

void MainWindow::SetTextEdit()
{
    foreach (TextEdit *te, textEditList)
    {
        te->SetTextEdit();
    }
}

QString MainWindow::StrippedFileName(const QString &filePath)
{
    return QFileInfo(filePath).fileName();
}

QString MainWindow::StrippedFileDir(const QString &filePath)
{
    return QFileInfo(filePath).absolutePath() + "/";
}

QString MainWindow::StrippedExecName(const QString &filePath)
{
    return QFileInfo(filePath).suffix();
}

void MainWindow::DefineDockWidgetArea(Qt::DockWidgetArea area)
{
    if (0x1 == area)
        projectDockPosition = "Left";
    else
        projectDockPosition = "Right";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    SaveFilesByClose();

    if (!isToContinue)
        event->ignore();
    else
    {
        int projectCount = projectTree->GetProjectCount();

        while (0 != projectCount)   //have to delete, but soft exception exit
        {
            CloseProject();
            --projectCount;
        }


        foreach (TextEdit *te, textEditList)    //have to delete, but soft exception exit
        {
            if (te != NULL)
                delete te;
        }

        ConfigStruct::GetConfig()->recentDir = recentDir;
        ConfigStruct::GetConfig()->WriterConfig();
        WriteMainWindowConfig();

        event->accept();
    }
}

void MainWindow::WriteMainWindowConfig()
{
    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);

    if (!settings.isWritable()) {
        QMessageBox::critical(this, "错误提示-iEditor", "<p><font color=red>iEditor无法保存软件配置，请检查 [config.ini] 是否丢失？</font></p>");
        return;
    }

    settings.beginGroup("MainWindowConfig");
    settings.setValue("mainWindowState", saveState());
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("recentProjectFiles", recentProjectFiles);
    settings.setValue("projectDockPosition", projectDockPosition);
    settings.setValue("isShowProjectDock", projectManageAction->isChecked());
    settings.setValue("isShowOutputDock", outputDockAction->isChecked());
    settings.setValue("isShowFileToolsBar", fileToolsBarAction->isChecked());
    settings.setValue("isShowEditToolsBar", editToolsBarAction->isChecked());
    settings.setValue("isShowBuildToolsBar", buildToolsBarAction->isChecked());
    settings.setValue("isShowDebugToolsBar", debugToolsBarAction->isChecked());
    settings.setValue("isShowStatusBar", statusBarAction->isChecked());
    settings.setValue("isLockTools", lockToolsAction->isChecked());
    settings.endGroup();
}

void MainWindow::ReadMainWindowConfig()
{
    QSettings settings("../iEditor/config/config.ini", QSettings::IniFormat);

    settings.beginGroup("MainWindowConfig");
    restoreState(settings.value("mainWindowState").toByteArray());
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    recentFiles = settings.value("recentFiles").toStringList();
    UpdateRecentFileActions();
    recentProjectFiles = settings.value("recentProjectFiles").toStringList();
    UpdateRecentProjectFileActions();
    recentDir = ConfigStruct::GetConfig()->recentDir;
    projectDock->setVisible(settings.value("isShowProjectDock").toBool());
    outputDock->setVisible(settings.value("isShowOutputDock").toBool());
    fileToolBar->setVisible(settings.value("isShowFileToolsBar").toBool());
    editToolBar->setVisible(settings.value("isShowEditToolsBar").toBool());
    buildToolBar->setVisible(settings.value("isShowBuildToolsBar").toBool());
    debugToolBar->setVisible(settings.value("isShowDebugToolsBar").toBool());
    statusBarAction->setChecked(settings.value("isShowStatusBar").toBool());
    statusBar()->setVisible(settings.value("isShowStatusBar").toBool());
    lockToolsAction->setChecked(settings.value("isLockTools").toBool());
    settings.endGroup();
}
