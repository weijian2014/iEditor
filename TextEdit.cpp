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

#include <QtGui>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

#include "TextEdit.h"
#include "ConfigStruct.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void Highlighter::highlightBlock(const QString &text)
{
  // if (!ConfigStruct::GetConfig()->isSimpleTextMode) {
  //     QVector<ConfigStruct::HighlightRule> highlightRuleList =
  //     ConfigStruct::GetConfig()->highlightRuleList; foreach (const
  //     ConfigStruct::HighlightRule &rule, highlightRuleList){
  //         QRegExp regExp(rule.regExp);
  //         int index = regExp.indexIn(text);//找到与正则相匹配index
  //         while (index >= 0){
  //             int count = regExp.matchedLength();
  //             setFormat(index, count, rule.format);
  //             index = regExp.indexIn(text, index + count);
  //         }
  //     }
  //     setCurrentBlockState(0);

  //     int startIndex = 0;
  //     if (1 != previousBlockState())
  //         startIndex =
  //         ConfigStruct::GetConfig()->multipleLinesCommentRegExpStart.indexIn(text);
  //     while (0 <= startIndex){
  //         int endIndex =
  //         ConfigStruct::GetConfig()->multipleLinesCommentRegExpEnd.indexIn(text,
  //         startIndex); int commentLength; if (-1 == endIndex){
  //             setCurrentBlockState(1);
  //             commentLength = text.length() - startIndex;
  //         }
  //         else {
  //             commentLength = endIndex - startIndex +
  //             ConfigStruct::GetConfig()->multipleLinesCommentRegExpEnd.matchedLength();
  //         }

  //         setFormat(startIndex, commentLength,
  //         ConfigStruct::GetConfig()->multiLineCommentFormat); startIndex =
  //         ConfigStruct::GetConfig()->multipleLinesCommentRegExpStart.indexIn(text,
  //         startIndex + commentLength);
  //     }
  // }
}

TextEdit::TextEdit() {
  filePath = "";
  isChange = false;
  lineNumberArea = 0;
  higtlighter = 0;
  CreteaContextMenu();  // 创建右键菜单
  SetTextEdit();

  setFrameStyle(0);
  // setViewportMargins(ConfigStruct::GetConfig()->lineNumberAreaWide, 0, 0, 0);
  // //行号区域大小
  setWordWrapMode(QTextOption::WrapAnywhere);
  lineNumberArea = new LineNumberArea(this);

  if (ConfigStruct::GetConfig()->isHighlightCurrentLine) {
    HighlightCurrenLine();  // 高亮当前行
  }

  connect(this, SIGNAL(cursorPositionChanged()), this,
          SLOT(HighlightCurrenLine()));

  connect(this, SIGNAL(updateRequest(QRect, int)), this,
          SLOT(UpdateLineNumberArea(QRect, int)));
}

TextEdit::~TextEdit() {
  if (!higtlighter) delete higtlighter;
  if (!lineNumberArea) delete lineNumberArea;
}

void TextEdit::SetTextEdit() {
  QPalette pl = palette();
  setFont(ConfigStruct::GetConfig()->textFont);  // TextEdit字体
  pl.setBrush(QPalette::Text,
              QBrush(ConfigStruct::GetConfig()->textColor));  // 文本颜色
  pl.setBrush(
      QPalette::Base,
      QBrush(ConfigStruct::GetConfig()->textEditBackgroundColor));  // 背景颜色
  setTabStopDistance(ConfigStruct::GetConfig()->tabWide * 10);      // Tab宽度
  setPalette(pl);
  emit cursorPositionChanged();  // 更新当前行高亮

  if (!ConfigStruct::GetConfig()->isSimpleTextMode) {  // 更新文本高亮
    ConfigStruct::GetConfig()->CreateHighlightRuleList();  // 更新各种高亮颜色
    higtlighter = new Highlighter(document());
    emit textChanged();
  } else {
    if (0 != higtlighter) delete higtlighter;
    higtlighter = 0;
    emit textChanged();
  }
}

