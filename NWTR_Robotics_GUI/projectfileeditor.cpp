#include "projectfileeditor.h"
#include "ui_projectfileeditor.h"

#include "styles.h"

#include <QMessageBox>
#include <QtWidgets>

ProjectFileEditor::ProjectFileEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectFileEditor),
    ethernetTcpCommunicationConfigWidget(NULL),
    serialCommunicationConfigWidget(NULL)
{
    ui->setupUi(this);

    setStyleSheet(currentProjectEditorTheme);

    ui->saveChangesButton->setStyleSheet(currentButtonTheme);

    connect(ui->saveChangesButton,SIGNAL(clicked(bool)),this,SIGNAL(saveRequested()));
}

ProjectFileEditor::~ProjectFileEditor()
{
    delete ui;
}

bool ProjectFileEditor::populateFromString(const QString &data)
{
    QDomElement root, com;
    QString errorStr;
    bool conf1 = false;
    bool conf2 = false;

    if ( !dom.setContent(data,false,&errorStr))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);

        msgBox.setStyleSheet(currentErrorBoxTheme);
        msgBox.exec();

        return false;
    }

    root = dom.documentElement();

    if ( root.tagName() != "Project")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));

        msgBox.setStyleSheet(currentErrorBoxTheme);
        msgBox.exec();

        return false;
    }

    com = root.namedItem("CommunicationConfig").toElement();

    if ( com.isNull() )
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no comunication tag in .pro file."));

        msgBox.setStyleSheet(currentErrorBoxTheme);
        msgBox.exec();

        return false;
    }

    QHBoxLayout* hLayoutComunication = new QHBoxLayout();

    if ( com.attribute("communication_type") == "Serial Communication ( COM )" )
    {
        serialCommunicationConfigWidget = new SerialCommunicationConfigWidget();

        connect(serialCommunicationConfigWidget,SIGNAL(contentChanged()),this,SIGNAL(contentChanged()));

        hLayoutComunication->addWidget(serialCommunicationConfigWidget);
        hLayoutComunication->addStretch(1);
    }
    else if ( com.attribute("communication_type") == "Ethernet Communication (TCP/IP)" )
    {
        ethernetTcpCommunicationConfigWidget = new EthernetTcpCommunicationConfigWidget();

        connect(ethernetTcpCommunicationConfigWidget,SIGNAL(contentChanged()),this,SIGNAL(contentChanged()));

        hLayoutComunication->addWidget(ethernetTcpCommunicationConfigWidget);
        hLayoutComunication->addStretch(1);
    }

    QHBoxLayout* hLayoutConfig = new QHBoxLayout();

    scaraSC1ConfigWidget = new ScaraSC1ConfigWidget();

    connect(scaraSC1ConfigWidget,SIGNAL(contentChanged()),this,SIGNAL(contentChanged()));

    hLayoutConfig->addWidget(scaraSC1ConfigWidget);
    hLayoutConfig->addStretch(1);

    ui->wigetsLayout->addLayout(hLayoutConfig);
    ui->wigetsLayout->addLayout(hLayoutComunication);
    ui->wigetsLayout->addStretch(1);

    if ( scaraSC1ConfigWidget != NULL )
        conf1 = scaraSC1ConfigWidget->populateFromDomElement(root);

    if ( serialCommunicationConfigWidget != NULL )
        conf2 = serialCommunicationConfigWidget->populateFromDomElement(root);
    else if ( ethernetTcpCommunicationConfigWidget != NULL )
        conf2 = ethernetTcpCommunicationConfigWidget->populateFromDomElement(root);

    return conf1 && conf2;
}

QString ProjectFileEditor::toStr()
{
   if ( scaraSC1ConfigWidget != NULL )
       scaraSC1ConfigWidget->saveChanges(dom);

   if ( serialCommunicationConfigWidget != NULL )
       serialCommunicationConfigWidget->saveChanges(dom);
   else if ( ethernetTcpCommunicationConfigWidget != NULL )
       ethernetTcpCommunicationConfigWidget->saveChanges(dom);

   return dom.toString();
}

































