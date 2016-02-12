#include "commandprompt.h"
#include "ui_commandprompt.h"

#include "styles.h"

#include <QStringBuilder>

CommandPrompt::CommandPrompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommandPrompt)
{
    ui->setupUi(this);

    setStyleSheet(currentWindowTheme);

    ui->commandLineEdit->setStyleSheet(currentLineEditTheme);
    ui->dataFormatComboBox->setStyleSheet(currentComboBoxTheme);
    ui->commandPromptTextEdit->setStyleSheet(currentCodeEditorTheme);
    ui->sendPushButton->setStyleSheet(currentButtonTheme);
    ui->sendPushButton->setMinimumWidth(70);

    connect(ui->sendPushButton,SIGNAL(clicked(bool)),this,SLOT(sendButtonClicked()));
}

CommandPrompt::~CommandPrompt()
{
    delete ui;
}

void CommandPrompt::receiveCommand(QString command)
{

}

void CommandPrompt::receiveProjectInfo(QString info)
{

}

void CommandPrompt::sendButtonClicked()
{
    if ( ui->commandLineEdit->text().isEmpty() )
        return;

    emit sendCommand(ui->commandLineEdit->text());

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % ui->commandLineEdit->text();
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    ui->commandLineEdit->clear();
}
