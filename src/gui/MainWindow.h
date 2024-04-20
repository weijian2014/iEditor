/****************************************************************************
**
**      Datetime:   2013年03月21日 星期四 15时43分13秒
**      Author  :   韦   健
**      Titles  :   主窗体类，派生于QMainWindow,ieditor主要的类
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_MAINWINDOW_H_
#define _IEDITOR_MAINWINDOW_H_

#include "FileExplorer.h"

#include <QMainWindow>
#include <QTextCursor>
#include <QTextDocument>
#include <QListWidgetItem>
#include <QList>
#include <QProcess>

class Config;
class FontAwesome;
class QAction;
class QMenu;
class QString;
class QLabel;
class TextEditTabBar;
class FindDialog;
class OptionDialog;
class SaveDialog;
class SideBar;
class Panel;

class MainWindow final : public QMainWindow {
   Q_OBJECT
public:
   explicit MainWindow(QWidget* parent = nullptr);
   MainWindow(const MainWindow&) = delete;
   MainWindow& operator=(const MainWindow&) = delete;
   MainWindow(MainWindow&&) = delete;
   ~MainWindow() {}
   void setNewTextEdit();
   void disableEditActions();

protected:
   void closeEvent(QCloseEvent* event);

public slots:
   void showPositionSlot();

private slots:
   void findSlot();
   void revokeAvailableSlot(bool flag);
   void recoverAvailableSlot(bool flag);
   void cutAvailableSlot(bool flag);
   void copyAvailableSlot(bool flag);
   void findForwardSlot();
   void findBackwardSlot();
   void replaceSlot();
   void replaceAllSlot();
   void setIsToContinueSlot(const QString& action);
   void saveFilesByStringListSlot(QList<QListWidgetItem*> items);

private:
   void createAction();
   void createMenu();
   void createToolBar();
   void createStatusBar();
   void initSideBar();
   void initPanel();
   void updateRecentFileActions();

   void createTextEditTabWidget();
   void setEnableAction();
   bool saveFile(const QString& filePath);
   void setCurrentFile(const QString& filePath);
   void findString(const QTextCursor& cursor,
                   QTextDocument::FindFlags findFlag);
   void findRegExp(const QTextCursor& cursor,
                   QTextDocument::FindFlags findFlag);
   void saveMainWindowConfig();
   void loadMainWindowConfig();
   void saveFilesByClose();

private:
   enum { MaxTabs = 20 };
   enum { MaxRecentFiles = 8 };

   Config* config_;
   FontAwesome* fontAwesome_;

   QAction* newAction_;
   QAction* openAction_;
   QAction* openDirAction_;
   QAction* saveAction_;
   QAction* saveAsAction_;
   QAction* recentFileAction_[MaxRecentFiles];
   QAction* closeAction_;
   QAction* exitAction_;

   QAction* revokeAction_;
   QAction* recoverAction_;
   QAction* cutAction_;
   QAction* copyAction_;
   QAction* pasteAction_;
   QAction* selectAllAction_;
   QAction* findAction_;

   QAction* sideBarAction_;
   QAction* panelAction_;
   QAction* fileToolsBarAction_;
   QAction* editToolsBarAction_;
   QAction* statusBarAction_;

   QAction* wordCountAction_;
   QAction* fullScreenAction_;
   QAction* lockToolsAction_;
   QAction* optionsAction_;

   QAction* aboutQtAction_;
   QAction* aboutAction_;

   QMenu* fileMenu_;
   QMenu* recentFilesMenu_;
   QMenu* editMenu_;
   QMenu* windowMenu_;
   QMenu* toolsMenu_;
   QMenu* helpMenu_;

   QToolBar* fileToolBar_;
   QToolBar* editToolBar_;

   QLabel* positionLabel_;  // cusor positoin

   TextEditTabBar* textEditTabBar_;
   FindDialog* findDialog_;
   OptionDialog* optionDialog_;
   SaveDialog* saveDialog_;
   SideBar* sideBar_;
   Panel* panel_;

   FileExplorer* fileExplorer_;

   QStringList recentFiles_;
   QProcess process_;
};  // MainWindow

#endif  // _IEDITOR_MAINWINDOW_H_
