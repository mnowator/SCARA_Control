#include "renamefiledialog.h"
#include "ui_renamefiledialog.h"

#include "styles.h"

RenameFileDialog::RenameFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameFileDialog)
{
    ui->setupUi(this);

    ui->renameButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setStyleSheet(currentButtonTheme);

    ui->filename->setStyleSheet(currentLineEditTheme);
}

RenameFileDialog::~RenameFileDialog()
{
    delete ui;
}

void RenameFileDialog::setFilename(const QString &filename)
{
    ui->filename->setText(filename);
}

QString RenameFileDialog::getFilename() const
{
    return ui->filename->text();
}
