#ifndef RELOADFILESDIALOG_H
#define RELOADFILESDIALOG_H

#include <QDialog>

namespace Ui {
class ReloadFilesDialog;
}

class ReloadFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReloadFilesDialog(QWidget *parent = 0);
    ~ReloadFilesDialog();

    void addFile(const QIcon &icon, QString const& path, QString const& fileName);

private:
    Ui::ReloadFilesDialog *ui;

    void emitReloadFilesSignal();

protected:
    virtual void showEvent(QShowEvent * event);

signals:
    void reloadFiles(QList<QPair<QString, QString> > listOfFiles);

private slots:
    void emitReloadFilesSignalSlot();
    void selectionChanged();
};

#endif // RELOADFILESDIALOG_H
