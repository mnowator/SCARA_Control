#include "newfiledialog.h"
#include "ui_newfiledialog.h"

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);

    connect(ui->createButton,SIGNAL(clicked(bool)),this,SLOT(createClicked()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelClicked()));
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}

QStringList NewFileDialog::retrieveData() const
{
    return QStringList() << ui->fileName->text() << ui->fileTypeComboBox->currentText();
}

void NewFileDialog::createClicked()
{
    done(true);
}

void NewFileDialog::cancelClicked()
{
    close();
}
