/****************************************************************************
**
**      Datetime:   2013年04月21日 星期日 14时37分53秒
**      Author  :   韦   健
**      Titles  :   新建文件或项目的导航类，派生于QWizard
**
**      Guangxi Normal University 2013
**
****************************************************************************/
#ifndef NEWWIZARD_H
#define NEWWIZARD_H

#include <QWizard>

class QWizardPage;
class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;

class NewWizard : public QWizard
{
    Q_OBJECT
public:
    explicit NewWizard(int type = 0, const QString& fileDir = "", QWidget *parent = 0, Qt::WindowFlags flag = Qt::WindowType::Widget);
    ~NewWizard();

signals:
    void FinishButtonClickedSignal(const QString &createFileFullPath, int projectType);

private slots:
    void LeftListItemChanged(int row);
    void CurrentPageChanged(int page);
    void BrowsePushButtonClicked();
    void FileNameLineEditTextChanged(const QString &fileN);
    void FinishButtonClicked();
    void SetFullPathMessage();

private:
    QStringList execFileType;
    QStringList execProjectType;
    QStringList leftList;

    int fileTypes;
    QString execName;

    QWizardPage *firstWizardPage;
    QWizardPage *secondWizardPage;
    QWizardPage *thirdWizardPage;

    QListWidget *leftListWidget;
    QListWidget *rightListWidget;


    QLabel *fileMessage;
    QLabel *fullPathMessage;
    QLabel *filePathLabel;
    QLineEdit *fileNameLineEdit;
    QLineEdit *filePathLineEdit;
    QPushButton *browsePushButton;
};

#endif // NEWWIZARD_H
