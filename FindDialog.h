/****************************************************************************
**
**      Datetime:   2013-03-22 12：21
**      Author  :   韦   健
**      Titles  :   这是查找对话框的类，派生于QDialog
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QGroupBox;
class QSpacerItem;
class QDialogButtonBox;

class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindDialog(QWidget *parent=0);
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    unsigned findFlags;
    bool isFirst;
    
signals:
    void FindSignal();
    void ReplaceSignal();
    void ReplaceAllSignal();
    
private slots:
    void EnableFindButton(const QString text);
    void FindForwardClicked();
    void FindBackwardClicked();
    void ReplaceClicked();
    void ReplaceAllClicked();
    void ClearClicked();
    void AdvancedButtonClicked(bool);
    
private:
    QLabel *findLabel;
    QLabel *replaceLabel;
    QPushButton *findForwardButton;
    QPushButton *findBackwardButton;
    QPushButton *closeButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QPushButton *clearButton;
    QPushButton *advancedButton;
    QCheckBox *documentStartCheckBox;
    QCheckBox *matchCaseCheckBox;
    QCheckBox *allWordMatchCheckBox;
    QCheckBox *useRegExpCheckBox;
    QGroupBox *advancedGroupBox;
    QSpacerItem *spacerItem;
    
    void SetTabOrder();
};

//////////////////////////////////////////////////////////////////////////////////////

class RemoveDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RemoveDialog(const QString &, QWidget *parent = 0);
    ~RemoveDialog();
    
signals:
    void OkButtonClickedSignal(const QString &filePath, bool);
    
private slots:
    void OkButtonClicked();
    
private:
    QString filePath;
    QLabel *messageLabel;
    QCheckBox *shiftDeleteCB;
    QDialogButtonBox *buttonBox;
};

#endif // FINDDIALOG_H
