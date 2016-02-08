#ifndef REMOVEFILEDIALOG_H
#define REMOVEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class RemoveFileDialog;
}

class RemoveFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveFileDialog(QWidget *parent = 0);
    ~RemoveFileDialog();

    void setFile(QString const& file);
    bool deletePermanently() const;

private:
    Ui::RemoveFileDialog *ui;
};

#endif // REMOVEFILEDIALOG_H