void TextEdit::CreteaContextMenu() {
  setContextMenuPolicy(Qt::CustomContextMenu);
  commentAction = new QAction(tr("增加注释"), this);
  commentAction->setShortcut(tr("Ctrl+/"));
  commentAction->setToolTip(tr("注释当前行或选中的行"));
  connect(commentAction, SIGNAL(triggered()), this, SLOT(Comment()));

  unCommentAction = new QAction(tr("撤销注释"), this);
  unCommentAction->setShortcut(tr("Ctrl+."));
  unCommentAction->setToolTip(tr("反注释当前行或选中的行"));
  connect(unCommentAction, SIGNAL(triggered()), this, SLOT(UnComment()));

  propertyAction = new QAction(tr("文件属性"), this);
  propertyAction->setShortcut(Qt::Key_F5);
  propertyAction->setToolTip(tr("查看当前文件的信息"));
  connect(propertyAction, SIGNAL(triggered()), this, SLOT(Property()));

  contextMenu = new QMenu(this);
  contextMenu->addAction(commentAction);
  contextMenu->addAction(unCommentAction);
  contextMenu->addSeparator();
  contextMenu->addAction(propertyAction);
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(ShowContextMenu()));
}

void TextEdit::ShowContextMenu() { contextMenu->exec(QCursor::pos()); }

void TextEdit::Comment() {
  QTextCursor tCursor = textCursor();
  QTextBlock textBlock = tCursor.block();
  int blockCount = 0;

  if (tCursor.hasSelection()) {
    int selectionCount = tCursor.selectionEnd() - tCursor.selectionStart();
    int beginLine, endLine;
    QString selectionTexts = "//";

    if (tCursor.atBlockStart()) {
      beginLine = tCursor.blockNumber();
      tCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                           selectionCount);
      endLine = tCursor.blockNumber();
      blockCount = endLine - beginLine + 1;

      if (tCursor.atBlockEnd()) {
        tCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                             selectionCount);

        for (int i = 0; i != blockCount; ++i) {
          selectionTexts.append(textBlock.text());
          textBlock = textBlock.next();
          if (blockCount != (i + 1)) selectionTexts.append("\r//");
        }

        insertPlainText(selectionTexts);

      } else {
        tCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                             selectionCount);
        insertPlainText("/*" + tCursor.selectedText() + "*/");
      }
    } else if (tCursor.atBlockEnd()) {
      beginLine = tCursor.blockNumber();
      tCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                           selectionCount);
      endLine = tCursor.blockNumber();
      blockCount = beginLine - endLine + 1;
      QStack<QString> stackString;
      if (tCursor.atBlockStart()) {
        for (int i = 0; i != blockCount; ++i) {
          if (0 == i) {
            selectionTexts = "//" + textBlock.text();
          } else {
            selectionTexts = "//" + textBlock.text() + "\r";
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
        tCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                             selectionCount);
        insertPlainText("/*" + tCursor.selectedText() + "*/");
      }
    } else {
      insertPlainText("/*" + tCursor.selectedText() + "*/");
    }

  } else {
    moveCursor(QTextCursor::StartOfBlock);
    insertPlainText("//");
    setTextCursor(tCursor);
  }
}

