#include "ethernettcpcommunicationconfigwidget.h"
#include "ui_ethernettcpcommunicationconfigwidget.h"

#include "styles.h"

#include "intvalidator.h"

EthernetTcpCommunicationConfigWidget::EthernetTcpCommunicationConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EthernetTcpCommunicationConfigWidget)
{
    ui->setupUi(this);

    ui->groupBox->setStyleSheet(currentGroupBoxTheme);

    ui->ipAddressLineEdit->setStyleSheet(currentLineEditTheme);
    ui->portLineEdit->setStyleSheet(currentLineEditTheme);
    ui->bytesOnCommandLineEdit->setStyleSheet(currentLineEditTheme);
    ui->wastedBytesLineEdit->setStyleSheet(currentLineEditTheme);

    ui->portLineEdit->setValidator(new IntValidator(0,65500,this));
    ui->bytesOnCommandLineEdit->setValidator(new IntValidator(0,64,this));
    ui->wastedBytesLineEdit->setValidator(new IntValidator(0,1000,this));

    connect(ui->ipAddressLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->portLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));

}

EthernetTcpCommunicationConfigWidget::~EthernetTcpCommunicationConfigWidget()
{
    delete ui;
}

bool EthernetTcpCommunicationConfigWidget::populateFromDomElement(const QDomElement &root)
{
    QDomElement element, com;

    com = root.namedItem("CommunicationConfig").toElement();

    if ( com.isNull() )
        return false;

    element = com.namedItem("AddressIP").toElement();
    if ( !element.isNull() )
        ui->ipAddressLineEdit->setText(element.text());

    element = com.namedItem("Port").toElement();
    if ( !element.isNull() )
        ui->portLineEdit->setText(element.text());

    element = com.namedItem("BytesOnCommand").toElement();
    if ( !element.isNull() )
        ui->bytesOnCommandLineEdit->setText(element.text());

    element = com.namedItem("WastedBytes").toElement();
    if ( !element.isNull() )
        ui->wastedBytesLineEdit->setText(element.text());


    return true;
}

void EthernetTcpCommunicationConfigWidget::saveChanges(QDomDocument &dom)
{
    QDomElement element,root,newCom;

    root = dom.documentElement();
    newCom = dom.createElement("CommunicationConfig").toElement();

    element = dom.createElement("AddressIP").toElement();
    element.appendChild(dom.createTextNode(ui->ipAddressLineEdit->text()));
    newCom.appendChild(element);

    element = dom.createElement("Port").toElement();
    element.appendChild(dom.createTextNode(ui->portLineEdit->text()));
    newCom.appendChild(element);

    element = dom.createElement("BytesOnCommand").toElement();
    element.appendChild(dom.createTextNode(ui->bytesOnCommandLineEdit->text()));
    newCom.appendChild(element);

    element = dom.createElement("WastedBytes").toElement();
    element.appendChild(dom.createTextNode(ui->wastedBytesLineEdit->text()));
    newCom.appendChild(element);

    newCom.setAttribute("communication_type", "Ethernet Communication (TCP/IP)");

    root.replaceChild(newCom,root.namedItem("CommunicationConfig"));
}



























