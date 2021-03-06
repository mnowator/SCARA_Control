#ifndef RENAMEFILEDIALOG_H
#define RENAMEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameFileDialog;
}

class RenameFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameFileDialog(QWidget *parent = 0);
    ~RenameFileDialog();

    void setFileName(QString const& fileName);

    QString getFileName() const;

private:
    Ui::RenameFileDialog *ui;
};

#endif // RENAMEFILEDIALOG_H
