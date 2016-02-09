#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include "styles.h"

#include <QDir>
#include <QFileDialog>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    ui->cancelButton->setStyleSheet(currentButtonTheme);
    ui->cancelButton->setMinimumWidth(50);
    ui->applyPushButton->setStyleSheet(currentButtonTheme);
    ui->applyPushButton->setMinimumWidth(50);
    ui->saveChangesPushButton->setStyleSheet(currentButtonTheme);
    ui->saveChangesPushButton->setMinimumWidth(90);
    ui->browseButton->setStyleSheet(currentButtonTheme);
    ui->browseButton->setMinimumWidth(50);

    ui->themeComboBox->setStyleSheet(currentComboBoxTheme);
    ui->languageComboBox->setStyleSheet(currentComboBoxTheme);

    ui->defaultProjectPathLineEdit->setStyleSheet(currentLineEditTheme);

    ui->tabWidget->setStyleSheet(currentTabWidgetTheme);

    ui->defaultProjectPathLineEdit->setText(QDir::homePath());

    connect(ui->browseButton,SIGNAL(clicked(bool)),this,SLOT(browseClicked()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::browseClicked()
{
    QFileDialog folderDialog;

    folderDialog.setFileMode(QFileDialog::Directory);
    folderDialog.setOption(QFileDialog::ShowDirsOnly);

    if ( folderDialog.exec() )
    {
        ui->defaultProjectPathLineEdit->setText(folderDialog.selectedFiles()[0]);
    }
}
