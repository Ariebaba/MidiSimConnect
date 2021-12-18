#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogassign.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void prepCommand(QString);
signals:
    void midiClose();
    void simClose();
    void midiConnect();
    void simConnect();
    void stopThreads();
    void configPick(QString);
    void midiPick(int);
    void sendUICommand(QString,QString,QString);
    void openAssignWindow();
    void currentMidi(QString,QString,QString,QString,QString);

public slots:
    void midiSlot(int, int, int);
    void simSlot(int, QString, QString,bool);
    void statusBar(QString, int);
    void configList(QList<QString>);
    void portList(QList<QString>);
    void midiPicked(int);

private slots:
    void on_pushButton_stopMidi_clicked();
    void on_comboBox_config_activated(int index);
    void on_pushButton_new_clicked();
    void on_pushButton_copy_clicked();
    void on_pushButton_rename_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_assign_clicked();

    void on_comboBox_midi_activated(int index);

    void on_pushButton_stopSim_clicked();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent* event);

};
#endif // MAINWINDOW_H
