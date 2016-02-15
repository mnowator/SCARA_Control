#include "commandprompt.h"
#include "ui_commandprompt.h"

#include "styles.h"

#include <QStringBuilder>
#include <QDebug>

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
    ui->connectPushButton->setStyleSheet(connectButtonsTheme);
    ui->connectPushButton->setMinimumWidth(80);
    ui->disconnectPushButton->setStyleSheet(disconnectButtonsTheme);
    ui->disconnectPushButton->setMinimumWidth(80);

    connect(ui->sendPushButton,SIGNAL(clicked(bool)),this,SLOT(sendButtonClicked()));
    connect(ui->connectPushButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonClicked()));
    connect(ui->disconnectPushButton,SIGNAL(clicked(bool)),this,SLOT(disconnectButtonClicked()));

    ui->disconnectPushButton->hide();
}

CommandPrompt::~CommandPrompt()
{
    disconnectButtonClicked();

    delete ui;
}

void CommandPrompt::setTitle(QString title)
{
    m_title = title;

    setWindowTitle(m_title+" - Command Prompt");
}

void CommandPrompt::setProjectPointer(Project *project)
{
    m_project = project;

    if ( project->projectState() == Project::ControlledByScript )
    {
        ui->connectPushButton->hide();
        ui->disconnectPushButton->hide();
    }
}

void CommandPrompt::receiveCommand(QString command)
{
    QString text = deviceHTMLFormat % QString("["+m_title+"]:");

    ui->commandPromptTextEdit->insertHtml(text);

    text = dataHTMLFormat % command;
    ui->commandPromptTextEdit->insertHtml(text);
    ui->commandPromptTextEdit->insertPlainText("\n");
}

void CommandPrompt::receiveProjectInfo(QString info)
{
    if ( info == "Connected to host.")
    {
        ui->connectPushButton->hide();
        ui->disconnectPushButton->show();
    }
    else if ( info == "Disconnected from host.")
    {
        ui->connectPushButton->show();
        ui->disconnectPushButton->hide();
    }


    QString text = infoHTMLFormat % info;

    ui->commandPromptTextEdit->insertHtml(text);
    ui->commandPromptTextEdit->insertPlainText("\n");
}

void CommandPrompt::projectChangeStateSlot(Project::ProjectState state)
{
    if ( state == Project::ControlledByScript )
    {
        ui->connectPushButton->hide();
        ui->disconnectPushButton->hide();
    }
    else if ( state == Project::Idle )
    {
        ui->connectPushButton->show();
        ui->disconnectPushButton->show();
    }
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

void CommandPrompt::connectButtonClicked()
{
    emit establishConnection();
}

void CommandPrompt::disconnectButtonClicked()
{
    receiveProjectInfo(QString("Dicsonnecting..."));

    emit dropConnection();
}

















