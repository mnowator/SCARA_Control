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
    ui->doNotSaveButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setStyleSheet(currentButtonTheme);
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

QStringList SaveChangesDialog::getSelectedFiles()
{
    QStringList selectedFiles;

    for ( unsigned idx=0; idx<ui->tableWidget->selectionModel()->selectedRows().count(); ++idx)
        selectedFiles << ui->tableWidget->takeItem(idx,0)->text();

    return selectedFiles;
}

int SaveChangesDialog::exec()
{
    ui->tableWidget->selectAll();

    return QDialog::exec();
}

void SaveChangesDialog::selectionChanged()
{

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


