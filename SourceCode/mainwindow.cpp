#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "midiObject.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::midiSlot(int byte0, int byte1, int byte2)
{
    ui->lineEdit_0->setText(QString::number(byte0));
    ui->lineEdit_1->setText(QString::number(byte1));
    ui->lineEdit_2->setText(QString::number(byte2));    
}

void MainWindow::simSlot(int commandID, QString command, QString type, bool found )
{
    if(found)
    {
        ui->lineEdit_simCommand->setText(command);
        if(type=="_btn")
        {
            ui->label_btnType->setText("key");
        }
        if(type=="_rel")
        {
            ui->label_btnType->setText("encoder");
        }
    }
    else
    {
        ui->lineEdit_simCommand->setText("");
        ui->label_btnType->setText("");
    }
}

void MainWindow::statusBar(QString statusMessage, int statusPos)
{
    switch (statusPos)
    {
    case 1:
        ui->label_status->setText(statusMessage);

        break;
    case 2:
        ui->label_midiStatus->setText(statusMessage);

        break;
    case 3:
        ui->label_simStatus->setText(statusMessage);

        break;
    }
     ui->statusbar->addPermanentWidget(ui->label_status,2);
     ui->statusbar->addPermanentWidget(ui->label_midiStatus,1);
     ui->statusbar->addPermanentWidget(ui->label_simStatus,1);
}

void MainWindow::on_pushButton_stopMidi_clicked()
{
    if(ui->label_midiStatus->text()=="Midi OK"){
        emit midiClose();
        ui->pushButton_stopMidi->setText("Re-Connect");
    }
   else
    {
        emit midiConnect();
        ui->pushButton_stopMidi->setText("Stop Midi");
    }
}

void MainWindow::on_pushButton_assign_clicked()
{
    emit openAssignWindow();
    QString byte0 = ui->lineEdit_0->text();
    QString byte1 = ui->lineEdit_1->text();
    QString byte2 = ui->lineEdit_2->text();
    QString type = ui->label_btnType->text();
    QString command = ui->lineEdit_simCommand->text();
    emit currentMidi(byte0,byte1,byte2,type,command);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit midiClose();
    emit simClose();
    QThread::currentThread()->msleep(10);
    emit stopThreads();
}

void MainWindow::configList(QList<QString> configList)
{
    ui->comboBox_config->clear();
    QList<QString>::iterator i;
    for (i = configList.begin(); i != configList.end()-1; ++i)
    {
        QString withExtention=*i;
        QString withoutExtention=withExtention.left(withExtention.lastIndexOf(QChar('.')));
        ui->comboBox_config->addItem(withoutExtention);
    }
    ui->comboBox_config->setCurrentIndex(configList.back().toInt());
}

void MainWindow::on_comboBox_config_activated(int index)
{
    qInfo()<< "Changed to " << index << ui->comboBox_config->currentText();
    emit configPick(ui->comboBox_config->currentText()+".csv");
}

void MainWindow::portList(QList<QString> portList)
{
    ui->comboBox_midi->clear();
    QList<QString>::iterator i;
    for (i = portList.begin(); i != portList.end(); ++i)
    {
        QString portItem = *i;
        ui->comboBox_midi->addItem(portItem);
    }
}

void MainWindow::on_comboBox_midi_activated(int index)
{
    //emit midiClose();
    emit midiPick(index);
    //emit midiConnect();
}

void MainWindow::midiPicked(int index)
{
    ui->comboBox_midi->setCurrentIndex(index);
}

void MainWindow::prepCommand(QString command)
{
    QString desiredText = ui->lineEdit_desired->text();
    if(desiredText!="" ||command=="delete")
    {
        emit sendUICommand(command,ui->comboBox_config->currentText()+".csv",desiredText+".csv");
        ui->lineEdit_desired->setText("");
    }
    else
    {
        statusBar("Enter desired name first",1);
    }
}

void MainWindow::on_pushButton_new_clicked()
{
   prepCommand("new");
}

void MainWindow::on_pushButton_copy_clicked()
{
    prepCommand("copy");
}

void MainWindow::on_pushButton_rename_clicked()
{
    prepCommand("rename");
}

void MainWindow::on_pushButton_delete_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete current file?","Are you sure?");
    if(reply == QMessageBox::Yes)
    {
        prepCommand("delete");
    }
}

void MainWindow::on_pushButton_stopSim_clicked()
{
    if(ui->label_simStatus->text()=="Sim Connection OK"){
        emit simClose();
        ui->pushButton_stopSim->setText("Re-Connect");
    }
   else
    {
        emit simConnect();
        ui->pushButton_stopSim->setText("Stop Connection");
    }
}

