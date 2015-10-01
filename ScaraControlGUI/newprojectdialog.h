#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();

private:
    Ui::NewProjectDialog *ui;

signals:
    void createProjectSignal(QString projectName, QString communicationType, QString projectPath);

private slots:
    void browseButtonClicked();
    void createButtonClicked();
};

#endif // NEWPROJECTDIALOG_H
