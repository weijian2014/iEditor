/****************************************************************************
**
**      Datetime:   2013-4-15
**      Author  :   韦   健
**      Titles  :   用于关闭iEditor时的提示已经被修改过的文件进行保存，该部件中
**                  可以选择性的保存想要保存的文件，可以不进行保存而直接关闭软件
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>

class QLabel;
class QDialogButtonBox;
class QListWidget;
class QAbstractButton;
class QListWidgetItem;

class SaveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();
    
    void SetFileNameList(QStringList &fileNameList);
    
signals:
    void SvaeSignal(QList<QListWidgetItem *>);
    void NoSaveSignal(const QString &);
    
private slots:
    void ItemSelectionChang();
    void Click(QAbstractButton*);
    
private:
    QLabel *topMassageLable;
    QLabel *midMassageLable;
    QLabel *bottomMassegeLable;
    QDialogButtonBox *buttonBox;
    QListWidget *saveListWidget;
};

#endif 
