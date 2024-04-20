/****************************************************************************
**
**      Datetime:   2013年03月21日 星期四 15时43分13秒
**      Author  :   韦   健
**      Titles  :   该类用于文件的显示，编辑、高亮、着色以及等号显示等，高亮用了
**                  QT提供的QSyntaxHighlighter，使用正则表达对词进行着色，词
**                  可以在软件中的config.ini中自定义
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef _IEDITOR_TEXTEDIT_H_
#define _IEDITOR_TEXTEDIT_H_

#include <QtGui>
#include <QTextEdit>
#include <QFile>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QPlainTextEdit>

class Config;

class Highlighter : public QSyntaxHighlighter {
   Q_OBJECT

public:
   Highlighter(QTextDocument* parent = nullptr);

protected:
   void highlightBlock(const QString& text) override;

private:
   Config* config_;
};

class TextEdit final : public QPlainTextEdit {
   Q_OBJECT

public:
   explicit TextEdit(QWidget* parent = nullptr);
   TextEdit(const TextEdit&) = delete;
   TextEdit& operator=(const TextEdit&) = delete;
   TextEdit(TextEdit&&) = delete;
   virtual ~TextEdit();

   void lineNumberAreaPaintEvent(QPaintEvent* event);
   int lineNumberAreaWidth();

   bool readFile(const QString& fileFullPath);
   bool writeFile(const QString& fileFullPath);

   QAction* getPropertyAction() { return propertyAction_; }
   QAction* getCommentAction() { return commentAction_; }

   const QString& fileName() const { return fileName_; }
   void setFileName(const QString& name) { fileName_.assign(name); }

   const QString& filePath() const { return filePath_; }
   void setFilePath(const QString& path) {
      filePath_.assign(path);
      fileFullPath_.assign(QString("%1/%2").arg(path).arg(fileName_));
   }

   const QString fileFullPath() const { return fileFullPath_; }

   void settingsChanged();

protected:
   void resizeEvent(QResizeEvent* event) override;

private slots:
   void commentHandleSlot();
   void propertySlot();

   void highlightCurrentLineSlot();
   void updateLineNumberAreaWidthSlot(int newBlockCount);
   void updateLineNumberAreaSlot(const QRect& rect, int dy);
   void showContextMenuSlot();

private:
   void creteaContextMenu();
   void unComment();

   Config* config_;
   QAction* commentAction_;
   QAction* propertyAction_;
   Highlighter* higtlighter_;
   QWidget* lineNumberArea_;
   QMenu* contextMenu_;
   QString fileName_;
   QString filePath_;
   QString fileFullPath_;
};  // TextEdit

class LineNumberArea final : public QWidget {
public:
   LineNumberArea(TextEdit* editor) : QWidget(editor), editor_(editor) {}

   QSize sizeHint() const override {
      return QSize(editor_->lineNumberAreaWidth(), 0);
   }

protected:
   void paintEvent(QPaintEvent* event) override {
      editor_->lineNumberAreaPaintEvent(event);
   }

private:
   TextEdit* editor_;
};

#endif  // _IEDITOR_TEXTEDIT_H_
