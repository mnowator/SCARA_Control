#include "newfiledialog.h"
#include "ui_newfiledialog.h"

#include "styles.h"

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);

    connect(ui->createButton,SIGNAL(clicked(bool)),this,SLOT(createClicked()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelClicked()));

    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setMinimumWidth(60);
    ui->createButton->setStyleSheet(currentButtonTheme);
    ui->createButton->setMinimumWidth(60);

    ui->fileTypeComboBox->setStyleSheet(currentComboBoxTheme);

    ui->fileName->setStyleSheet(currentLineEditTheme);
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
