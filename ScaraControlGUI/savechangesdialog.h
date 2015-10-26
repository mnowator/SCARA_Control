#ifndef SAVECHANGESDIALOG_H
#define SAVECHANGESDIALOG_H

#include <QDialog>

namespace Ui {
class SaveChangesDialog;
}

class SaveChangesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveChangesDialog(QWidget *parent = 0);
    ~SaveChangesDialog();

    void addFile(QIcon const& icon, QString const& fileName, QString const& filePath);
    QList<QString> getSelectedFiles();

public slots:
    virtual int exec();

private:
    Ui::SaveChangesDialog *ui;

private slots:
    void selectionChanged();
    void saveButtonClicked();
    void doNotSave();
};

#endif // SAVECHANGESDIALOG_H
