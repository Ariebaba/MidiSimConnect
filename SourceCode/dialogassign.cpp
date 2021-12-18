#include "dialogassign.h"
#include "ui_dialogassign.h"
#include <QObject>
#include <QMessageBox>

bool midi = false;
bool button = false;
bool msfs = false;

DialogAssign::DialogAssign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAssign)
{
    ui->setupUi(this);
    ui->btn_Assign->setDisabled(true);
    ui->btn_Delete->setDisabled(true);

}

DialogAssign::~DialogAssign()
{
    delete ui;
}

void DialogAssign::midiSlot(QString byte0, QString byte1, QString byte2,QString type,QString command)
{
    ui->lineEdit_0d->setText(byte0);
    ui->lineEdit_1d->setText(byte1);
    ui->lineEdit_2d->setText(byte2);
    ui->lineEdit_code->setText(command);
    if(type=="key")
    {
        ui->radioButton_btn->setChecked(true);
    }
    if(type=="encoder")
    {
        ui->radioButton_rel->setChecked(true);
    }
    if(ui->lineEdit_0d->text()!=""||ui->lineEdit_1d->text()!=""||ui->lineEdit_2d->text()!="")
    {
        emit fillChecker("midi");
        emit fillChecker("button");
        emit fillChecker("msfs");
    }

}

void DialogAssign::assignSlot(QString group)
{
    if(group=="midi")
    {
        midi = true;
    }
    if(group=="button")
    {
        button = true;
    }
    if(group=="msfs")
    {
        msfs = true;
    }
    if(group=="reset")
    {        
        midi = false;
        button = false;
        msfs = false;
        ui->lineEdit_0d->setText("");
        ui->lineEdit_1d->setText("");
        ui->lineEdit_2d->setText("");
        ui->lineEdit_code->setText("");
        ui->btn_Assign->setDisabled(true);
        ui->btn_Delete->setDisabled(true);
        ui->radioButton_btn->setAutoExclusive(false);
        ui->radioButton_btn->setChecked(false);
        ui->radioButton_btn->setAutoExclusive(true);
        ui->radioButton_btn->setAutoExclusive(false);
        ui->radioButton_rel->setChecked(false);
        ui->radioButton_btn->setAutoExclusive(true);
    }
    if(midi&&button&&msfs)
    {
        ui->btn_Assign->setDisabled(false);
        ui->btn_Delete->setDisabled(false);
    }    
}

void DialogAssign::prepCommand(QString command)
{
    QString value = ui->lineEdit_code->text();
    if(value!="")
    {
        if(ui->radioButton_btn->isChecked())
        {
            QString id = ui->lineEdit_0d->text()+ui->lineEdit_1d->text()+"_btn";
            emit commandChange(id,value,command);
        }
        if(ui->radioButton_rel->isChecked())
        {
            QString id = ui->lineEdit_0d->text()+ui->lineEdit_1d->text()+ui->lineEdit_2d->text()+"_rel";
            emit commandChange(id,value,command);
        }
    }
    else
    {
        emit statusSignal("No command entered",1);
    }
}

void DialogAssign::on_btn_AssignCancel_clicked()
{
    emit fillChecker("reset");
    accept();
}

void DialogAssign::on_btn_Assign_clicked()
{
    prepCommand("Assign");
    emit fillChecker("reset");
    accept();
}


void DialogAssign::on_btn_Delete_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete this link?","Are you sure?");
    if(reply == QMessageBox::Yes)
    {
        prepCommand("Delete");
    }
    emit fillChecker("reset");
    accept();
}

void DialogAssign::on_radioButton_btn_toggled(bool checked)
{
    if(checked)
    {
        emit fillChecker("button");
    }
}

void DialogAssign::on_radioButton_rel_toggled(bool checked)
{
    if(checked)
    {
        emit fillChecker("button");
    }
}

void DialogAssign::on_lineEdit_code_textChanged(const QString &arg1)
{
    if(arg1!="")
    {
        emit fillChecker("msfs");
    }
}

