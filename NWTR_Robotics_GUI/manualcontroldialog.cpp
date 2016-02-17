#include "manualcontroldialog.h"
#include "ui_manualcontroldialog.h"

#include "styles.h"

#include <QStringBuilder>
#include <QDebug>
#include <QScrollBar>

ManualControlDialog::ManualControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualControlDialog)
{
    ui->setupUi(this);

    setStyleSheet(currentWindowTheme);

    ui->moveAxisGroupBox->setStyleSheet(currentGroupBoxTheme);
    ui->moveJogGroupBox->setStyleSheet(currentGroupBoxTheme);
    ui->homingGroupBox->setStyleSheet(currentGroupBoxTheme);
    ui->positionGroupBox->setStyleSheet(currentGroupBoxTheme);
    ui->speeedsGroupBox->setStyleSheet(currentGroupBoxTheme);
    ui->connectionGroupBox->setStyleSheet(currentGroupBoxTheme);

    ui->commandLineEdit->setStyleSheet(currentLineEditTheme);
    ui->xLineEdit->setStyleSheet(currentLineEditTheme);
    ui->yLineEdit->setStyleSheet(currentLineEditTheme);
    ui->zLineEdit->setStyleSheet(currentLineEditTheme);

    ui->sendPushButton->setStyleSheet(currentButtonTheme);
    ui->savePointPushButton->setStyleSheet(currentButtonTheme);
    ui->homingFirstSegmentPushButon->setStyleSheet(currentButtonTheme);
    ui->homingSecondSegmentPushButton->setStyleSheet(currentButtonTheme);
    ui->homingThirdSegmentPushButton->setStyleSheet(currentButtonTheme);
    ui->connectPushButton->setStyleSheet(connectButtonsTheme);
    ui->disconnectPushButton->setStyleSheet(disconnectButtonsTheme);

    ui->xMinusPushButton->setStyleSheet(moveButtonsTheme);
    ui->xPlusPushButton->setStyleSheet(moveButtonsTheme);
    ui->yMinusPushButton->setStyleSheet(moveButtonsTheme);
    ui->yPlusPushButton->setStyleSheet(moveButtonsTheme);
    ui->zMinusPushButton->setStyleSheet(moveButtonsTheme);
    ui->zPlusPushButton->setStyleSheet(moveButtonsTheme);
    ui->moveJogLeftFirstSegmentPushButton->setStyleSheet(moveButtonsTheme);
    ui->moveJogRightFirstSegmentPushButton->setStyleSheet(moveButtonsTheme);
    ui->moveJogLeftSecondSegmentPushButton->setStyleSheet(moveButtonsTheme);
    ui->moveJogRightSecondSegmentPushButton->setStyleSheet(moveButtonsTheme);

    ui->pickNPlacePushButton->setStyleSheet(connectButtonsTheme);
    ui->pickNPlacePushButton->setText("Pick");

    connect(ui->sendPushButton,SIGNAL(clicked(bool)),this,SLOT(on_sendPushButton_clicked()));

    ui->disconnectPushButton->hide();
}

ManualControlDialog::~ManualControlDialog()
{
    delete ui;
}

void ManualControlDialog::setTitle(QString title)
{
    m_title = title;

    setWindowTitle(m_title+" - Manual Control");
}

void ManualControlDialog::on_connectPushButton_clicked()
{
    emit establishConnection();
}

void ManualControlDialog::on_disconnectPushButton_clicked()
{
    receiveProjectInfo(QString("Dicsonnecting..."));

    emit dropConnection();
}

void ManualControlDialog::on_moveJogLeftFirstSegmentPushButton_pressed()
{
    emit sendCommand(QString("MJLM2"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJLM2");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogLeftFirstSegmentPushButton_released()
{
    emit sendCommand(QString("RJM2"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM2");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogRightFirstSegmentPushButton_pressed()
{
    emit sendCommand(QString("MJRM2"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJRM2");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogRightFirstSegmentPushButton_released()
{
    emit sendCommand(QString("RJM2"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM2");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogLeftSecondSegmentPushButton_pressed()
{
    emit sendCommand(QString("MJLM3"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJLM3");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogLeftSecondSegmentPushButton_released()
{
    emit sendCommand(QString("RJM3"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM3");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogRightSecondSegmentPushButton_pressed()
{
    emit sendCommand(QString("MJRM3"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJRM3");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_moveJogRightSecondSegmentPushButton_released()
{
    emit sendCommand(QString("RJM3"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM3");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_zPlusPushButton_released()
{
    emit sendCommand(QString("RJM4"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM4");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_zPlusPushButton_pressed()
{
    emit sendCommand(QString("MJLM4"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJLM4");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_zMinusPushButton_pressed()
{
    emit sendCommand(QString("MJRM4"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("MJRM4");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_zMinusPushButton_released()
{
    emit sendCommand(QString("RJM4"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("RJM4");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::receiveCommand(QString command)
{
    QString text = deviceHTMLFormat % QString("["+m_title+"]:");

    ui->commandPromptTextEdit->insertHtml(text);

    text = dataHTMLFormat % command;
    ui->commandPromptTextEdit->insertHtml(text);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::receiveProjectInfo(QString info)
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

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_sendPushButton_clicked()
{
    if ( ui->commandLineEdit->text().isEmpty() )
        return;

    emit sendCommand(ui->commandLineEdit->text());

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % ui->commandLineEdit->text();
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);

    ui->commandLineEdit->clear();
}

void ManualControlDialog::on_homingFirstSegmentPushButon_clicked()
{
    emit sendCommand(QString("HOMINGM2"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("HOMINGM2");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_homingSecondSegmentPushButton_clicked()
{
    emit sendCommand(QString("HOMINGM3"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("HOMINGM3");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}

void ManualControlDialog::on_homingThirdSegmentPushButton_clicked()
{
    emit sendCommand(QString("HOMINGM4"));

    QString toSend = ownTextHTMLFormat % ">>> ";

    ui->commandPromptTextEdit->insertHtml(toSend);

    toSend = dataHTMLFormat % QString("HOMINGM4");
    ui->commandPromptTextEdit->insertHtml(toSend);
    ui->commandPromptTextEdit->insertPlainText("\n");

    QTextCursor cursor = ui->commandPromptTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->commandPromptTextEdit->setTextCursor(cursor);
}
