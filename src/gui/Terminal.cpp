#include "Terminal.h"
#include "Config.h"

#include <QProcess>
#include <QKeyEvent>
#include <QApplication>

Terminal::Terminal(QWidget* parent)
    : QTextEdit(parent),
      config_(common::Singleton<Config>::instancePointer()),
      proc(nullptr) {
   Q_ASSERT(config_);
   Q_ASSERT(config_->isInit());

   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setWindowTitle(tr("命令行提示符"));
   settingsChangedSlot();

   proc = new QProcess(this);
   connect(proc, SIGNAL(readyReadStandardOutput()), this,
           SLOT(readyReadStandardOutputSlot()));
   connect(proc, SIGNAL(readyReadStandardError()), this,
           SLOT(readyReadStandardErrorSlot()));
}

Terminal::~Terminal() {}

bool Terminal::execCommand(const QString& cmd) {
   if (cmd.isEmpty()) {
      return false;
   }

#ifdef Q_OS_WIN
   proc->start("cmd", QStringList() << "/c" << cmd);
#elif Q_OS_LINUX
   proc->start("bash", QStringList() << "/c" << cmd);
#endif

   if (!proc->waitForStarted()) {
      return false;
   }

   if (!proc->waitForFinished()) {
      return false;
   }

   return true;
}

void Terminal::output(const QString& msg) {
   if (Q_LIKELY(!msg.isEmpty())) {
      append(msg.trimmed());
      moveCursor(QTextCursor::End);
      lastPosition = textCursor().position();
   }
}

void Terminal::keyPressEvent(QKeyEvent* e) {
   QTextCursor editCursor = textCursor();

   // Qt::Key_Enter 是小键盘（数字键盘）的 Enter，对应的虚拟键码为：0x01000005
   // Qt::Key_Return 是大键盘区的 Enter，对应的虚拟键码为：0x01000004
   if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
      e->ignore();
      editCursor.setPosition(lastPosition, QTextCursor::MoveAnchor);
      editCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
      QString string = editCursor.selectedText();
      editCursor.clearSelection();

#ifdef Q_OS_WIN
      lastInput = string.toLocal8Bit() + '\r' + '\n';
#elif Q_OS_LINUX
      lastInput = string.toLocal8Bit() + '\n';
#endif

      proc->write(lastInput);
      return;
   } else if (e->key() == Qt::Key_Backspace &&
              editCursor.position() <= lastPosition) {
      return;
   } else if (e->key() == Qt::Key_Delete &&
              editCursor.position() <= lastPosition) {
      return;
   } else {
      return QTextEdit::keyPressEvent(e);
   }
}

void Terminal::readyReadStandardOutputSlot() {
   output(proc->readAllStandardOutput());
}

void Terminal::readyReadStandardErrorSlot() {
   output(proc->readAllStandardError());
}

void Terminal::settingsChangedSlot() {
   QPalette pl = palette();

   setFont(config_->textFont_);  // TextEdit字体
   pl.setBrush(QPalette::Text,
               QBrush(config_->getCurrentColorStyle()->textColor));  // 文本颜色
   pl.setBrush(QPalette::Base,
               QBrush(config_->getCurrentColorStyle()
                          ->textEditBackgroundColor));  // 背景颜色
   setTabStopDistance(config_->tabWide_ * 10);          // Tab宽度
   setPalette(pl);

   // emit cursorPositionChanged();  // 当前行高亮
   emit textChanged();  // 文本高亮
}
