#include "pointseditor.h"

#include <QHeaderView>
#include <QAbstractButton>


PointsEditor::PointsEditor(QWidget *parent)
    : QTableWidget(parent)
{
    this->setStyleSheet("border: 0;"
                        "background-color: rgb(43, 43, 43);"
                        "QTableCornerButton::section {border: 2px outset red; background: green;}");

    QStringList labels;

    labels << "Label" << "X" << "Y" << "Z";

    setColumnCount(4);
    setRowCount(1);

    setCornerButtonEnabled(false);

    this->setHorizontalHeaderLabels(labels);
    this->horizontalHeader()->show();

    this->horizontalHeader()->setStyleSheet("QHeaderView::section {"
                "background-color: rgb(61, 56, 54);"
                                            "border: 1px solid rgb(41, 43, 45);}");
    this->verticalHeader()->setStyleSheet("QHeaderView::section {"
                "background-color: rgb(61, 56, 54);"
                                          "border: 1px solid rgb(41, 43, 45);}");

}

