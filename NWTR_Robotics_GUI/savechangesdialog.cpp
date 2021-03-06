#include "savechangesdialog.h"
#include "ui_savechangesdialog.h"

#include "styles.h"

SaveChangesDialog::SaveChangesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveChangesDialog)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    connect(ui->saveButton,         SIGNAL(clicked(bool)),this,SLOT(saveButtonClicked()));
    connect(ui->doNotSaveButton,    SIGNAL(clicked(bool)),this,SLOT(doNotSave()));

    ui->saveButton->setStyleSheet(currentButtonTheme);
    ui->saveButton->setMinimumWidth(60);
    ui->doNotSaveButton->setStyleSheet(currentButtonTheme);
    ui->doNotSaveButton->setMinimumWidth(75);
    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setMinimumWidth(50);
}

SaveChangesDialog::~SaveChangesDialog()
{
    delete ui;
}

void SaveChangesDialog::addFile(const QIcon &icon, const QString &fileName, const QString &filePath)
{
    QTableWidgetItem* fileItem = new QTableWidgetItem(icon,fileName);
    QTableWidgetItem* pathItem = new QTableWidgetItem(filePath);

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,fileItem);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,pathItem);
}

QList<QPair<QString, QString> > SaveChangesDialog::getSelectedFiles()
{
    QList<QPair<QString, QString> > selectedFiles;

    for ( unsigned idx=0; idx<ui->tableWidget->selectionModel()->selectedRows().count(); ++idx)
    {
        selectedFiles.append(qMakePair(ui->tableWidget->takeItem(idx,0)->text(),
                                       ui->tableWidget->takeItem(idx,1)->text()));
    }

    return selectedFiles;
}

int SaveChangesDialog::exec()
{
    ui->tableWidget->selectAll();

    connect(ui->tableWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged()));

    return QDialog::exec();
}

void SaveChangesDialog::selectionChanged()
{
    ui->saveButton->setText(tr("Save selected"));
    ui->saveButton->setMinimumWidth(80);
}

void SaveChangesDialog::saveButtonClicked()
{
    done(true);
}

void SaveChangesDialog::doNotSave()
{
    ui->tableWidget->clearSelection();

    done(true);
}


