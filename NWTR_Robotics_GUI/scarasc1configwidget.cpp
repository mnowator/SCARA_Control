#include "scarasc1configwidget.h"
#include "ui_scarasc1configwidget.h"

#include "styles.h"

#include "intvalidator.h"
#include "doublevalidator.h"

ScaraSC1ConfigWidget::ScaraSC1ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScaraSC1ConfigWidget)
{
    ui->setupUi(this);

    ui->lengthOfFirstSegmentLineEdit->setStyleSheet(currentLineEditTheme);
    ui->lengthOfSecondSegmentLineEdit->setStyleSheet(currentLineEditTheme);
    ui->lengthOfThirdSegmentLineEdit->setStyleSheet(currentLineEditTheme);
    ui->correctionValueLineEdit->setStyleSheet(currentLineEditTheme);
    ui->firstSegmentLimitAngleOnCWLineEdit->setStyleSheet(currentLineEditTheme);
    ui->firstSegmentLimitAngleOnCCWLineEdit->setStyleSheet(currentLineEditTheme);
    ui->secondSegmentLimitAngleOnCWLineEdit->setStyleSheet(currentLineEditTheme);
    ui->secondSegmentLimitAngleOnCCWLineEdit->setStyleSheet(currentLineEditTheme);
    ui->numberOfStepsForFirstSegmentLineEdit->setStyleSheet(currentLineEditTheme);
    ui->numberOfStepsForSecondSegmentLineEdit->setStyleSheet(currentLineEditTheme);
    ui->numberOfStepsBetweenLimitsOnZLineEdit->setStyleSheet(currentLineEditTheme);
    ui->distanceBetweenLimintsOnZLineEdit->setStyleSheet(currentLineEditTheme);
    ui->syncFreqLineEdit->setStyleSheet(currentLineEditTheme);

    ui->mainProgramComboBox->setStyleSheet(currentComboBoxTheme);
    ui->mainPointsFileComboBox->setStyleSheet(currentComboBoxTheme);
    ui->scara_sc1_config->setStyleSheet(currentGroupBoxTheme);

    ui->lengthOfFirstSegmentLineEdit->setValidator(new DoubleValidator(30,999999,10,ui->lengthOfFirstSegmentLineEdit));
    ui->lengthOfSecondSegmentLineEdit->setValidator(new DoubleValidator(30,999999,10,this));
    ui->lengthOfThirdSegmentLineEdit->setValidator(new DoubleValidator(31,999999,10,this));
    ui->correctionValueLineEdit->setValidator(new DoubleValidator(0,999999,10,this));
    ui->firstSegmentLimitAngleOnCWLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->firstSegmentLimitAngleOnCCWLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->secondSegmentLimitAngleOnCWLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->secondSegmentLimitAngleOnCCWLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->numberOfStepsForFirstSegmentLineEdit->setValidator(new IntValidator(0,999999,this));
    ui->numberOfStepsForSecondSegmentLineEdit->setValidator(new IntValidator(0,999999,this));
    ui->numberOfStepsBetweenLimitsOnZLineEdit->setValidator(new IntValidator(30,999999,this));
    ui->syncFreqLineEdit->setValidator(new IntValidator(1,999999,this));

    QGraphicsScene* sceneXY = new QGraphicsScene(this);
    QGraphicsScene* sceneZ = new QGraphicsScene(this);

    sceneXY->addPixmap(QPixmap(":/new/icons/scaraXYSurface.png"));
    sceneZ->addPixmap(QPixmap(":/new/icons/scaraZSurface.png"));

    ui->scaraXYSurfaceGraphicsView->setScene(sceneXY);
    ui->scaraZSurfaceGraphicsView->setScene(sceneZ);

    connect(ui->firstSegmentBeginOnCCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(firstSegmentBeginOnCCWClicked()));
    connect(ui->firstSegmentBeginOnCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(firstSegmentBeginOnCWClicked()));
    connect(ui->secondSegmentBeginOnCCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(secondSegmentBeginOnCCWClicked()));
    connect(ui->secondSegmentBeginOnCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(secondSegmentBeginOnCWClicked()));
    connect(ui->thirdSegmentBeginOnCCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(thirdSegmentBeginOnCCWClicked()));
    connect(ui->thirdSegmentBeginOnCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(thirdSegmentBeginOnCWClicked()));
    connect(ui->increasingZonCCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(increasingZonCCWClicked()));
    connect(ui->increasingZonCWRadioButton,SIGNAL(clicked(bool)),this,SLOT(increasingZonCWClicked()));

    connect(ui->lengthOfFirstSegmentLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->lengthOfSecondSegmentLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->lengthOfThirdSegmentLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->correctionValueLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->firstSegmentLimitAngleOnCCWLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->firstSegmentLimitAngleOnCWLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->secondSegmentLimitAngleOnCCWLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->secondSegmentLimitAngleOnCWLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->numberOfStepsBetweenLimitsOnZLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->numberOfStepsForFirstSegmentLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->numberOfStepsForSecondSegmentLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    connect(ui->syncFreqLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));

    connect(ui->mainProgramComboBox,SIGNAL(currentIndexChanged(int)),this,SIGNAL(contentChanged()));
    connect(ui->mainPointsFileComboBox,SIGNAL(currentIndexChanged(int)),this,SIGNAL(contentChanged()));
}

