#include "pointsfileeditor.h"
#include "ui_pointsfileeditor.h"

#include "styles.h"
#include "lineedit.h"

#include "doublevalidator.h"

#include <QPushButton>
#include <QMessageBox>

PointsFileEditor::PointsFileEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointsFileEditor)
{
    ui->setupUi(this);

    removeRowMapper = new QSignalMapper(this);

    connect(removeRowMapper,SIGNAL(mapped(int)),ui->tableWidget,SLOT(removeRow(int)));
    connect(ui->addPointButton,SIGNAL(clicked(bool)),this,SLOT(addButtonClicked()));

    //setStyleSheet(currentProjectEditorTheme);

    ui->addPointButton->setStyleSheet(currentButtonTheme);
    ui->addPointButton->setMinimumWidth(60);

    ui->xLineEdit->setStyleSheet(currentLineEditTheme);
    ui->yLineEdit->setStyleSheet(currentLineEditTheme);
    ui->zLineEdit->setStyleSheet(currentLineEditTheme);
    ui->labelLineEdit->setStyleSheet(currentLineEditTheme);

    ui->xLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->yLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    ui->zLineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));

    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {"
                                                       "background-color: rgb(61, 56, 54);"
                                                       "border: 1px solid rgb(41, 43, 45);}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {"
                                                     "background-color: rgb(61, 56, 54);"
                                                     "border: 1px solid rgb(41, 43, 45);}");

    ui->tableWidget->setColumnWidth(4,30);

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SIGNAL(contentChanged()));
}

PointsFileEditor::~PointsFileEditor()
{
    delete ui;
}

QString PointsFileEditor::toStr()
{
    QDomDocument dom("NWTR_Robotics");
    QDomElement root = dom.createElement("PointsFile");
    dom.appendChild(root);

    for ( unsigned i=0; i< ui->tableWidget->rowCount(); ++i)
    {
        QDomElement point, value;

        point = dom.createElement("Point");

        value = dom.createElement("Label");
        value.appendChild(dom.createTextNode(((LineEdit*)ui->tableWidget->cellWidget(i,0))->text()));
        point.appendChild(value);

        value = dom.createElement("X");
        value.appendChild(dom.createTextNode(((LineEdit*)ui->tableWidget->cellWidget(i,1))->text()));
        point.appendChild(value);

        value = dom.createElement("Y");
        value.appendChild(dom.createTextNode(((LineEdit*)ui->tableWidget->cellWidget(i,2))->text()));
        point.appendChild(value);

        value = dom.createElement("Z");
        value.appendChild(dom.createTextNode(((LineEdit*)ui->tableWidget->cellWidget(i,3))->text()));
        point.appendChild(value);

        root.appendChild(point);
    }

    return dom.toString();
}

