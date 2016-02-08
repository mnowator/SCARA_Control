#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>


namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = 0);
    ~NewFileDialog();

    QStringList retrieveData() const;

private:
    Ui::NewFileDialog *ui;

private slots:
    void createClicked();
    void cancelClicked();
};

#endif // NEWFILEDIALOG_H
