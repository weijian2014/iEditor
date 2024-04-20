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

#include "FileExplorer.h"
#include "Config.h"

#include <QStandardItemModel>
#include <QDir>
#include <QFileInfo>

FileExplorer::FileExplorer(QWidget* parent)
    : QTreeView(parent),
      config_(common::Singleton<Config>::instancePointer()),
      model_(new QStandardItemModel(parent)) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
   settingsChanged();
}

FileExplorer::~FileExplorer() {}

bool FileExplorer::loading(const QString& path) {
   if (!path_.isEmpty()) {
      model_->clear();
   }

   QDir dir(path);
   if (!dir.exists()) {
      qWarning() << "Directory" << path << "does not exist";
      return false;
   }
   path_.assign(path);

   QStandardItem* item(nullptr);
   dir.setSorting(QDir::SortFlag::Name | QDir::SortFlag::DirsFirst);
   dir.setFilter(dir.filter() | QDir::NoDotAndDotDot);  // 去掉.和..目录
   QFileInfoList list = dir.entryInfoList();

   // Root节点
   QStandardItem* root = new QStandardItem(dir.dirName());
   model_->appendRow(root);

   // qDebug() << "Loading directory" << path_ << ", files" << list.length()
   //          << "in this level";

   for (const auto& f : list) {
      item = new QStandardItem(f.fileName());
      Q_ASSERT(item);
      if (f.isDir()) {
         item->setIcon(QIcon(":/RemixIcon-v4.2.0/Document/folder-2-fill.svg"));
         item->setToolTip(f.absoluteFilePath());
         if (!subLoading(f.absoluteFilePath(), item)) {
            return false;
         }
      } else if (f.isFile()) {
         item->setIcon(QIcon(":/RemixIcon-v4.2.0/Document/file-2-fill.svg"));
         item->setData(QString("%1/%2").arg(path_).arg(f.fileName()));
         item->setToolTip(item->data().toString());
      }
      root->appendRow(item);
   }

   model_->setHeaderData(0, Qt::Horizontal, path_);  // 设置表头数据
   setModel(model_);
   setHeaderHidden(true);  // 隐藏表头
   // expandAll();            // 展开所有节点
   setExpanded(model_->indexFromItem(root), true);  // 只展开根节点
   return true;
}

void FileExplorer::selectItem(const QString& fileFullPath) {
   if (!model_) {
      return;
   }

   QFileInfo fileInfo(fileFullPath);
   if (!fileInfo.isFile()) {
      return;
   }

   if (!fileInfo.path().startsWith(path_)) {
      return;
   }

   QList<QStandardItem*> items = model_->findItems(
       fileInfo.fileName(),
       Qt::MatchCaseSensitive | Qt::MatchExactly | Qt::MatchRecursive);

   for (const auto& item : items) {
      // 遍历找到相同路径下的项
      if (item->data().toString() == fileFullPath) {
         setCurrentIndex(item->index());
         break;
      }
   }
}

void FileExplorer::settingsChanged() {
   // 样式
   QPalette pl = palette();
   setFont(config_->textFont_);
   pl.setBrush(QPalette::Text,
               QBrush(config_->getCurrentColorStyle()->textColor));  // 文本颜色
   pl.setBrush(QPalette::Base,
               QBrush(config_->getCurrentColorStyle()
                          ->textEditBackgroundColor));  // 背景颜色
   setPalette(pl);
}

QString FileExplorer::selectedFullFileName(const QModelIndex& index) {
   const auto* item = model_->itemFromIndex(index);
   if (item) {
      return item->data().toString();
   } else {
      return "";
   }
}

bool FileExplorer::subLoading(const QString& subPath,
                              QStandardItem* parentItem) const {
   QDir dir(subPath);
   if (!dir.exists()) {
      qWarning() << "Sub directory " << subPath << "does not exist";
      return false;
   }

   QStandardItem* item(nullptr);
   dir.setSorting(QDir::SortFlag::Name | QDir::SortFlag::DirsFirst);
   dir.setFilter(dir.filter() | QDir::NoDotAndDotDot);  // 去掉.和..目录
   QFileInfoList list = dir.entryInfoList();

   // qDebug() << "Loading sub directory" << subPath << ", files" <<
   // list.length()
   //          << "in this level";

   for (const auto& f : list) {
      item = new QStandardItem(f.fileName());

      Q_ASSERT(item);
      if (f.isDir()) {
         item->setIcon(QIcon(":/RemixIcon-v4.2.0/Document/folder-2-fill.svg"));
         item->setToolTip(f.absoluteFilePath());
         if (!subLoading(f.absoluteFilePath(), item)) {
            return false;
         }
      } else if (f.isFile()) {
         item->setIcon(QIcon(":/RemixIcon-v4.2.0/Document/file-2-fill.svg"));
         item->setData(QString("%1/%2/%3")
                           .arg(path_)
                           .arg(parentItem->text())
                           .arg(f.fileName()));  // 设置文件的全路径为数据
         item->setToolTip(item->data().toString());
      }
      parentItem->appendRow(item);
   }

   return true;
}
