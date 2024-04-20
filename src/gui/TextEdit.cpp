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

#include "TextEdit.h"
#include "Define.h"
#include "Config.h"
#include "MainWindow.h"

#include <QtGui>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

static const QString CommentPrefixC("//");

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent),
      config_(common::Singleton<Config>::instancePointer()) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
}

void Highlighter::highlightBlock(const QString& text) {
   if (!config_->isSimpleTextMode_) {
      for (const auto& rule : std::as_const(config_->highlightRuleList_)) {
         QRegularExpressionMatchIterator matchIterator =
             rule->pattern.globalMatch(text);
         while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(),
                      rule->format);
         }
      }
      setCurrentBlockState(0);
      int startIndex = 0;
      if (previousBlockState() != 1) {
         startIndex = text.indexOf(config_->multipleLinesCommentRegExpStart_);
      }

      while (startIndex >= 0) {
         QRegularExpressionMatch match =
             config_->multipleLinesCommentRegExpEnd_.match(text, startIndex);
         int endIndex = match.capturedStart();
         int commentLength = 0;
         if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
         } else {
            commentLength = endIndex - startIndex + match.capturedLength();
         }
         setFormat(startIndex, commentLength, config_->multiLineCommentFormat_);
         startIndex = text.indexOf(config_->multipleLinesCommentRegExpStart_,
                                   startIndex + commentLength);
      }
   }
}

TextEdit::TextEdit(QWidget* parent)
    : QPlainTextEdit(parent),
      config_(common::Singleton<Config>::instancePointer()),
      commentAction_(nullptr),
      propertyAction_(nullptr),
      higtlighter_(nullptr),
      lineNumberArea_(nullptr),
      contextMenu_(nullptr),
      fileName_(""),
      filePath_(""),
      fileFullPath_("") {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());
   creteaContextMenu();  // 创建右键菜单
   higtlighter_ = new Highlighter(document());
   setFrameStyle(0);
   setWordWrapMode(QTextOption::WrapAnywhere);
   lineNumberArea_ = new LineNumberArea(this);

   updateLineNumberAreaWidthSlot(0);
   settingsChanged();

   if (config_->isHighlightCurrentLine_) {
      highlightCurrentLineSlot();
   }

   connect(this, &TextEdit::blockCountChanged, this,
           &TextEdit::updateLineNumberAreaWidthSlot);
   connect(this, &TextEdit::updateRequest, this,
           &TextEdit::updateLineNumberAreaSlot);
   connect(this, &TextEdit::cursorPositionChanged, this,
           &TextEdit::highlightCurrentLineSlot);
}

TextEdit::~TextEdit() { config_ = nullptr; }

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent* event) {
   QPainter painter(lineNumberArea_);
   painter.fillRect(event->rect(),
                    config_->getCurrentColorStyle()->textEditBackgroundColor);
   QTextBlock block = firstVisibleBlock();
   int blockNumber = block.blockNumber();
   int top =
       qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
   int bottom = top + qRound(blockBoundingRect(block).height());
   while (block.isValid() && top <= event->rect().bottom()) {
      if (block.isVisible() && bottom >= event->rect().top()) {
         QString number = QString::number(blockNumber + 1);
         painter.setPen(config_->getCurrentColorStyle()->lineNumberColor);
         painter.drawText(0, top, lineNumberArea_->width(),
                          fontMetrics().height(), Qt::AlignHCenter,
                          number);  // Qt::AlignHCenter 行号在行号区域居中显示
      }

      block = block.next();
      top = bottom;
      bottom = top + qRound(blockBoundingRect(block).height());
      ++blockNumber;
   }
}

int TextEdit::lineNumberAreaWidth() {
   int digits = 1;
   int max = qMax(1, blockCount());
   while (max >= 10) {
      max /= 10;
      ++digits;
   }

   int space = config_->lineNumberAreaWide_ +
               fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
   return space;
}

bool TextEdit::readFile(const QString& fileFullPath) {
   if (QString(UntitledNameC) == fileFullPath) {
      return false;
   }

   QFile file(fileFullPath);

   if (file.open(QFile::ReadOnly | QFile::Text)) {
      QTextStream in(&file);

#ifndef QT_NO_CURSOR
      QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
      setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
      QApplication::restoreOverrideCursor();
#endif
      file.close();
      return true;
   }

   QMessageBox::warning(
       this, tr("读取出错"),
       tr("<p>无法读取[%1],出错信息：</p><font color=red>%2</font>")
           .arg(fileFullPath)
           .arg(file.errorString()));
   file.close();
   return false;
}

