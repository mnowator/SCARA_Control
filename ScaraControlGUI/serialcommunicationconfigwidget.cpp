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
    ui->dataBitsLineEdit->setStyleSheet(currentLineEditTheme);
    ui->stopBitsLineEdit->setStyleSheet(currentLineEditTheme);

    ui->parityComboBox->setStyleSheet(currentComboBoxTheme);
    ui->parityComboBox->setEditable(true);
    ui->parityComboBox->lineEdit()->setAlignment(Qt::AlignRight);
    ui->flowControlComboBox->setStyleSheet(currentComboBoxTheme);
    ui->flowControlComboBox->setEditable(true);
    ui->flowControlComboBox->lineEdit()->setAlignment(Qt::AlignRight);

    ui->baudLineEdit->setValidator(new QIntValidator(0,30000,this));
    ui->dataBitsLineEdit->setValidator(new QIntValidator(0,30000,this));
    ui->stopBitsLineEdit->setValidator(new QIntValidator(0,30000,this));

    ui->groupBox->setStyleSheet(currentGroupBoxTheme);
}

SerialCommunicationConfigWidget::~SerialCommunicationConfigWidget()
{
    delete ui;
}

bool SerialCommunicationConfigWidget::populateFromDomElement(QDomElement const& root)
{
    QDomElement element, com;

    com = root.namedItem("CommunicationConfig").toElement();

    if ( com.isNull() )
        return false;

    element = com.namedItem("SerialLine").toElement();
    if ( !element.isNull() )
        ui->comLineEdit->setText(element.text());

    element = com.namedItem("BaudRate").toElement();
    if ( !element.isNull() )
        ui->baudLineEdit->setText(element.text());

    element = com.namedItem("DataBits").toElement();
    if ( !element.isNull() )
        ui->dataBitsLineEdit->setText(element.text());

    element = com.namedItem("StopBits").toElement();
    if ( !element.isNull() )
        ui->stopBitsLineEdit->setText(element.text());

    element = com.namedItem("Parity").toElement();
    if ( !element.isNull() )
        for ( unsigned i=0; i<ui->parityComboBox->count(); ++i )
            if ( ui->parityComboBox->itemText(i) == element.text() )
                ui->parityComboBox->setCurrentIndex(i);

    element = com.namedItem("FlowControl").toElement();
    if ( !element.isNull() )
        for ( unsigned i=0; i<ui->flowControlComboBox->count(); ++i )
            if ( ui->flowControlComboBox->itemText(i) == element.text() )
                ui->flowControlComboBox->setCurrentIndex(i);

    return true;
}





















