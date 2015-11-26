#include "removefiledialog.h"
#include "ui_removefiledialog.h"

#include "styles.h"

RemoveFileDialog::RemoveFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveFileDialog)
{
    ui->setupUi(this);

    setStyleSheet(currentWindowTheme);

    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->removeButton->setStyleSheet(currentButtonTheme);
}

RemoveFileDialog::~RemoveFileDialog()
{
    delete ui;
}

void RemoveFileDialog::setFile(const QString &file)
{
    ui->fileLabel->setText(file);
}

bool RemoveFileDialog::deletePermanently() const
{
    return ui->deletePermanentlyCheckBox->isChecked();
}
