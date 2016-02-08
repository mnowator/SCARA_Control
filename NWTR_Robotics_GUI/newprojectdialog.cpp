#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

#include "styles.h"

#include <QFileDialog>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);

    connect(ui->browseButton,SIGNAL(clicked(bool)),this,SLOT(browseButtonClicked()));
    connect(ui->createButton,SIGNAL(clicked(bool)),this,SLOT(createButtonClicked()));

    ui->projectPathLineEdit->setText(QDir::homePath()+tr("/NWRT Robotics/"));

    ui->browseButton->setStyleSheet(currentButtonTheme);
    ui->browseButton->setMinimumWidth(60);
    ui->createButton->setStyleSheet(currentButtonTheme);
    ui->createButton->setMinimumWidth(60);
    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setMinimumWidth(60);

    ui->projectNameLineEdit->setStyleSheet(currentLineEditTheme);
    ui->projectPathLineEdit->setStyleSheet(currentLineEditTheme);

    ui->communicationTypeComboBox->setStyleSheet(currentComboBoxTheme);
    ui->projectTypeComboBox->setStyleSheet(currentComboBoxTheme);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::browseButtonClicked()
{
    QFileDialog folderDialog;

    folderDialog.setFileMode(QFileDialog::Directory);
    folderDialog.setOption(QFileDialog::ShowDirsOnly);

    if ( folderDialog.exec() )
    {
        ui->projectPathLineEdit->setText(folderDialog.selectedFiles()[0]);
    }
}

void NewProjectDialog::createButtonClicked()
{
    emit createProjectSignal(ui->projectNameLineEdit->text(),
                             ui->communicationTypeComboBox->currentText(),
                             ui->projectPathLineEdit->text(),
                             ui->projectTypeComboBox->currentText());
}






















