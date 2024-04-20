/****************************************************************************
**
**      Datetime:   2013年03月21日 星期四 15时43分13秒
**      Author  :   韦   健
**      Titles  :   主窗体类，派生于QMainWindow,ieditor主要的类
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#include "MainWindow.h"
#include "Define.h"
#include "TextEditTabBar.h"
#include "TextEdit.h"
#include "FindDialog.h"
#include "OptionDialog.h"
#include "SaveDialog.h"
#include "SideBar.h"
#include "Panel.h"
#include "Config.h"
#include "FontAwesome.h"
#include "qaction.h"
#include "qdockwidget.h"

#include <QCoreApplication>
#include <QtGui>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      config_(common::Singleton<Config>::instancePointer()),
      fontAwesome_(common::Singleton<FontAwesome>::instancePointer()),
      newAction_(nullptr),
      openAction_(nullptr),
      openDirAction_(nullptr),
      saveAction_(nullptr),
      saveAsAction_(nullptr),
      closeAction_(nullptr),
      exitAction_(nullptr),
      revokeAction_(nullptr),
      recoverAction_(nullptr),
      cutAction_(nullptr),
      copyAction_(nullptr),
      pasteAction_(nullptr),
      selectAllAction_(nullptr),
      findAction_(nullptr),
      sideBarAction_(nullptr),
      panelAction_(nullptr),
      fileToolsBarAction_(nullptr),
      editToolsBarAction_(nullptr),
      statusBarAction_(nullptr),
      wordCountAction_(nullptr),
      fullScreenAction_(nullptr),
      lockToolsAction_(nullptr),
      optionsAction_(nullptr),
      aboutQtAction_(nullptr),
      aboutAction_(nullptr),
      fileMenu_(nullptr),
      recentFilesMenu_(nullptr),
      editMenu_(nullptr),
      windowMenu_(nullptr),
      toolsMenu_(nullptr),
      helpMenu_(nullptr),
      fileToolBar_(nullptr),
      editToolBar_(nullptr),
      positionLabel_(nullptr),
      textEditTabBar_(nullptr),
      findDialog_(nullptr),
      optionDialog_(nullptr),
      saveDialog_(nullptr),
      sideBar_(nullptr),
      panel_(nullptr),
      fileExplorer_(nullptr),
      recentFiles_() {
   setObjectName("MainWindow");
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
   Q_ASSERT(fontAwesome_);
   Q_ASSERT(fontAwesome_->isInit());
   std::memset(recentFileAction_, 0, sizeof(QAction*) * MaxRecentFiles);

   // 停靠角落的分配
   setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
   setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

   setWindowIcon(QIcon(":/images/ieditor.png"));
   setWindowTitle(QString("%1 %2")
                      .arg(QCoreApplication::applicationName())
                      .arg(QCoreApplication::applicationVersion()));

   createAction();
   createMenu();
   createToolBar();
   createStatusBar();
   createTextEditTabWidget();
   initSideBar();
   initPanel();
   loadMainWindowConfig();

   // open recentFile and recentProject
   for (int i = 0; i < MaxRecentFiles; ++i) {
      if (recentFileAction_[i]->isVisible()) {
         textEditTabBar_->addTextEditTab(
             recentFileAction_[i]->data().toString());
         break;
      }
   }

   process_.setReadChannel(QProcess::ProcessChannel::StandardOutput);
   // setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::createAction() {
   newAction_ =
       new QAction(QIcon(":/images/new.png"), tr("新建文件(&N)"), this);
   newAction_->setShortcut(QKeySequence::New);
   newAction_->setToolTip(tr("新建文件(Ctrl+N)"));
   connect(newAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->addTextEditTab(UntitledNameC); });

   openAction_ =
       new QAction(QIcon(":/images/open.png"), tr("打开文件(&O)"), this);
   openAction_->setShortcut(QKeySequence::Open);
   openAction_->setToolTip(tr("打开文件(Ctrl+O)"));
   connect(openAction_, &QAction::triggered, this, [this]() {
      QString file = QFileDialog::getOpenFileName(this, tr("打开文件"),
                                                  config_->recentDir_);

      if (file.isEmpty()) {
         return;
      }

      if (textEditTabBar_->addTextEditTab(file)) {
         recentFiles_ << file;
      }
   });

   openDirAction_ =
       new QAction(QIcon(":/images/open.png"), tr("打开文件夹(&O)"), this);
   openDirAction_->setShortcut(QKeySequence("Ctrl+Shift+O"));
   openDirAction_->setToolTip(tr("打开文件夹(Ctrl+Shift+O)"));
   connect(openDirAction_, &QAction::triggered, this, [this]() {
      QFileDialog* fileDialog = new QFileDialog(this);
      fileDialog->setFileMode(QFileDialog::AnyFile);
      QString dir = fileDialog->getExistingDirectory(this, tr("打开文件夹"),
                                                     config_->recentDir_);

      if (dir.isEmpty()) {
         return;
      }

      fileExplorer_->loading(dir);
   });

   saveAction_ = new QAction(QIcon(":/images/save.png"), tr("保存"), this);
   saveAction_->setObjectName(ObjectNameC_MainWindowSaveAction);
   saveAction_->setShortcut(QKeySequence::Save);
   saveAction_->setToolTip(tr("保存(Ctrl+S)"));

   saveAsAction_ =
       new QAction(QIcon(":/images/saveAs.png"), tr("另存为"), this);
   saveAsAction_->setShortcut(QKeySequence::SaveAs);
   saveAsAction_->setToolTip(tr("另存为(Ctrl+Shift+S)"));
   connect(saveAsAction_, &QAction::triggered, this, [this]() {
      TextEdit* te = textEditTabBar_->currentTextEdit();
      QString saveAsFilePath = QFileDialog::getSaveFileName(
          this, tr("文件另存为"),
          QString("%1/%2").arg(config_->recentDir_).arg(te->fileName()));

      if (!saveAsFilePath.isEmpty()) {
         config_->recentDir_ = QFileInfo(saveAsFilePath).absolutePath() + "/";
         if (te->writeFile(saveAsFilePath)) {
            statusBar()->showMessage(tr("文件成功保存至%1").arg(saveAsFilePath),
                                     2000);
            te->getPropertyAction()->setEnabled(true);
            te->setFilePath(saveAsFilePath);
            setCurrentFile(saveAsFilePath);
            saveAction_->setEnabled(false);
         } else {
            statusBar()->showMessage(tr("文件未保存！"), 2000);
         }
      }
   });

   for (int i = 0; i < MaxRecentFiles; ++i) {
      recentFileAction_[i] = new QAction(this);
      recentFileAction_[i]->setVisible(false);
      connect(recentFileAction_[i], &QAction::triggered, this, [this]() {
         QAction* action = qobject_cast<QAction*>(sender());
         if (action) {
            textEditTabBar_->addTextEditTab(action->data().toString());
         }
      });
   }

   closeAction_ =
       new QAction(QIcon(":/images/close.png"), tr("关闭文件"), this);
   closeAction_->setObjectName(ObjectNameC_MainWindowCloseAction);
   closeAction_->setShortcut(QKeySequence::Close);
   closeAction_->setToolTip(tr("关闭文件(Ctrl+W)"));
   connect(closeAction_, &QAction::triggered, this, [this]() {
      textEditTabBar_->tabClosedSlot(textEditTabBar_->currentIndex());
   });

   exitAction_ = new QAction(QIcon(":/images/exit.png"), tr("退出"), this);
   exitAction_->setShortcut(QKeySequence("Alt+F4"));
   exitAction_->setToolTip(tr("退出(Alt+F4)"));
   connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));

   revokeAction_ = new QAction(QIcon(":/images/revoke.png"), tr("撤消"), this);
   revokeAction_->setShortcut(QKeySequence::Undo);
   revokeAction_->setToolTip(tr("撤消(Ctrl+Z)"));
   connect(revokeAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->undo(); });

   recoverAction_ =
       new QAction(QIcon(":/images/recover.png"), tr("恢复"), this);
   recoverAction_->setShortcut(QKeySequence::Redo);
   recoverAction_->setToolTip(tr("恢复(Ctrl+Shift+Z)"));
   connect(recoverAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->redo(); });

   cutAction_ = new QAction(QIcon(":/images/cut.png"), tr("剪切"), this);
   cutAction_->setShortcut(QKeySequence::Cut);
   cutAction_->setToolTip(tr("剪切(Ctrl+X)"));
   connect(cutAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->cut(); });

   copyAction_ = new QAction(QIcon(":/images/copy.png"), tr("复制"), this);
   copyAction_->setShortcut(QKeySequence::Copy);
   copyAction_->setToolTip(tr("复制(Ctrl+C)"));
   connect(copyAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->copy(); });

   pasteAction_ = new QAction(QIcon(":/images/paste.png"), tr("粘贴"), this);
   pasteAction_->setObjectName(ObjectNameC_MainWindowPasteAction);
   pasteAction_->setShortcut(QKeySequence::Paste);
   pasteAction_->setToolTip(tr("粘贴(Ctrl+V)"));
   connect(pasteAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->paste(); });

   selectAllAction_ =
       new QAction(QIcon(":/images/selectAll.png"), tr("全选"), this);
   selectAllAction_->setObjectName(ObjectNameC_MainWindowSelectAllAction);
   selectAllAction_->setShortcut(QKeySequence::SelectAll);
   selectAllAction_->setToolTip(tr("全选(Ctrl+A)"));
   connect(selectAllAction_, &QAction::triggered, this,
           [this]() { textEditTabBar_->currentTextEdit()->selectAll(); });

   findAction_ = new QAction(QIcon(":/images/find.png"), tr("查找|替换"), this);
   findAction_->setObjectName(ObjectNameC_MainWindowFindAction);
   findAction_->setShortcut(QKeySequence::Find);
   findAction_->setToolTip(tr("查找|替换(Ctrl+F)"));
   connect(findAction_, SIGNAL(triggered()), this, SLOT(findSlot()));

   sideBarAction_ = new QAction(tr("侧边栏(&P)"), this);
   sideBarAction_->setShortcut(QKeySequence("Ctrl+P"));
   sideBarAction_->setCheckable(true);
   sideBarAction_->setToolTip(tr("切换侧边栏(Ctrl+P)"));

   panelAction_ = new QAction(tr("面板(&M)"), this);
   panelAction_->setShortcut(QKeySequence("Ctrl+M"));
   panelAction_->setCheckable(true);
   panelAction_->setToolTip(tr("切换面板(Ctrl+M)"));

   fileToolsBarAction_ = new QAction(tr("文件栏(&F)"), this);
   fileToolsBarAction_->setShortcut(QKeySequence("Ctrl+Shift+F"));
   fileToolsBarAction_->setCheckable(true);
   fileToolsBarAction_->setToolTip(tr("切换文件栏(Ctrl+Shift+F)"));

   editToolsBarAction_ = new QAction(tr("编辑栏(&E)"), this);
   editToolsBarAction_->setShortcut(QKeySequence("Ctrl+Shift+E"));
   editToolsBarAction_->setCheckable(true);
   editToolsBarAction_->setToolTip(tr("切换编辑栏(Ctrl+Shift+E)"));

   statusBarAction_ = new QAction(tr("状态栏(&S)"), this);
   statusBarAction_->setShortcut(QKeySequence("Ctrl+Shift+K"));
   statusBarAction_->setCheckable(true);
   statusBarAction_->setToolTip(tr("切换状态栏(Ctrl+Shift+K)"));

   wordCountAction_ =
       new QAction(QIcon(":/images/wordCount.png"), tr("字数统计"), this);
   wordCountAction_->setShortcut(QKeySequence("Ctrl+Shift+W"));
   wordCountAction_->setToolTip(tr("字数统计(Ctrl+Shift+W)"));
   connect(wordCountAction_, &QAction::triggered, this, [this]() {
      // clang-format off
      QMessageBox::information(
          this,
          tr("字数统计"),
          tr("<h4>数字:  %1 字</h4><h4>块数: %2 块</h4><h4>行数: %3 行</h4>")
              .arg(textEditTabBar_->currentTextEdit()->document()->characterCount())
              .arg(textEditTabBar_->currentTextEdit()->document()->blockCount())
              .arg(textEditTabBar_->currentTextEdit()->document()->lineCount()));
      // clang-format on
   });

   lockToolsAction_ =
       new QAction(QIcon(":/images/lockTools.png"), tr("锁定工具栏(&L)"), this);
   lockToolsAction_->setShortcut(QKeySequence("Ctrl+L"));
   lockToolsAction_->setCheckable(true);
   lockToolsAction_->setToolTip(tr("锁定工具栏(Ctrl+L)"));
   connect(lockToolsAction_, &QAction::toggled, this, [this](bool isLock) {
      fileToolBar_->setMovable(!isLock);
      editToolBar_->setMovable(!isLock);
   });

   fullScreenAction_ =
       new QAction(QIcon(":/images/fullScreen.png"), tr("全屏显示(&F)"), this);
   fullScreenAction_->setShortcut(Qt::Key_F11);
   fullScreenAction_->setCheckable(true);
   fullScreenAction_->setToolTip(tr("全屏显示(F11)"));
   connect(fullScreenAction_, &QAction::toggled, this,
           [this](bool isFull) { isFull ? showFullScreen() : showNormal(); });

   optionsAction_ =
       new QAction(QIcon(":/images/options.png"), tr("首选项(&O)"), this);
   optionsAction_->setShortcut(Qt::Key_F5);
   optionsAction_->setToolTip(tr("首选项(F5)"));
   connect(optionsAction_, &QAction::triggered, this, [this]() {
      if (!optionDialog_) {
         optionDialog_ = new OptionDialog(this);
         connect(optionDialog_, &OptionDialog::configChangedSignal, this,
                 [this]() {
                    config_->createHighlightingRules();
                    fileExplorer_->settingsChanged();
                    textEditTabBar_->settingsChanged();
                    panel_->settingsChanged();
                 });
      }

      optionDialog_->show();
   });

   aboutQtAction_ = new QAction(QIcon(":/images/qt.png"), tr("关于QT"), this);
   aboutQtAction_->setToolTip(tr("关于QT"));
   connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

   aboutAction_ =
       new QAction(QIcon(":/images/ieditor.png"),
                   tr("关于%1").arg(QCoreApplication::applicationName()), this);
   aboutAction_->setToolTip(
       tr("关于%1").arg(QCoreApplication::applicationName()));
   connect(aboutAction_, &QAction::triggered, this, [this]() {
      QMessageBox::about(
          this, tr("关于%1").arg(QCoreApplication::applicationName()),
          QString(
              tr("<h1>ieditor %1</h1><h4><font color=darkred>A simple C/C++ "
                 "IDE base on QT %2</font><h4><p>Academic advisor "
                 "黄建民教授</p><p>Guangxi Normal University © 2013 韦健</p>"))
              .arg(QCoreApplication::applicationVersion())
              .arg(qVersion()));
   });

   // 开始时禁止要禁止的菜单项
   setEnableAction();
}

void MainWindow::createMenu() {
   fileMenu_ = menuBar()->addMenu(tr("文件(&F)"));
   fileMenu_->addAction(newAction_);
   fileMenu_->addAction(openAction_);
   fileMenu_->addAction(openDirAction_);
   fileMenu_->addAction(saveAction_);
   fileMenu_->addAction(saveAsAction_);

   fileMenu_->addSeparator();
   recentFilesMenu_ = fileMenu_->addMenu(tr("最近打开的文件..."));
   for (int i = 0; i < MaxRecentFiles; ++i) {
      recentFilesMenu_->addAction(recentFileAction_[i]);
   }
   fileMenu_->addSeparator();

   fileMenu_->addAction(closeAction_);
   fileMenu_->addAction(exitAction_);

   editMenu_ = menuBar()->addMenu(tr("编辑(&E)"));
   editMenu_->setObjectName(ObjectNameC_MainWindowEditMenu);
   editMenu_->addAction(revokeAction_);
   editMenu_->addAction(recoverAction_);
   editMenu_->addSeparator();
   editMenu_->addAction(cutAction_);
   editMenu_->addAction(copyAction_);
   editMenu_->addAction(pasteAction_);
   editMenu_->addSeparator();
   editMenu_->addAction(selectAllAction_);
   editMenu_->addSeparator();
   editMenu_->addAction(findAction_);

   windowMenu_ = menuBar()->addMenu(tr("窗口(&W)"));
   windowMenu_->addAction(fileToolsBarAction_);
   windowMenu_->addAction(editToolsBarAction_);
   windowMenu_->addAction(statusBarAction_);
   windowMenu_->addSeparator();
   windowMenu_->addAction(sideBarAction_);
   windowMenu_->addAction(panelAction_);

   toolsMenu_ = menuBar()->addMenu(tr("工具(&T)"));
   toolsMenu_->addAction(wordCountAction_);
   toolsMenu_->addSeparator();
   toolsMenu_->addAction(fullScreenAction_);
   toolsMenu_->addSeparator();
   toolsMenu_->addAction(lockToolsAction_);
   toolsMenu_->addSeparator();
   toolsMenu_->addAction(optionsAction_);

   helpMenu_ = menuBar()->addMenu(tr("帮助(&H)"));
   helpMenu_->addAction(aboutQtAction_);
   helpMenu_->addAction(aboutAction_);
}

void MainWindow::createToolBar() {
   fileToolBar_ = addToolBar(tr("文件工具栏"));
   fileToolBar_->setObjectName("fileToolBar");
   fileToolBar_->addAction(newAction_);
   fileToolBar_->addAction(openAction_);
   fileToolBar_->addAction(openDirAction_);
   fileToolBar_->addAction(saveAction_);
   fileToolBar_->addSeparator();
   fileToolBar_->addAction(closeAction_);

   editToolBar_ = addToolBar(tr("编辑工具栏"));
   editToolBar_->setObjectName("editToolBar");
   editToolBar_->addAction(revokeAction_);
   editToolBar_->addAction(recoverAction_);
   editToolBar_->addSeparator();
   editToolBar_->addAction(cutAction_);
   editToolBar_->addAction(copyAction_);
   editToolBar_->addAction(pasteAction_);
   editToolBar_->addSeparator();
   editToolBar_->addAction(findAction_);

   connect(fileToolsBarAction_, SIGNAL(toggled(bool)), fileToolBar_,
           SLOT(setVisible(bool)));
   connect(fileToolBar_, SIGNAL(visibilityChanged(bool)), fileToolsBarAction_,
           SLOT(setChecked(bool)));
   connect(editToolsBarAction_, SIGNAL(toggled(bool)), editToolBar_,
           SLOT(setVisible(bool)));
   connect(editToolBar_, SIGNAL(visibilityChanged(bool)), editToolsBarAction_,
           SLOT(setChecked(bool)));
}

void MainWindow::createStatusBar() {
   positionLabel_ = new QLabel(tr("光标位置"), this);
   positionLabel_->setAlignment(Qt::AlignHCenter);
   positionLabel_->setMinimumSize(positionLabel_->sizeHint());
   positionLabel_->setFrameShadow(QFrame::Sunken);
   positionLabel_->setFrameShape(QFrame::Panel);
   statusBar()->addPermanentWidget(positionLabel_);
   connect(statusBarAction_, SIGNAL(toggled(bool)), statusBar(),
           SLOT(setVisible(bool)));
}

void MainWindow::createTextEditTabWidget() {
   textEditTabBar_ = new TextEditTabBar(this);
   setCentralWidget(textEditTabBar_);
   connect(saveAction_, SIGNAL(triggered()), textEditTabBar_, SLOT(saveSlot()));
}

void MainWindow::initSideBar() {
   sideBar_ = new SideBar(this);
   addDockWidget(config_->sideBarPosition_, sideBar_);

   connect(sideBarAction_, SIGNAL(toggled(bool)), sideBar_,
           SLOT(setVisible(bool)));
   connect(sideBar_, SIGNAL(visibilityChanged(bool)), sideBarAction_,
           SLOT(setChecked(bool)));
   connect(sideBar_, &QDockWidget::dockLocationChanged, this,
           [this](Qt::DockWidgetArea area) {
              config_->sideBarPosition_ = area;
              if (area == Qt::DockWidgetArea::LeftDockWidgetArea) {
                 sideBar_->setTabPosition(QTabWidget::TabPosition::West);
              } else if (area == Qt::DockWidgetArea::RightDockWidgetArea) {
                 sideBar_->setTabPosition(QTabWidget::TabPosition::East);
              }
           });

   emit sideBar_->dockLocationChanged(config_->sideBarPosition_);
   fileExplorer_ = new FileExplorer(this);
   sideBar_->addTabWidget(fileExplorer_, tr("文件浏览器"));

   Q_ASSERT(textEditTabBar_);

   // 侧边栏-文件浏览器的文件被选中时，在编辑区打开它
   connect(fileExplorer_, &QTreeView::clicked, this,
           [this](const QModelIndex& index) {
              QString file = fileExplorer_->selectedFullFileName(index);
              if (file.isEmpty()) {
                 return;
              }

              textEditTabBar_->addTextEditTab(file);
           });

   // 编辑区-标签面切换时，在侧边栏选中
   connect(textEditTabBar_, SIGNAL(currentIndexChangedSignal(const QString&)),
           fileExplorer_, SLOT(selectItem(const QString&)));
}

void MainWindow::initPanel() {
   Q_ASSERT(panelAction_);
   panel_ = new Panel(this);
   addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, panel_);
   connect(panelAction_, SIGNAL(toggled(bool)), panel_, SLOT(setVisible(bool)));
   connect(panel_, SIGNAL(visibilityChanged(bool)), panelAction_,
           SLOT(setChecked(bool)));
}

//////////////////////////////  up is mainwindow setup

void MainWindow::updateRecentFileActions() {
   QMutableStringListIterator iter(recentFiles_);

   while (iter.hasNext()) {
      if (!QFile::exists(iter.next())) {
         iter.remove();
      }
   }

   for (int i = 0; i < MaxRecentFiles; ++i) {
      if (i < recentFiles_.count()) {
         recentFileAction_[i]->setText(
             QString("&%1 %2").arg(i + 1).arg(recentFiles_[i]));
         recentFileAction_[i]->setData(recentFiles_[i]);
         recentFileAction_[i]->setVisible(true);
      } else {
         recentFileAction_[i]->setVisible(false);
      }
   }
}

void MainWindow::setNewTextEdit() {
   // 新建时设置可用的菜单
   closeAction_->setEnabled(true);
   findAction_->setEnabled(true);
   saveAsAction_->setEnabled(true);
   pasteAction_->setEnabled(true);
   selectAllAction_->setEnabled(true);
   wordCountAction_->setEnabled(true);
   textEditTabBar_->currentTextEdit()->getPropertyAction()->setEnabled(false);
}

void MainWindow::disableEditActions() {
   QList<QAction*> actions = editMenu_->actions();
   for (QAction* action : actions) {
      if (action) {
         action->setDisabled(true);
      }
   }
}

bool MainWindow::saveFile(const QString& filePath) {
   return textEditTabBar_->currentTextEdit()->writeFile(filePath);
}

//////////////////////////////////// donw is main slot

void MainWindow::findSlot() {
   if (!findDialog_) {
      findDialog_ = new FindDialog(this);
      connect(findDialog_, SIGNAL(findForwardSignal()), this,
              SLOT(findForwardSlot()));
      connect(findDialog_, SIGNAL(findBackwardSignal()), this,
              SLOT(findBackwardSlot()));
      connect(findDialog_, SIGNAL(replaceSignal()), this, SLOT(replaceSlot()));
      connect(findDialog_, SIGNAL(replaceAllSignal()), this,
              SLOT(replaceAllSlot()));
   }

   findDialog_->show();
}

void MainWindow::findString(const QTextCursor& cursor,
                            QTextDocument::FindFlags findFlag) {
   QTextCursor findCursorResult =
       textEditTabBar_->currentTextEdit()->document()->find(
           findDialog_->findLineEdit_->text(), cursor, findFlag);

   if (findCursorResult.isNull()) {
      QApplication::beep();
      statusBar()->showMessage(
          tr("查找到文件%1, 请反向查找!")
              .arg(findFlag & QTextDocument::FindBackward ? tr("开头")
                                                          : "结尾"),
          1500);
   } else {
      textEditTabBar_->currentTextEdit()->setTextCursor(findCursorResult);
      // 查找到后高亮显示
      QPalette palette = textEditTabBar_->currentTextEdit()->palette();
      palette.setColor(QPalette::Highlight,
                       palette.color(QPalette::Active, QPalette::Highlight));
      textEditTabBar_->currentTextEdit()->setPalette(palette);
   }
}

void MainWindow::findRegExp(const QTextCursor& cursor,
                            QTextDocument::FindFlags findFlag) {
   QTextCursor findCursorResult =
       textEditTabBar_->currentTextEdit()->document()->find(
           QRegularExpression(findDialog_->findLineEdit_->text()), cursor,
           findFlag);

   if (findCursorResult.isNull()) {
      QApplication::beep();
      statusBar()->showMessage(tr("查找到文件结尾，请反向查找！"), 1500);
   } else {
      textEditTabBar_->currentTextEdit()->setTextCursor(findCursorResult);
      // 查找到后高亮显示
      QPalette palette = textEditTabBar_->currentTextEdit()->palette();
      palette.setColor(QPalette::Highlight,
                       palette.color(QPalette::Active, QPalette::Highlight));
      textEditTabBar_->currentTextEdit()->setPalette(palette);
   }
}

void MainWindow::findForwardSlot() {
   QTextCursor findBeginCursor =
       textEditTabBar_->currentTextEdit()->textCursor();
   const unsigned findFlagsFromSettings = findDialog_->getFindFlags();
   if (findFlagsFromSettings & MyFindFlag::DocumentBegin) {
      findBeginCursor.movePosition(QTextCursor::MoveOperation::Start);
   }

   // 经测试，正好相反才符合查找逻辑
   QTextDocument::FindFlags flagForFind(QTextDocument::FindBackward);
   if (findFlagsFromSettings & MyFindFlag::FindCaseSensitively) {
      flagForFind |= QTextDocument::FindCaseSensitively;
   }
   if (findFlagsFromSettings & MyFindFlag::FindWholeWords) {
      flagForFind |= QTextDocument::FindWholeWords;
   }

   if (findFlagsFromSettings & MyFindFlag::UsingRegularExpression) {
      // 使用正则表达式
      findRegExp(findBeginCursor, flagForFind);
   } else {
      findString(findBeginCursor, flagForFind);
   }
}

void MainWindow::findBackwardSlot() {
   QTextCursor findBeginCursor =
       textEditTabBar_->currentTextEdit()->textCursor();
   const unsigned findFlagsFromSettings = findDialog_->getFindFlags();
   if (findFlagsFromSettings & MyFindFlag::DocumentBegin) {
      findBeginCursor.movePosition(QTextCursor::MoveOperation::Start);
   }

   // 经测试，正好相反才符合查找逻辑
   QTextDocument::FindFlags flagForFind;
   if (findFlagsFromSettings & MyFindFlag::FindCaseSensitively) {
      flagForFind |= QTextDocument::FindCaseSensitively;
   }
   if (findFlagsFromSettings & MyFindFlag::FindWholeWords) {
      flagForFind |= QTextDocument::FindWholeWords;
   }

   if (findFlagsFromSettings & MyFindFlag::UsingRegularExpression) {
      // 使用正则表达式
      findRegExp(findBeginCursor, flagForFind);
   } else {
      findString(findBeginCursor, flagForFind);
   }
}

void MainWindow::replaceSlot() {
   if (textEditTabBar_->currentTextEdit()->textCursor().hasSelection())
      textEditTabBar_->currentTextEdit()->insertPlainText(
          findDialog_->replaceLineEdit_->text());
}

void MainWindow::replaceAllSlot() {
   QTextCursor textCursor =
       QTextCursor(textEditTabBar_->currentTextEdit()->document());
   while (!textCursor.isNull() && !textCursor.atEnd()) {
      textCursor = textEditTabBar_->currentTextEdit()->document()->find(
          findDialog_->findLineEdit_->text(), textCursor,
          QTextDocument::FindCaseSensitively);
      textEditTabBar_->currentTextEdit()->setTextCursor(textCursor);
      textEditTabBar_->currentTextEdit()->insertPlainText(
          findDialog_->replaceLineEdit_->text());
   }
}

void MainWindow::showPositionSlot() {
   QTextCursor cursor = textEditTabBar_->currentTextEdit()->textCursor();
   int column = cursor.columnNumber();
   int row = cursor.blockNumber();
   if (cursor.hasSelection()) {
      // 选中的字符数
      int chars = cursor.selectionEnd() - cursor.selectionStart();
      positionLabel_->setText(
          tr("\t行%1, 列%2(已选择%3)").arg(row + 1).arg(column + 1).arg(chars));
   } else {
      positionLabel_->setText(tr("\t行%1, 列%2").arg(row + 1).arg(column + 1));
   }
}

void MainWindow::revokeAvailableSlot(bool flag) {
   revokeAction_->setEnabled(flag);
}

void MainWindow::recoverAvailableSlot(bool flag) {
   recoverAction_->setEnabled(flag);
}

void MainWindow::cutAvailableSlot(bool flag) { cutAction_->setEnabled(flag); }

void MainWindow::copyAvailableSlot(bool flag) { copyAction_->setEnabled(flag); }

void MainWindow::setEnableAction() {
   saveAction_->setEnabled(false);
   saveAsAction_->setEnabled(false);
   closeAction_->setEnabled(false);
   revokeAction_->setEnabled(false);
   recoverAction_->setEnabled(false);
   cutAction_->setEnabled(false);
   copyAction_->setEnabled(false);
   pasteAction_->setEnabled(false);
   selectAllAction_->setEnabled(false);
   findAction_->setEnabled(false);
   wordCountAction_->setEnabled(false);
}

void MainWindow::setIsToContinueSlot(const QString& action) {
   // if ("Cancel" == action) {
   //    isToContinue_ = false;
   // } else {
   //    isToContinue_ = true;
   // }
}

void MainWindow::saveFilesByStringListSlot(QList<QListWidgetItem*> items) {}

void MainWindow::saveFilesByClose() {
   QStringList fileNameList;

   // foreach (TextEdit* te, textEditList_) {
   //    if (te->isChanged()) {
   //       if (te->getFilePath().isEmpty()) {
   //          fileNameList << te->documentTitle();
   //       } else {
   //          fileNameList << te->getFilePath();
   //       }
   //    }
   // }

   if (fileNameList.isEmpty()) return;

   saveDialog_ = new SaveDialog(this);
   saveDialog_->setFileNameList(fileNameList);

   connect(saveDialog_, SIGNAL(svaeSignal(QList<QListWidgetItem*>)), this,
           SLOT(saveFilesByStringListSlot(QList<QListWidgetItem*>)));
   connect(saveDialog_, SIGNAL(noSaveSignal(QString)), this,
           SLOT(setIsToContinueSlot(QString)));
   saveDialog_->show();
   saveDialog_->exec();
}

void MainWindow::setCurrentFile(const QString& filePath) {
   if (!filePath.isEmpty()) {
      // currentFileName_ = strippedFileName(filePath);
      // setWindowTitle(currentFileName_ + "");
      // currentTextEdit_->setDocumentTitle(currentFileName_);
      // textEditTabBar_->setTabText(currentTabIndex_, currentFileName_);
      recentFiles_.removeAll(filePath);
      recentFiles_.prepend(filePath);
      updateRecentFileActions();
   }
}

void MainWindow::closeEvent(QCloseEvent* event) {
   // saveFilesByClose();
   config_->saveConfig();
   saveMainWindowConfig();
   event->accept();
}

void MainWindow::saveMainWindowConfig() {
   config_->mainWindowState_.assign(saveState());
   config_->mainWindowGeometry_.assign(saveGeometry());
   config_->recentFiles_ = recentFiles_;
   config_->isShowSideBar_ = sideBarAction_->isChecked();
   config_->isShowOutputDock_ = panelAction_->isChecked();
   config_->isShowFileToolsBar_ = fileToolsBarAction_->isChecked();
   config_->isShowEditToolsBar_ = editToolsBarAction_->isChecked();
   config_->isShowStatusBar_ = statusBarAction_->isChecked();
   config_->isLockTools_ = lockToolsAction_->isChecked();
   config_->saveMainWindowConfig();
}

void MainWindow::loadMainWindowConfig() {
   restoreState(config_->mainWindowState_);
   restoreGeometry(config_->mainWindowGeometry_);
   recentFiles_ = config_->recentFiles_;
   updateRecentFileActions();
   sideBar_->setVisible(config_->isShowSideBar_);
   panel_->setVisible(config_->isShowOutputDock_);
   fileToolBar_->setVisible(config_->isShowFileToolsBar_);
   editToolBar_->setVisible(config_->isShowEditToolsBar_);
   statusBarAction_->setChecked(config_->isShowStatusBar_);
   statusBar()->setVisible(config_->isShowStatusBar_);
   lockToolsAction_->setChecked(config_->isLockTools_);
}
