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
#ifndef _IEDITOR_PROJECTTREE_H_
#define _IEDITOR_PROJECTTREE_H_

#include <QTreeView>
#include <QString>

class Config;
class QStandardItemModel;
class QStandardItem;

class FileExplorer final : public QTreeView {
   Q_OBJECT
public:
   explicit FileExplorer(QWidget* parent = nullptr);
   FileExplorer(const FileExplorer&) = delete;
   FileExplorer& operator=(const FileExplorer&) = delete;
   FileExplorer(FileExplorer&&) = delete;
   virtual ~FileExplorer();

   bool loading(const QString& path);

   QString selectedFullFileName(const QModelIndex& index);

   QStandardItemModel* model() { return model_; }

   void settingsChanged();

public slots:
   void selectItem(const QString& fileFullPath);

private:
   bool subLoading(const QString& subPath, QStandardItem* parentItem) const;

private:
   Config* config_;
   QStandardItemModel* model_;
   QString path_;
};

#endif  // _IEDITOR_PROJECTTREE_H_