void TextEdit::UnComment() {
  QTextCursor tCursor = textCursor();
  QTextBlock textBlock = tCursor.block();
  QString unString;

  if (tCursor.hasSelection()) {
    unString = tCursor.selectedText();
    unString.replace("//", "");
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

void TextEdit::Property() {
  if ("" == filePath) return;
  QFileInfo fileInfo(filePath);
  if (!fileInfo.exists()) return;
  QMessageBox::information(
      this, tr("%1文件属性-iEditor").arg(fileInfo.fileName()),
      tr("<font color=darkblue><p>文件名称: %1</p><p>扩展名称: "
         "%2</p><p>文件大小: %3 Byte</p><p>存储位置: %4</p><p>是否可读: "
         "%5</p><p>是否可写: %6</p><p>创建时间: %7</p><p>最后访问: "
         "%8</p><p>修改时间: %9</p></font>")
          .arg(fileInfo.fileName().split(".").first())
          .arg(fileInfo.suffix())
          .arg(fileInfo.size())
          .arg(fileInfo.path())
          .arg(fileInfo.isReadable())
          .arg(fileInfo.isWritable())
          .arg(fileInfo.birthTime().toString())
          .arg(fileInfo.lastRead().toString())
          .arg(fileInfo.lastModified().toString()));
}

bool TextEdit::ReadDocument(const QString &filePath) {
  setDocumentTitle(filePath);

  if (tr("新建文件") == filePath) return false;

  QFile file(filePath);

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
      this, tr("读取出错-iEditor"),
      tr("<p>无法读取[%1],出错信息：</p><font color=red>%2</font>")
          .arg(filePath)
          .arg(file.errorString()));
  file.close();
  return false;
}

bool TextEdit::WriteDocument(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("iEditor提示"),
                         tr("保存[%1]文件出错0.0<p>出错信息:%2</p>")
                             .arg(filePath)
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

void TextEdit::HighlightCurrenLine() {
  QList<QTextEdit::ExtraSelection> extraSelectionList;
  if (!isReadOnly()) {
    QTextEdit::ExtraSelection extraSelection;
    extraSelection.cursor = textCursor();
    if (ConfigStruct::GetConfig()->isHighlightCurrentLine) {
      extraSelection.format.setBackground(
          ConfigStruct::GetConfig()->currentLineColor);
    } else {
      extraSelection.format.setBackground(
          ConfigStruct::GetConfig()->textEditBackgroundColor);
    }
    extraSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    extraSelectionList.append(extraSelection);
  }
  setExtraSelections(extraSelectionList);
}

void TextEdit::UpdateLineNumberArea(const QRect &rect, int pos) {
  if (pos)
    lineNumberArea->scroll(0, pos);
  else {
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
  }
}

void TextEdit::resizeEvent(QResizeEvent *event) {
  QPlainTextEdit::resizeEvent(event);
  QRect rect = contentsRect();
  lineNumberArea->setGeometry(
      QRect(rect.left(), rect.top(),
            ConfigStruct::GetConfig()->lineNumberAreaWide, rect.height()));
  setViewportMargins(lineNumberArea->width(), 0, 0, 0);  // 行号区域大小
}

void TextEdit::DrawLinwNumberEvent(QPaintEvent *event) {
  if (ConfigStruct::GetConfig()->isShowRowNumber) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),
                     ConfigStruct::GetConfig()->lineAreaBackgroundColor);
    painter.setPen(ConfigStruct::GetConfig()->lineNumberColor);
    QFont f = QFont(ConfigStruct::GetConfig()->textFont);
    f.setPointSize(fontMetrics().height() - fontMetrics().height() / 2.5);
    painter.setFont(f);
    QTextBlock textBlock = firstVisibleBlock();
    int blockNumber = textBlock.blockNumber();
    int top =
        (int)blockBoundingGeometry(textBlock).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(textBlock).height();
    QString number;

    while (textBlock.isValid() && top <= event->rect().bottom()) {
      if (textBlock.isVisible() && bottom >= event->rect().top()) {
        number = QString::number(blockNumber + 1);
        painter.drawText(
            0, top, lineNumberArea->width() - lineNumberArea->width() / 8,
            fontMetrics().height(), Qt::AlignRight | Qt::AlignVCenter, number);
      }

      textBlock = textBlock.next();
      top = bottom;
      bottom = top + (int)blockBoundingRect(textBlock).height();
      ++blockNumber;
    }
  } else {
    QPainter painter(lineNumberArea);
    painter.fillRect(lineNumberArea->rect(),
                     ConfigStruct::GetConfig()->lineAreaBackgroundColor);
  }
}