bool PointsFileEditor::populateFromString(const QString &data)
{
    QDomDocument dom;
    QDomElement root;
    QString errorStr;

    if ( !dom.setContent(data,false,&errorStr))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);

        msgBox.setStyleSheet(currentErrorBoxTheme);
        msgBox.exec();

        return false;
    }

    root = dom.documentElement();

    if ( root.tagName() != "PointsFile")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'PointsFile' tag in file."));

        msgBox.setStyleSheet(currentErrorBoxTheme);
        msgBox.exec();

        return false;
    }

    ui->tableWidget->setRowCount(0);

    for ( QDomElement point=root.firstChildElement("Point"); !point.isNull(); point = point.nextSiblingElement("Point"))
    {
        QDomElement label,x,y,z;

        label = point.namedItem("Label").toElement();
        x = point.namedItem("X").toElement();
        y = point.namedItem("Y").toElement();
        z = point.namedItem("0").toElement();

        if ( !label.isNull() )
        {
            QTableWidgetItem* tableWidgetItem;
            LineEdit* lineEdit;
            unsigned row = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);

            tableWidgetItem = new QTableWidgetItem();
            lineEdit = new LineEdit(this);
            ui->tableWidget->setItem(row,0,tableWidgetItem);
            lineEdit->setText(label.text());
            lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
            lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
            ui->tableWidget->setCellWidget(row,0,lineEdit);



            tableWidgetItem = new QTableWidgetItem();
            lineEdit = new LineEdit(this);
            ui->tableWidget->setItem(row,1,tableWidgetItem);

            if ( !x.isNull() )
                lineEdit->setText(x.text());
            else
                lineEdit->setText("0");

            lineEdit->setText(ui->xLineEdit->text());
            lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
            lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
            lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
            ui->tableWidget->setCellWidget(row,1,lineEdit);



            tableWidgetItem = new QTableWidgetItem();
            lineEdit = new LineEdit(this);
            ui->tableWidget->setItem(row,2,tableWidgetItem);

            if ( !y.isNull() )
                lineEdit->setText(y.text());
            else
                lineEdit->setText("0");

            lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
            lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
            lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
            ui->tableWidget->setCellWidget(row,2,lineEdit);



            tableWidgetItem = new QTableWidgetItem();
            lineEdit = new LineEdit(this);
            ui->tableWidget->setItem(row,3,tableWidgetItem);

            if ( !z.isNull() )
                lineEdit->setText(z.text());
            else
                lineEdit->setText("0");

            lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
            lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
            lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
            ui->tableWidget->setCellWidget(row,3,lineEdit);



            tableWidgetItem = new QTableWidgetItem();
            QPushButton* pushButton = new QPushButton(QIcon(":/new/icons/lc_quit.png"),"",this);
            removeRowMapper->setMapping(pushButton,row);
            connect(pushButton,SIGNAL(clicked(bool)),removeRowMapper,SLOT(map()));
            connect(pushButton,SIGNAL(clicked(bool)),this,SIGNAL(contentChanged()));
            ui->tableWidget->setItem(row,4,tableWidgetItem);
            ui->tableWidget->setCellWidget(row,4,pushButton);
        }
    }

    return false;
}

void PointsFileEditor::addButtonClicked()
{
    if ( ui->labelLineEdit->text().isEmpty() ||
         ui->xLineEdit->text().isEmpty() ||
         ui->yLineEdit->text().isEmpty() ||
         ui->zLineEdit->text().isEmpty() )
        return;

    QTableWidgetItem* tableWidgetItem;
    LineEdit* lineEdit;
    unsigned row = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(row);

    tableWidgetItem = new QTableWidgetItem();
    lineEdit = new LineEdit(this);
    ui->tableWidget->setItem(row,0,tableWidgetItem);
    lineEdit->setText(ui->labelLineEdit->text());
    lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    ui->tableWidget->setCellWidget(row,0,lineEdit);

    tableWidgetItem = new QTableWidgetItem();
    lineEdit = new LineEdit(this);
    ui->tableWidget->setItem(row,1,tableWidgetItem);
    lineEdit->setText(ui->xLineEdit->text());
    lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    ui->tableWidget->setCellWidget(row,1,lineEdit);

    tableWidgetItem = new QTableWidgetItem();
    lineEdit = new LineEdit(this);
    ui->tableWidget->setItem(row,2,tableWidgetItem);
    lineEdit->setText(ui->yLineEdit->text());
    lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    ui->tableWidget->setCellWidget(row,2,lineEdit);

    tableWidgetItem = new QTableWidgetItem();
    lineEdit = new LineEdit(this);
    ui->tableWidget->setItem(row,3,tableWidgetItem);
    lineEdit->setText(ui->zLineEdit->text());
    lineEdit->setValidator(new DoubleValidator(-999999,999999,10,this));
    lineEdit->setAlignment(Qt::AlignHCenter	| Qt::AlignVCenter);
    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(contentChanged()));
    ui->tableWidget->setCellWidget(row,3,lineEdit);

    tableWidgetItem = new QTableWidgetItem();
    QPushButton* pushButton = new QPushButton(QIcon(":/new/icons/lc_quit.png"),"",this);
    removeRowMapper->setMapping(pushButton,row);
    connect(pushButton,SIGNAL(clicked(bool)),removeRowMapper,SLOT(map()));
    connect(pushButton,SIGNAL(clicked(bool)),this,SIGNAL(contentChanged()));
    ui->tableWidget->setItem(row,4,tableWidgetItem);
    ui->tableWidget->setCellWidget(row,4,pushButton);

    ui->labelLineEdit->clear();
    ui->xLineEdit->setText("0");
    ui->yLineEdit->setText("0");
    ui->zLineEdit->setText("0");
}





