bool TextEdit::writeFile(const QString& fileFullPath) {
   QFile file(fileFullPath);
   if (!file.open(QFile::WriteOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("ieditor提示"),
                           tr("保存[%1]文件出错0.0<p>出错信息:%2</p>")
                               .arg(fileFullPath)
                               .arg(file.errorString()));
      return false;
   }

   QTextStream out(&file);

#ifndef QT_NO_CURSOR
   QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
   out << toPlainText();
#ifndef QT_NO_CURSOR
   QApplication::restoreOverrideCursor();
#endif
   file.close();
   return true;
}

void TextEdit::settingsChanged() {
   QPalette pl = palette();
   setFont(config_->textFont_);  // TextEdit字体
   pl.setBrush(QPalette::Text,
               QBrush(config_->getCurrentColorStyle()->textColor));  // 文本颜色
   pl.setBrush(QPalette::Base,
               QBrush(config_->getCurrentColorStyle()
                          ->textEditBackgroundColor));  // 背景颜色
   setTabStopDistance(config_->tabWide_ * 10);          // Tab宽度
   setPalette(pl);

   // 显示/隐藏行号
   if (config_->isShowRowNumber_) {
      lineNumberArea_->show();
   } else {
      lineNumberArea_->hide();
   }
   updateLineNumberAreaWidthSlot(0);

   emit cursorPositionChanged();  // 更新编辑区当前行高亮
   higtlighter_->rehighlight();   // 颜色样式变化时更新文本高亮
}

void TextEdit::resizeEvent(QResizeEvent* event) {
   QPlainTextEdit::resizeEvent(event);
   QRect cr = contentsRect();
   lineNumberArea_->setGeometry(
       QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TextEdit::commentHandleSlot() {
   QTextCursor cursor = textCursor();
   QTextBlock textBlock = cursor.block();
   int selectedRows = 0;
   if (cursor.hasSelection()) {
      QString selectedText = cursor.selectedText();
      int selectedChars = cursor.selectionEnd() - cursor.selectionStart();
      int beginLine(0), endLine(0);
      QString selectionTexts = CommentPrefixC;

      if (cursor.atBlockStart()) {
         beginLine = cursor.blockNumber();
         cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                             selectedChars);
         endLine = cursor.blockNumber();
         selectedRows = endLine - beginLine + 1;

         if (cursor.atBlockEnd()) {
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                                selectedChars);
            bool isUncomment(true);
            for (int i = 0; i != selectedRows; ++i) {
               // 判断是否所有行都增加了注释
               if (textBlock.text().left(2) != CommentPrefixC) {
                  isUncomment = false;
                  break;
               }
               textBlock = textBlock.next();
            }

            if (isUncomment) {
               // qDebug() << "Uncomment";
               cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                                   selectedChars);
               setTextCursor(cursor);
               unComment();
               return;
            }

            for (int i = 0; i != selectedRows; ++i) {
               selectionTexts.append(textBlock.text());
               textBlock = textBlock.next();
               if (selectedRows != (i + 1)) selectionTexts.append("\r//");
            }

            insertPlainText(selectionTexts);

         } else {
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                                selectedChars);
            insertPlainText("/*" + cursor.selectedText() + "*/");
         }
      } else if (cursor.atBlockEnd()) {
         beginLine = cursor.blockNumber();
         cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                             selectedChars);
         endLine = cursor.blockNumber();
         selectedRows = beginLine - endLine + 1;
         QStack<QString> stackString;
         if (cursor.atBlockStart()) {
            for (int i = 0; i != selectedRows; ++i) {
               if (0 == i) {
                  selectionTexts = CommentPrefixC + textBlock.text();
               } else {
                  selectionTexts = CommentPrefixC + textBlock.text() + "\r";
               }
               stackString.push(selectionTexts);
               textBlock = textBlock.previous();
            }

            selectionTexts = "";
            while (!stackString.isEmpty()) {
               selectionTexts.append(stackString.pop());
            }

            insertPlainText(selectionTexts);
         } else {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                selectedChars);
            insertPlainText("/*" + cursor.selectedText() + "*/");
         }
      } else {
         insertPlainText("/*" + cursor.selectedText() + "*/");
      }
   } else {
      moveCursor(QTextCursor::StartOfBlock);
      if (textBlock.text().left(2) == CommentPrefixC) {
         setTextCursor(cursor);
         unComment();
         return;
      }

      insertPlainText(CommentPrefixC);
      setTextCursor(cursor);
   }
}

