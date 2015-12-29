#include "scarasc1configwidget.h"
#include "ui_scarasc1configwidget.h"

#include "styles.h"

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

    ui->lengthOfFirstSegmentLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->lengthOfSecondSegmentLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->lengthOfThirdSegmentLineEdit->setValidator(new QDoubleValidator(31,999999,10,this));
    ui->correctionValueLineEdit->setValidator(new QDoubleValidator(0,999999,10,this));
    ui->firstSegmentLimitAngleOnCWLineEdit->setValidator(new QDoubleValidator(-999999,999999,10,this));
    ui->firstSegmentLimitAngleOnCCWLineEdit->setValidator(new QDoubleValidator(-999999,999999,10,this));
    ui->secondSegmentLimitAngleOnCWLineEdit->setValidator(new QDoubleValidator(-999999,999999,10,this));
    ui->secondSegmentLimitAngleOnCCWLineEdit->setValidator(new QDoubleValidator(-999999,999999,10,this));
    ui->numberOfStepsForFirstSegmentLineEdit->setValidator(new QIntValidator(0,999999,this));
    ui->numberOfStepsForSecondSegmentLineEdit->setValidator(new QIntValidator(0,999999,this));
    ui->numberOfStepsBetweenLimitsOnZLineEdit->setValidator(new QIntValidator(30,999999,this));
    ui->syncFreqLineEdit->setValidator(new QIntValidator(1,999999,this));

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
}

ScaraSC1ConfigWidget::~ScaraSC1ConfigWidget()
{
    delete ui;
}

bool ScaraSC1ConfigWidget::populateFromDomElement(QDomElement const& root)
{
    QDomElement element;

    element = root.namedItem("Files").toElement();
    if ( !element.isNull() )
        for ( QDomElement file = element.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File") )
        {
            QDomElement name = file.namedItem("Name").toElement();

            if ( !name.isNull() )
            {
                if ( name.text().endsWith(".py") )
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

void ScaraSC1ConfigWidget::firstSegmentBeginOnCCWClicked()
{
    ui->firstSegmentBeginOnCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::firstSegmentBeginOnCWClicked()
{
    ui->firstSegmentBeginOnCCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::secondSegmentBeginOnCCWClicked()
{
    ui->secondSegmentBeginOnCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::secondSegmentBeginOnCWClicked()
{
    ui->secondSegmentBeginOnCCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::increasingZonCWClicked()
{
    ui->increasingZonCCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::increasingZonCCWClicked()
{
    ui->increasingZonCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::thirdSegmentBeginOnCWClicked()
{
    ui->thirdSegmentBeginOnCCWRadioButton->setChecked(false);
}

void ScaraSC1ConfigWidget::thirdSegmentBeginOnCCWClicked()
{
    ui->thirdSegmentBeginOnCWRadioButton->setChecked(false);
}











