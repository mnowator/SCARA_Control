#include "projectfileeditor.h"
#include "ui_projectfileeditor.h"

#include "styles.h"

#include <QMessageBox>

ProjectFileEditor::ProjectFileEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectFileEditor)
{
    ui->setupUi(this);

    //setStyleSheet(currentWindowTheme);

    ui->saveChangesButton->setStyleSheet(currentButtonTheme);

    QHBoxLayout* hLayoutComunication = new QHBoxLayout();
    QHBoxLayout* hLayoutConfig = new QHBoxLayout();

    serialCommunicationConfigWidget = new SerialCommunicationConfigWidget();
    scaraSC1ConfigWidget = new ScaraSC1ConfigWidget();

    hLayoutComunication->addWidget(serialCommunicationConfigWidget);
    hLayoutComunication->addStretch(1);

    hLayoutConfig->addWidget(scaraSC1ConfigWidget);
    hLayoutConfig->addStretch(1);

    ui->wigetsLayout->addLayout(hLayoutConfig);
    ui->wigetsLayout->addLayout(hLayoutComunication);
    ui->wigetsLayout->addStretch(1);
}

ProjectFileEditor::~ProjectFileEditor()
{
    delete ui;
}

bool ProjectFileEditor::populateFromString(const QString &data)
{
    QDomDocument dom;
    QDomElement root;
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

    if ( scaraSC1ConfigWidget != NULL )
        conf1 = scaraSC1ConfigWidget->populateFromDomElement(root);

    if ( serialCommunicationConfigWidget != NULL )
        conf2 = serialCommunicationConfigWidget->populateFromDomElement(root);

    return conf1 && conf2;
}


































