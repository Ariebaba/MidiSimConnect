#include "assigndialog.h"
#include "ui_assigndialog.h"

assignDialog::assignDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::assignDialog)
{
    ui->setupUi(this);
}

assignDialog::~assignDialog()
{
    delete ui;
}
