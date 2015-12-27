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
    ui->lengthOfThirdSegmentLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->correctionValueLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->firstSegmentLimitAngleOnCWLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->firstSegmentLimitAngleOnCCWLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->secondSegmentLimitAngleOnCWLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->secondSegmentLimitAngleOnCCWLineEdit->setValidator(new QDoubleValidator(30,999999,10,this));
    ui->numberOfStepsForFirstSegmentLineEdit->setValidator(new QIntValidator(0,999999,this));
    ui->numberOfStepsForSecondSegmentLineEdit->setValidator(new QIntValidator(0,999999,this));
    ui->numberOfStepsBetweenLimitsOnZLineEdit->setValidator(new QIntValidator(0,999999,this));
    ui->syncFreqLineEdit->setValidator(new QIntValidator(1,999999,this));

    QGraphicsScene* sceneXY = new QGraphicsScene(this);
    QGraphicsScene* sceneZ = new QGraphicsScene(this);

    sceneXY->addPixmap(QPixmap(":/new/icons/scaraXYSurface.png"));
    sceneZ->addPixmap(QPixmap(":/new/icons/scaraZSurface.png"));

    ui->scaraXYSurfaceGraphicsView->setScene(sceneXY);
    ui->scaraZSurfaceGraphicsView->setScene(sceneZ);
}

ScaraSC1ConfigWidget::~ScaraSC1ConfigWidget()
{
    delete ui;
}
