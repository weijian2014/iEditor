/****************************************************************************
**
**      Datetime:   2013年04月20日 星期六 13时11分10秒
**      Author  :   韦   健
**      Titles  : 该类用于多标签使用的部件，对左键双击进行了重载，双击标签页可以
**                  关闭标签，双击该部件的空白区域可以新建一个标签
**
**      Guangxi Normal University 2013
**
****************************************************************************/

#include "TextEditTabBar.h"
#include "Config.h"
#include "Define.h"
#include "TextEdit.h"
#include "MainWindow.h"

#include <QMouseEvent>
#include <QTabBar>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>

static const uint64_t MaxNewTabCount(4096);

TextEditTabBar::TextEditTabBar(QWidget* parent)
    : QTabWidget(parent),
      config_(common::Singleton<Config>::instancePointer()),
      mainWindow_(nullptr) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
   setTabShape(QTabWidget::Rounded);  // Rounded | Triangular
   setTabsClosable(true);
   tabBar()->installEventFilter(this);
   setObjectName("textEditTab");

   mainWindow_ = static_cast<MainWindow*>(parentWidget());

   // 与主窗体的联动
   connect(this, SIGNAL(currentChanged(int)), this,
           SLOT(currentIndexChangedSlot(int)));
   connect(this, SIGNAL(tabCloseRequested(int)), this,
           SLOT(tabClosedSlot(int)));
}

TextEdit* TextEditTabBar::addTextEditTab(const QString& file) {
   TextEdit* textEdit(nullptr);
   if (file.startsWith(UntitledNameC)) {
      QString newTabName("");
      for (uint64_t i = 1; i < MaxNewTabCount; ++i) {
         newTabName.assign(tr("%1%2").arg(file).arg(i));
         if (textEditMap_.find(newTabName) == textEditMap_.end()) {
            break;
         }
      }
      if (textEditMap_.find(newTabName) != textEditMap_.end()) {
         qWarning() << file << "has been opened";
         return nullptr;
      }

      textEdit = new TextEdit();  // 不传入this, 手动删除
      textEdit->setFileName(newTabName);
      mainWindow_->setWindowTitle(newTabName);
      textEditMap_.insert(newTabName, textEdit);
   } else {
      const auto& iter = textEditMap_.find(file);
      if (iter != textEditMap_.end()) {
         setCurrentIndex(indexOf(iter.value()));
         return nullptr;
      }

      if (!QFile::exists(file)) {
         return nullptr;
      }

      textEdit = new TextEdit();  // 不传入this, 手动删除
      if (!textEdit->readFile(file)) {
         return nullptr;
      }

      QFileInfo fileInfo(file);
      textEdit->setFileName(fileInfo.fileName());
      textEdit->setFilePath(fileInfo.path());
      mainWindow_->setWindowTitle(file);
      textEditMap_.insert(file, textEdit);
   }

   int index = addTab(textEdit, textEdit->fileName());

   // 与主窗体的联动
   connect(textEdit, SIGNAL(cursorPositionChanged()), mainWindow_,
           SLOT(showPositionSlot()));
   connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChangedSlot()));
   connect(textEdit, SIGNAL(undoAvailable(bool)), mainWindow_,
           SLOT(revokeAvailableSlot(bool)));
   connect(textEdit, SIGNAL(redoAvailable(bool)), mainWindow_,
           SLOT(recoverAvailableSlot(bool)));
   connect(textEdit, SIGNAL(copyAvailable(bool)), mainWindow_,
           SLOT(cutAvailableSlot(bool)));
   connect(textEdit, SIGNAL(copyAvailable(bool)), mainWindow_,
           SLOT(copyAvailableSlot(bool)));

   setTabText(index, textEdit->fileName());
   setTabToolTip(index, file);
   setCurrentIndex(index);
   mainWindow_->setNewTextEdit();

   // qDebug() << "Add tab ok, index" << index;
   return textEdit;
}

TextEdit* TextEditTabBar::currentTextEdit() const {
   return static_cast<TextEdit*>(currentWidget());
}

void TextEditTabBar::settingsChanged() {
   for (TextEdit* te : textEditMap_) {
      te->settingsChanged();
   }
}

void TextEditTabBar::saveSlot() {
   TextEdit* te = currentTextEdit();
   Q_ASSERT(te);
   if (te->document()->isModified()) {
      if (saveFile(te)) {
         setTabText(currentIndex(), te->fileName());
         mainWindow_->setWindowTitle(te->fileFullPath());
      }
   }
}

void TextEditTabBar::mouseDoubleClickEvent(QMouseEvent* event) {
   if (Qt::LeftButton == event->button()) {
      // 新建标签页
      addTextEditTab(UntitledNameC);
   }
   QTabWidget::mouseDoubleClickEvent(event);
}

