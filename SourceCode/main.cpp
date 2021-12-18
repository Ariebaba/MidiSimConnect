#include "mainwindow.h"
#include "dialogassign.h"

#include "fileHandler.h"
#include "midiObject.h"
#include "flightSim.h"

#include <QApplication>
#include <QThread>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path("config/");
    QFile styleSheetFile(QCoreApplication::applicationDirPath()+"/"+path+QString("Adaptic.qss"));
    //QFile styleSheetFile("Adaptic.qss");
    if(!styleSheetFile.open(QFile::ReadOnly))
    {
        qInfo() << "could not open config file." << styleSheetFile.errorString();

    }

    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    DialogAssign d;
    d.setModal(true);

    fileHandler f;
    midiObject m;
    flightSim s;

    QObject::connect(&w,&MainWindow::openAssignWindow,&d,&DialogAssign::show);
    QObject::connect(&w,&MainWindow::currentMidi,&d,&DialogAssign::midiSlot);
    QObject::connect(&d,&DialogAssign::fillChecker,&d,&DialogAssign::assignSlot);
    QObject::connect(&d,&DialogAssign::commandChange,&f,&fileHandler::assignReceive);
    QObject::connect(&d,&DialogAssign::statusSignal,&w,&MainWindow::statusBar);

    QThread fileThread;
    QObject::connect(&fileThread, &QThread::started,&f,&fileHandler::configInit);
    QObject::connect(&f, &fileHandler::configAmount,&w,&MainWindow::configList);
    QObject::connect(&f,&fileHandler::statusSignal,&w,&MainWindow::statusBar);
    QObject::connect(&f,&fileHandler::commandSend,&w,&MainWindow::simSlot);
    QObject::connect(&m,&midiObject::midiSend,&f,&fileHandler::midiReceive);
    QObject::connect(&w,&MainWindow::midiPick,&f,&fileHandler::midiChange);
    QObject::connect(&w,&MainWindow::configPick,&f,&fileHandler::readPicked);
    QObject::connect(&w,&MainWindow::sendUICommand,&f,&fileHandler::readUICommand);
    QObject::connect(&w,&MainWindow::stopThreads,&fileThread,&QThread::quit);
    f.moveToThread(&fileThread);
    fileThread.start();

    QThread midiThread;
    QObject::connect(&midiThread, &QThread::started,&m,&midiObject::midiReceive);
    QObject::connect(&m,&midiObject::statusSignal, &w, &MainWindow::statusBar);
    QObject::connect(&m,&midiObject::midiSend, &w, &MainWindow::midiSlot);
    QObject::connect(&m,&midiObject::portList, &w, &MainWindow::portList);
    QObject::connect(&w,&MainWindow::stopThreads, &midiThread, &QThread::quit);
    QObject::connect(&w,&MainWindow::midiClose, &m, &midiObject::doneCheck,Qt::DirectConnection);
    QObject::connect(&w,&MainWindow::midiPick, &m, &midiObject::midiPick);
    QObject::connect(&w,&MainWindow::midiConnect,&m,&midiObject::midiReceive);
    QObject::connect(&f,&fileHandler::midiPick, &m, &midiObject::midiPick);
    m.moveToThread(&midiThread);
    midiThread.start();

    QThread simThread;
    QObject::connect(&simThread, &QThread::started,&s,&flightSim::initSim);
    QObject::connect(&s,&flightSim::statusSignal, &w, &MainWindow::statusBar);
    QObject::connect(&f,&fileHandler::commandSend, &s, &flightSim::commandReceived);
    QObject::connect(&w,&MainWindow::simClose, &s, &flightSim::quitCheck,Qt::DirectConnection);
    QObject::connect(&w,&MainWindow::simConnect,&s,&flightSim::initSim);
    QObject::connect(&w,&MainWindow::stopThreads, &simThread, &QThread::quit);
    s.moveToThread(&simThread);
    simThread.start();

    w.show();
    return a.exec();
}