ScaraSC1ConfigWidget::~ScaraSC1ConfigWidget()
{
    delete ui;
}

bool ScaraSC1ConfigWidget::populateFromDomElement(QDomElement const& root)
{
    QDomElement element;

    ui->mainPointsFileComboBox->clear();
    ui->mainProgramComboBox->clear();

    element = root.namedItem("Files").toElement();
    if ( !element.isNull() )
        for ( QDomElement file = element.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File") )
        {
            QDomElement name = file.namedItem("Name").toElement();

            if ( !name.isNull() )
            {
                if ( name.text().endsWith(".js") )
                    ui->mainProgramComboBox->addItem(QIcon(":/new/icons/pythonfile.png"),name.text());
                else if ( name.text().endsWith(".pt") )
                    ui->mainPointsFileComboBox->addItem(QIcon(":/new/icons/pointsfile.png"),name.text());
            }
        }

    element = root.namedItem("MainProgramFile").toElement();
    if ( !element.isNull())
        for ( unsigned i=0; i<ui->mainProgramComboBox->count(); ++i )
            if ( ui->mainProgramComboBox->itemText(i) == element.text() )
                ui->mainProgramComboBox->setCurrentIndex(i);

    element = root.namedItem("MainPointsFile").toElement();
    if ( !element.isNull())
        for ( unsigned i=0; i<ui->mainPointsFileComboBox->count(); ++i )
            if ( ui->mainPointsFileComboBox->itemText(i) == element.text() )
                ui->mainPointsFileComboBox->setCurrentIndex(i);

    element = root.namedItem("SyncFreq").toElement();
    if ( !element.isNull())
        ui->syncFreqLineEdit->setText(element.text());

    element = root.namedItem("LengthOfFirstSegment").toElement();
    if ( !element.isNull() )
        ui->lengthOfFirstSegmentLineEdit->setText(element.text());

    element = root.namedItem("LengthOfSecondSegment").toElement();
    if ( !element.isNull() )
        ui->lengthOfSecondSegmentLineEdit->setText(element.text());

    element = root.namedItem("CorrectionValue").toElement();
    if ( !element.isNull() )
        ui->correctionValueLineEdit->setText(element.text());

    element = root.namedItem("FirstSegmentAngleOnCw").toElement();
    if ( !element.isNull() )
        ui->firstSegmentLimitAngleOnCWLineEdit->setText(element.text());

    element = root.namedItem("FirstSegmentAngleOnCCW").toElement();
    if ( !element.isNull() )
        ui->firstSegmentLimitAngleOnCCWLineEdit->setText(element.text());

    element = root.namedItem("NumberOfStepsForFirstSegment").toElement();
    if ( !element.isNull() )
        ui->numberOfStepsForFirstSegmentLineEdit->setText(element.text());

    element = root.namedItem("SecondSegmentAngleOnCw").toElement();
    if ( !element.isNull() )
        ui->secondSegmentLimitAngleOnCWLineEdit->setText(element.text());

    element = root.namedItem("SecondSegmentAngleOnCCW").toElement();
    if ( !element.isNull() )
        ui->secondSegmentLimitAngleOnCCWLineEdit->setText(element.text());

    element = root.namedItem("NumberOfStepsForSecondSegment").toElement();
    if ( !element.isNull() )
        ui->numberOfStepsForSecondSegmentLineEdit->setText(element.text());

    element = root.namedItem("FirstSegmentBeginOn").toElement();
    if ( !element.isNull() )
    {
        if ( element.text() == "CW" )
        {
            ui->firstSegmentBeginOnCWRadioButton->setChecked(true);
            ui->firstSegmentBeginOnCCWRadioButton->setChecked(false);
        }
        else if ( element.text() == "CCW" )
        {
            ui->firstSegmentBeginOnCWRadioButton->setChecked(false);
            ui->firstSegmentBeginOnCCWRadioButton->setChecked(true);
        }
    }

    element = root.namedItem("SecondSegmentBeginOn").toElement();
    if ( !element.isNull() )
    {
        if ( element.text() == "CW" )
        {
            ui->secondSegmentBeginOnCWRadioButton->setChecked(true);
            ui->secondSegmentBeginOnCCWRadioButton->setChecked(false);
        }
        else if ( element.text() == "CCW" )
        {
            ui->secondSegmentBeginOnCWRadioButton->setChecked(false);
            ui->secondSegmentBeginOnCCWRadioButton->setChecked(true);
        }
    }

    element = root.namedItem("DistanceBetweenTwoLimtsOnZ").toElement();
    if ( !element.isNull() )
        ui->distanceBetweenLimintsOnZLineEdit->setText(element.text());

    element = root.namedItem("LengthOfThirdSegment").toElement();
    if ( !element.isNull() )
        ui->lengthOfThirdSegmentLineEdit->setText(element.text());

    element = root.namedItem("NumberOfStepsBetweenLimitsOnZ").toElement();
    if ( !element.isNull() )
        ui->numberOfStepsBetweenLimitsOnZLineEdit->setText(element.text());

    element = root.namedItem("IncreasingZOn").toElement();
    if ( !element.isNull() )
    {
        if ( element.text() == "CW" )
        {
            ui->increasingZonCWRadioButton->setChecked(true);
            ui->increasingZonCCWRadioButton->setChecked(false);
        }
        else if ( element.text() == "CCW" )
        {
            ui->increasingZonCWRadioButton->setChecked(false);
            ui->increasingZonCCWRadioButton->setChecked(true);
        }
    }

    element = root.namedItem("ThirdSegmentBeginOn").toElement();
    if ( !element.isNull() )
    {
        if ( element.text() == "CW" )
        {
            ui->thirdSegmentBeginOnCWRadioButton->setChecked(true);
            ui->thirdSegmentBeginOnCCWRadioButton->setChecked(false);
        }
        else if ( element.text() == "CCW" )
        {
            ui->thirdSegmentBeginOnCWRadioButton->setChecked(false);
            ui->thirdSegmentBeginOnCCWRadioButton->setChecked(true);
        }
    }

    return true;
}

void ScaraSC1ConfigWidget::saveChanges(QDomDocument &dom)
{
    QDomElement newElement, root;

    root = dom.documentElement();

    newElement = dom.createElement("MainProgramFile");
    newElement.appendChild(dom.createTextNode(ui->mainProgramComboBox->currentText()));
    root.appendChild(newElement);
    root.replaceChild(newElement,root.namedItem("MainProgramFile"));

    newElement = dom.createElement("MainPointsFile");
    newElement.appendChild(dom.createTextNode(ui->mainPointsFileComboBox->currentText()));
    root.appendChild(newElement);
    root.replaceChild(newElement,root.namedItem("MainPointsFile"));

    newElement = dom.createElement("SyncFreq");
    newElement.appendChild(dom.createTextNode(ui->syncFreqLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("SyncFreq"));

    newElement = dom.createElement("LengthOfFirstSegment");
    newElement.appendChild(dom.createTextNode(ui->lengthOfFirstSegmentLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("LengthOfFirstSegment"));

    newElement = dom.createElement("LengthOfSecondSegment");
    newElement.appendChild(dom.createTextNode(ui->lengthOfSecondSegmentLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("LengthOfSecondSegment"));

    newElement = dom.createElement("CorrectionValue");
    newElement.appendChild(dom.createTextNode(ui->correctionValueLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("CorrectionValue"));

    newElement = dom.createElement("FirstSegmentAngleOnCw");
    newElement.appendChild(dom.createTextNode(ui->firstSegmentLimitAngleOnCWLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("FirstSegmentAngleOnCw"));

    newElement = dom.createElement("FirstSegmentAngleOnCCW");
    newElement.appendChild(dom.createTextNode(ui->firstSegmentLimitAngleOnCCWLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("FirstSegmentAngleOnCCW"));

    newElement = dom.createElement("NumberOfStepsForFirstSegment");
    newElement.appendChild(dom.createTextNode(ui->numberOfStepsForFirstSegmentLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("NumberOfStepsForFirstSegment"));

    newElement = dom.createElement("SecondSegmentAngleOnCw");
    newElement.appendChild(dom.createTextNode(ui->secondSegmentLimitAngleOnCWLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("SecondSegmentAngleOnCw"));

    newElement = dom.createElement("SecondSegmentAngleOnCCW");
    newElement.appendChild(dom.createTextNode(ui->secondSegmentLimitAngleOnCCWLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("SecondSegmentAngleOnCCW"));

    newElement = dom.createElement("NumberOfStepsForSecondSegment");
    newElement.appendChild(dom.createTextNode(ui->numberOfStepsForSecondSegmentLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("NumberOfStepsForSecondSegment"));

    newElement = dom.createElement("FirstSegmentBeginOn");
    if ( ui->firstSegmentBeginOnCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CW"));
    else if ( ui->firstSegmentBeginOnCCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CCW"));
    root.replaceChild(newElement,root.namedItem("FirstSegmentBeginOn"));

    newElement = dom.createElement("SecondSegmentBeginOn");
    if ( ui->secondSegmentBeginOnCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CW"));
    else if ( ui->secondSegmentBeginOnCCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CCW"));
    root.replaceChild(newElement,root.namedItem("SecondSegmentBeginOn"));

    newElement = dom.createElement("DistanceBetweenTwoLimtsOnZ");
    newElement.appendChild(dom.createTextNode(ui->distanceBetweenLimintsOnZLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("DistanceBetweenTwoLimtsOnZ"));

    newElement = dom.createElement("LengthOfThirdSegment");
    newElement.appendChild(dom.createTextNode(ui->lengthOfThirdSegmentLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("LengthOfThirdSegment"));

    newElement = dom.createElement("NumberOfStepsBetweenLimitsOnZ");
    newElement.appendChild(dom.createTextNode(ui->numberOfStepsBetweenLimitsOnZLineEdit->text()));
    root.replaceChild(newElement,root.namedItem("NumberOfStepsBetweenLimitsOnZ"));

    newElement = dom.createElement("IncreasingZOn");
    if ( ui->increasingZonCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CW"));
    else if ( ui->increasingZonCCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CCW"));
    root.replaceChild(newElement,root.namedItem("IncreasingZOn"));

    newElement = dom.createElement("ThirdSegmentBeginOn");
    if ( ui->thirdSegmentBeginOnCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CW"));
    else if ( ui->thirdSegmentBeginOnCCWRadioButton->isChecked() )
        newElement.appendChild(dom.createTextNode("CCW"));
    root.replaceChild(newElement,root.namedItem("ThirdSegmentBeginOn"));
}

void ScaraSC1ConfigWidget::firstSegmentBeginOnCCWClicked()
{
    if ( !ui->firstSegmentBeginOnCCWRadioButton->isChecked() )
        ui->firstSegmentBeginOnCCWRadioButton->setChecked(true);
    else
    {
        ui->firstSegmentBeginOnCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::firstSegmentBeginOnCWClicked()
{
    if ( !ui->firstSegmentBeginOnCWRadioButton->isChecked() )
        ui->firstSegmentBeginOnCWRadioButton->setChecked(true);
    else
    {
        ui->firstSegmentBeginOnCCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::secondSegmentBeginOnCCWClicked()
{
    if ( !ui->secondSegmentBeginOnCCWRadioButton->isChecked() )
        ui->secondSegmentBeginOnCCWRadioButton->setChecked(true);
    else
    {
        ui->secondSegmentBeginOnCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::secondSegmentBeginOnCWClicked()
{
    if ( !ui->secondSegmentBeginOnCWRadioButton->isChecked() )
        ui->secondSegmentBeginOnCWRadioButton->setChecked(true);
    else
    {
        ui->secondSegmentBeginOnCCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::increasingZonCWClicked()
{
    if ( !ui->increasingZonCWRadioButton->isChecked() )
        ui->increasingZonCWRadioButton->setChecked(true);
    else
    {
        ui->increasingZonCCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::increasingZonCCWClicked()
{
    if ( !ui->increasingZonCCWRadioButton->isChecked() )
        ui->increasingZonCCWRadioButton->setChecked(true);
    else
    {
        ui->increasingZonCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::thirdSegmentBeginOnCWClicked()
{
    if ( !ui->thirdSegmentBeginOnCWRadioButton->isChecked() )
        ui->thirdSegmentBeginOnCWRadioButton->setChecked(true);
    else
    {
        ui->thirdSegmentBeginOnCCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}

void ScaraSC1ConfigWidget::thirdSegmentBeginOnCCWClicked()
{
    if ( !ui->thirdSegmentBeginOnCCWRadioButton->isChecked() )
        ui->thirdSegmentBeginOnCCWRadioButton->setChecked(true);
    else
    {
        ui->thirdSegmentBeginOnCWRadioButton->setChecked(false);

        emit contentChanged();
    }
}




























