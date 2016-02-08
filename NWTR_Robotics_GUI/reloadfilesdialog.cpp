#include "reloadfilesdialog.h"
#include "ui_reloadfilesdialog.h"

#include "styles.h"

ReloadFilesDialog::ReloadFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReloadFilesDialog)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    connect(ui->reloadButton,SIGNAL(clicked(bool)),this,SLOT(emitReloadFilesSignalSlot()));

    ui->reloadButton->setStyleSheet(currentButtonTheme);
    ui->reloadButton->setMinimumWidth(80);
    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setMinimumWidth(60);
}

ReloadFilesDialog::~ReloadFilesDialog()
{
    delete ui;
}

void ReloadFilesDialog::addFile(const QIcon &icon, const QString &path, const QString &fileName)
{
    QTableWidgetItem* fileItem = new QTableWidgetItem(icon,fileName);
    QTableWidgetItem* pathItem = new QTableWidgetItem(path);

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,fileItem);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,pathItem);

    ui->tableWidget->selectAll();
}

void ReloadFilesDialog::showEvent(QShowEvent *event)
{
    this->setFocus();

    ui->tableWidget->selectAll();

    connect(ui->tableWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged()));


    QDialog::showEvent(event);
}

void ReloadFilesDialog::emitReloadFilesSignalSlot()
{
    QList<QPair<QString, QString> > selectedFiles;

    for ( unsigned idx=0; idx<ui->tableWidget->selectionModel()->selectedRows().count(); ++idx)
    {
        selectedFiles.append(qMakePair(ui->tableWidget->takeItem(idx,0)->text(),
                                       ui->tableWidget->takeItem(idx,1)->text()));
    }

    emit reloadFiles(selectedFiles);

    this->done(true);
}

void ReloadFilesDialog::selectionChanged()
{
    ui->reloadButton->setText("Reload selected");
    ui->reloadButton->setMinimumWidth(95);
}



















