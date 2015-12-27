#include "projectfileeditor.h"
#include "ui_projectfileeditor.h"

#include "styles.h"

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
