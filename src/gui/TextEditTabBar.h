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
#ifndef _IEDITOR_TEXTEDITTAB_H_
#define _IEDITOR_TEXTEDITTAB_H_

#include <QTabWidget>
#include <memory>

class Config;
class TextEdit;
class MainWindow;

class TextEditTabBar final : public QTabWidget {
   Q_OBJECT

public:
   explicit TextEditTabBar(QWidget* parent = nullptr);
   TextEditTabBar(const TextEditTabBar&) = delete;
   TextEditTabBar& operator=(const TextEditTabBar&) = delete;
   TextEditTabBar(TextEditTabBar&&) = delete;
   virtual ~TextEditTabBar() {}

public:
   TextEdit* addTextEditTab(const QString& file);
   uint64_t tabs() const { return textEditMap_.size(); }
   TextEdit* currentTextEdit() const;

   void settingsChanged();

signals:
   void currentIndexChangedSignal(const QString& fileFullPath);

public slots:
   void saveSlot();                // 保存内容
   void tabClosedSlot(int index);  // tab关闭

protected:
   void mouseDoubleClickEvent(QMouseEvent* event);
   bool eventFilter(QObject*, QEvent*);

private slots:
   void currentIndexChangedSlot(int index);  // tab切换
   void textChangedSlot();                   // 当前文本内容改变

private:
   bool saveFile(TextEdit* te);
   bool removeTextEdit(const QString& key);

private:
   Config* config_;
   MainWindow* mainWindow_;
   QMap<QString, TextEdit*> textEditMap_;  // <文件完全路径名，编辑器>
};

#endif  // _IEDITOR_TEXTEDITTAB_H_
