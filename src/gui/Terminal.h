#ifndef _IEDITOR_TERMINAL_H_
#define _IEDITOR_TERMINAL_H_

#include <QTextEdit>

class Config;
class QProcess;

class Terminal final : public QTextEdit {
   Q_OBJECT

public:
   Terminal(QWidget* parent = nullptr);
   Terminal(const Terminal&) = delete;
   Terminal& operator=(const Terminal&) = delete;
   Terminal(Terminal&&) = delete;
   ~Terminal();

   bool execCommand(const QString& cmd);
   void output(const QString& msg);

protected:
   virtual void keyPressEvent(QKeyEvent* e) override;

public slots:
   void readyReadStandardOutputSlot();  // 有标准输出时
   void readyReadStandardErrorSlot();   // 有标准错误输出时
   void settingsChangedSlot();          // 颜色样式改变时

private:
   Config* config_;
   QProcess* proc;          // 命令行终端进程
   long long lastPosition;  // 光标位置
   QByteArray lastInput;    // 上一次的输入内容

};  // class Terminal

#endif  // _IEDITOR_TERMINAL_H_
