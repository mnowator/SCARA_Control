#include "serialcommunicationconfigwidget.h"
#include "ui_serialcommunicationconfigwidget.h"

#include "styles.h"

SerialCommunicationConfigWidget::SerialCommunicationConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialCommunicationConfigWidget)
{
    ui->setupUi(this);

    ui->comLineEdit->setStyleSheet(currentLineEditTheme);
    ui->baudLineEdit->setStyleSheet(currentLineEditTheme);
    ui->dataBitsLineedit->setStyleSheet(currentLineEditTheme);
    ui->stopBitsLineEdit->setStyleSheet(currentLineEditTheme);

    ui->parityComboBox->setStyleSheet(currentComboBoxTheme);
    ui->parityComboBox->setEditable(true);
    ui->parityComboBox->lineEdit()->setAlignment(Qt::AlignRight);
    ui->flowControlComboBox->setStyleSheet(currentComboBoxTheme);
    ui->flowControlComboBox->setEditable(true);
    ui->flowControlComboBox->lineEdit()->setAlignment(Qt::AlignRight);

    ui->groupBox->setStyleSheet(currentGroupBoxTheme);
}

SerialCommunicationConfigWidget::~SerialCommunicationConfigWidget()
{
    delete ui;
}