bool TextEditTabBar::eventFilter(QObject* o, QEvent* e) {
   if (o == tabBar() && e->type() == QEvent::MouseButtonDblClick) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
      tabClosedSlot(tabBar()->tabAt(mouseEvent->pos()));
   }
   return QTabWidget::eventFilter(o, e);
}

void TextEditTabBar::currentIndexChangedSlot(int index) {
   if (index < 0) {
      return;
   }

   TextEdit* te = currentTextEdit();
   Q_ASSERT(te);
   te->setFocus();

   if (te->filePath().isEmpty()) {
      mainWindow_->setWindowTitle(te->fileName());
   } else {
      mainWindow_->setWindowTitle(te->fileFullPath());
      emit currentIndexChangedSignal(te->fileFullPath());
   }
   // qDebug() << "currentIndexChangedSlot(), index changed to" << index << ","
   //          << mainWindow_->windowTitle();

   mainWindow_->showPositionSlot();
   // saveAction_->setEnabled(te->isChanged());
}

void TextEditTabBar::textChangedSlot() {
   TextEdit* te = static_cast<TextEdit*>(currentWidget());
   Q_ASSERT(te);

   if (!te->document()->isModified()) {
      return;
   }

   // 编辑区内容有变化
   QString title(te->fileName());
   title.append(" *");
   setTabText(currentIndex(), title);
   if (te->fileFullPath().isEmpty()) {
      mainWindow_->setWindowTitle(title);
   } else {
      mainWindow_->setWindowTitle(te->fileFullPath() + "*");
   }

   te->getCommentAction()->setEnabled(true);
   QAction* save =
       mainWindow_->findChild<QAction*>(ObjectNameC_MainWindowSaveAction);
   Q_ASSERT(save);
   save->setEnabled(true);
}

void TextEditTabBar::tabClosedSlot(int index) {
   TextEdit* te = static_cast<TextEdit*>(widget(index));
   Q_ASSERT(te);

   bool isContinue(true);
   if (te->document()->isModified()) {
      isContinue = saveFile(te);
   }

   if (isContinue) {
      removeTab(index);
      if (te->fileFullPath().isEmpty()) {
         removeTextEdit(te->fileName());
      } else {
         removeTextEdit(te->fileFullPath());
      }

      if (tabs() == 0) {
         mainWindow_->disableEditActions();
         QAction* close = mainWindow_->findChild<QAction*>(
             ObjectNameC_MainWindowCloseAction);
         if (close) {
            close->setDisabled(true);
         }

         QAction* find =
             mainWindow_->findChild<QAction*>(ObjectNameC_MainWindowFindAction);
         if (find) {
            find->setDisabled(true);
         }
      }
   }
}

bool TextEditTabBar::saveFile(TextEdit* te) {
   Q_ASSERT(te->document()->isModified());
   QString saveFullPath("");

   // 新标签还没有路径
   if (te->filePath().isEmpty()) {
      // 询问是否保存
      int result = QMessageBox::warning(
          this, tr("保存提示"),
          tr("<p>%1 已经改变!</p><p>需要保存吗？</p>").arg(te->fileName()),
          QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);

      if (result == QMessageBox::Cancel) {
         return false;
      }

      if (result == QMessageBox::Yes) {
         // 选择保存的路径
         saveFullPath.assign(QFileDialog::getSaveFileName(
             this, tr("保存文件到"),
             QString("%1/%2").arg(config_->recentDir_).arg(te->fileName()),
             tr("所有文件 (*.*)")));
         if (saveFullPath.isEmpty()) {
            // 没有选择路径，视为取消
            return false;
         }
      } else {
         // 不保存，直接丢弃
         Q_ASSERT(result == QMessageBox::Discard);
         return true;
      }
   } else {
      saveFullPath.append(te->fileFullPath());
   }

   // qDebug() << saveFullPath;
   if (te->writeFile(saveFullPath)) {
      mainWindow_->statusBar()->showMessage(
          tr("文件%1保存成功!").arg(te->fileFullPath()), 2000);
      QFileInfo fileInfo(saveFullPath);

      // 新标签, 以新路径存储
      if (te->filePath().isEmpty()) {
         te->setFilePath(fileInfo.path());
         textEditMap_.remove(te->fileName());
         textEditMap_.insert(te->fileFullPath(), te);
      }
      return true;
   } else {
      return false;
   }
}

bool TextEditTabBar::removeTextEdit(const QString& key) {
   auto iter = textEditMap_.find(key);
   if (iter != textEditMap_.end()) {
      delete iter.value();
      textEditMap_.erase(iter);
      return true;
   }
   return false;
}
