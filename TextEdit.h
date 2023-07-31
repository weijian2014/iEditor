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
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui>
#include <QTextEdit>
#include <QFile>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
    
public:
    explicit Highlighter(QTextDocument *parent = 0);
    
protected:
    void highlightBlock(const QString &text);
};//HighLighter


class TextEdit : public QPlainTextEdit
{
    Q_OBJECT
    
public:
    explicit TextEdit();
    ~TextEdit();
    
    bool ReadDocument(const QString &filePath);
    bool WriteDocument(const QString &filePath);
    
protected:
    //void keyReleaseEvent(QKeyEvent *e);
    
signals:
    //void KeyPressed(bool);
    
public slots:
    void DrawLinwNumberEvent(QPaintEvent *event);
    void Comment();
    void UnComment();
    void Property();
    void SetTextEdit();
    
private slots:
    void HighlightCurrenLine();
    void resizeEvent(QResizeEvent *event);
    void UpdateLineNumberArea(const QRect &, int pos);
    void ShowContextMenu();
    
public:
    QString filePath;
    bool isChange;
    QAction *commentAction;
    QAction *unCommentAction;
    QAction *propertyAction;
    
private:
    void CreteaContextMenu();
    
    Highlighter *higtlighter;
    QWidget *lineNumberArea;
    QMenu *contextMenu;
    
};//TextEdit


class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(TextEdit *parent) : QWidget(parent)
    {
        textEditArea = parent;
    }
    
    ~LineNumberArea() { }
    
protected:
    void paintEvent(QPaintEvent *event){
        textEditArea->DrawLinwNumberEvent(event);
    }
        
private:
    TextEdit *textEditArea;
};//LineNumberArea

#endif // TEXTEDITAREA_H
