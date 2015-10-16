#include "savechanges.h"
#include "ui_savechanges.h"

SaveChanges::SaveChanges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveChanges)
{
    ui->setupUi(this);
}

SaveChanges::~SaveChanges()
{
    delete ui;
}