void TextEdit::propertySlot() {
   if (filePath_.isEmpty()) {
      return;
   }

   QFileInfo fileInfo(filePath_);
   if (!fileInfo.exists()) {
      return;
   }

   QMessageBox::information(
       this, tr("%1文件属性").arg(fileInfo.fileName()),
       tr("<font color=darkblue><p>文件名称: %1</p><p>扩展名称: "
          "%2</p><p>文件大小: %3 字节</p><p>存储位置: %4</p><p>是否可读: "
          "%5</p><p>是否可写: %6</p><p>创建时间: %7</p><p>最后访问: "
          "%8</p><p>修改时间: %9</p></font>")
           .arg(fileInfo.fileName().split(".").first())
           .arg(fileInfo.suffix())
           .arg(fileInfo.size())
           .arg(fileInfo.path())
           .arg(fileInfo.isReadable() ? tr("是") : tr("否"))
           .arg(fileInfo.isWritable() ? tr("是") : tr("否"))
           .arg(fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"))
           .arg(fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"))
           .arg(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss")));
}

void TextEdit::highlightCurrentLineSlot() {
   QList<QTextEdit::ExtraSelection> extraSelections;
   if (!isReadOnly()) {
      QTextEdit::ExtraSelection selection;
      selection.cursor = textCursor();

      QColor currentlineBackgroundColor;
      if (config_->isHighlightCurrentLine_) {
         currentlineBackgroundColor =
             config_->getCurrentColorStyle()->currentLineColor;
      } else {
         currentlineBackgroundColor =
             config_->getCurrentColorStyle()->textEditBackgroundColor;
      }

      selection.format.setBackground(currentlineBackgroundColor);
      selection.format.setProperty(QTextFormat::FullWidthSelection, true);
      selection.cursor = textCursor();
      selection.cursor.clearSelection();
      extraSelections.append(selection);
   }

   setExtraSelections(extraSelections);
}

void TextEdit::updateLineNumberAreaWidthSlot(int newBlockCount) {
   (void)newBlockCount;
   int width(0);
   if (config_->isShowRowNumber_) {
      width = lineNumberAreaWidth();
   }
   setViewportMargins(width, 0, 0, 0);
}

void TextEdit::updateLineNumberAreaSlot(const QRect& rect, int dy) {
   if (dy) {
      lineNumberArea_->scroll(0, dy);
   } else {
      lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(),
                              rect.height());
   }

   if (rect.contains(viewport()->rect())) {
      updateLineNumberAreaWidthSlot(0);
   }
}

void TextEdit::showContextMenuSlot() { contextMenu_->exec(QCursor::pos()); }

void TextEdit::creteaContextMenu() {
   setContextMenuPolicy(Qt::CustomContextMenu);
   commentAction_ = new QAction(tr("增加或撤销注释"), this);
   commentAction_->setShortcut(QKeySequence("Ctrl+/"));
   commentAction_->setShortcutContext(Qt::WidgetShortcut);
   commentAction_->setToolTip(tr("增加或撤销当前行或选中行注释"));

   propertyAction_ = new QAction(tr("文件属性"), this);
   propertyAction_->setShortcut(Qt::Key_F6);
   propertyAction_->setShortcutContext(Qt::WidgetShortcut);
   propertyAction_->setToolTip(tr("查看当前文件的信息"));

   // 右键菜单快捷键，解决上方设置快捷键不生效
   QShortcut* commentActionShortcut =
       new QShortcut(QKeySequence("Ctrl+/"), this, SLOT(commentHandleSlot()),
                     SLOT(commentHandleSlot()), Qt::WidgetShortcut);
   (void)commentActionShortcut;

   QShortcut* propertyActionShortcut =
       new QShortcut(QKeySequence(Qt::Key_F6), this, SLOT(propertySlot()),
                     SLOT(propertySlot()), Qt::WidgetShortcut);
   (void)propertyActionShortcut;

   connect(commentAction_, SIGNAL(triggered()), this,
           SLOT(commentHandleSlot()));
   connect(propertyAction_, SIGNAL(triggered()), this, SLOT(propertySlot()));

   contextMenu_ = new QMenu(this);
   contextMenu_->addAction(commentAction_);
   contextMenu_->addSeparator();
   contextMenu_->addAction(propertyAction_);

   connect(this, SIGNAL(customContextMenuRequested(QPoint)), this,
           SLOT(showContextMenuSlot()));
}

void TextEdit::unComment() {
   QTextCursor tCursor = textCursor();
   QTextBlock textBlock = tCursor.block();
   QString unString;

   if (tCursor.hasSelection()) {
      unString = tCursor.selectedText();
      unString.replace(CommentPrefixC, "");
      unString.replace("/*", "");
      unString.replace("*/", "");
      insertPlainText(unString);
   } else {
      moveCursor(QTextCursor::StartOfBlock);
      QTextCursor newTCursor = textCursor();
      unString = textBlock.text();
      if ("//" != unString.left(2)) return;
      if (newTCursor.atBlockStart()) {
         newTCursor.deleteChar();
         newTCursor.deleteChar();
      }
      setTextCursor(tCursor);
   }
}
