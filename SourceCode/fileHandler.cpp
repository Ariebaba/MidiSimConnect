#include "fileHandler.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QDirIterator>
#include <QStringList>
#include <QMap>

QMap<QString,QString> activeConfig;
QString activeConfigName;

fileHandler::fileHandler(QObject *parent) : QObject(parent)
{
    checkDir();
}

fileHandler::~fileHandler()
{
    qInfo()<< "Closing";
}

// Checks to see if any config and ini file is present. If not.. makes one of each.
void fileHandler::checkDir()
{
    QDir dir(QCoreApplication::applicationDirPath());
    QString path("config/");
    if (!dir.exists(path))
    {
        if(dir.mkpath(path))
        {
            emit statusSignal("Config folder created",1);
        }
        else
        {
            emit statusSignal("Something went wrong",1);
        }

    }
    if (fileCounter("*.csv").length() == 0)
    {
        makeFile("config01.csv");        
    }
    if (fileCounter("*.ini").length() == 0)
    {
        makeFile("program.ini");
        QMap<QString,QString> programIni =readFile("program.ini");
        programIni.insert("lastConfig","config01.csv");
        programIni.insert("lastMidi","0");
        qInfo()<< programIni;
        writeFile("program.ini",programIni);
    }
}

// Checks the amount of files of type in the config folder. This prog only checks config files.

QList<QString> fileHandler::fileCounter(QString fileType)
{
    int total_files = 0;
    QList<QString> fileList;
    QString dirCounter = QCoreApplication::applicationDirPath() + "/config/";
    QDirIterator iter(dirCounter, QStringList() << fileType, QDir::Files);
    while (iter.hasNext())
    {
        iter.next();
        total_files++;
        fileList.append(iter.fileName());
    }

    return fileList;
    fileList.clear();
}

//After initializing fileHandler, the flow continues here called from main.
//This function reads the program.ini file to obtain the last used config
//and sends all the available config files to the main window combobox.

void fileHandler::configInit()
{ 
    // look into dubble load at copy and delete
    QMap<QString,QString> programIni =readFile("program.ini");
    QString lastConfig = programIni.value("lastConfig");
    int lastMidi = programIni.value("lastMidi").toInt();
    QStringList configFiles=fileCounter("*.csv");
    for(int i =0; i<configFiles.length();i++)
    {
        if (QString(configFiles[i])==lastConfig)
        {
            configFiles.append(QString::number(i));
        }
    }
    activeConfig.clear();
    activeConfig = readFile(lastConfig);
    activeConfigName = lastConfig;
    emit statusSignal("Config file list updated",1);
    emit configAmount(configFiles);
    emit midiPick(lastMidi);
}

void fileHandler::makeFile(QString fileName)
{
    QString path("config/");
    QFile newFile(QCoreApplication::applicationDirPath()+"/"+path+QString(fileName));
    //qInfo()<< newFile;
    if(!newFile.open(QIODevice::WriteOnly))
    {
        qInfo() << "could not make new file." << newFile.errorString();
        return;
    }
    else
    {
        emit statusSignal("Something went wrong",1);
    }
    qInfo() << "File: "<< fileName << " made";
    emit statusSignal(fileName +" created",1);
    newFile.close();
}

QMap<QString,QString> fileHandler::readFile(QString fileName)
{
    QString path("config/");
    QFile openFile(QCoreApplication::applicationDirPath()+"/"+path+QString(fileName));
    QMap<QString, QString> map;

    if(!openFile.open(QIODevice::ReadOnly))
    {
        qInfo() << "could not open config file." << openFile.errorString();
        emit statusSignal("Something went wrong",1);
    }
    openFile.seek(0);

    while (!openFile.atEnd())
    {
        QByteArray line = openFile.readLine().trimmed();
        QList section = line.split(',');
        QString key = section[0];
        QString value = section[1];
        map.insert(key,value);
    }
    openFile.close();
    return map;

}

void fileHandler::writeFile(QString fileName, QMap<QString,QString> map)
{
    QString path("config/");
    QFile openFile(QCoreApplication::applicationDirPath()+"/"+path+QString(fileName));       
    if(!openFile.open(QIODevice::WriteOnly))
    {
        qInfo() << "could not open file." << openFile.errorString();
        emit statusSignal("Something went wrong",1);
        return;
    }

    QTextStream stream(&openFile);
    QMap<QString, QString>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        stream << i.key() << "," << i.value() << Qt::endl;
        ++i;
    }
    openFile.close();
}

void fileHandler::readPicked(QString configFile)
{
    activeConfig.clear();
    activeConfig = readFile(configFile);
    activeConfigName = configFile;
    QMap<QString,QString> updateIni = readFile("program.ini");
    updateIni["lastConfig"]=configFile;
    writeFile("program.ini",updateIni);
     qInfo()<<"from picked "<<activeConfig;
}

void fileHandler::readUICommand(QString command,QString oldFile,QString fileName)
{
    if(command=="new")
    {
        newFile(fileName);
    }
    if(command=="copy")
    {
        copyFile(oldFile,fileName);
    }
    if(command=="rename")
    {
        renameFile(oldFile,fileName);
    }
    if(command=="delete")
    {
        deleteFile(oldFile);
    }
}


bool fileHandler::containCheck(QString fileName)
{
    QStringList configFiles=fileCounter("*.csv");
    for(int i =0; i<configFiles.length();i++)
    {
        if (QString(configFiles[i])==fileName)
        {
            return true;
        }
    }
    return false;
}

void fileHandler::newFile(QString fileName)
{
    emit statusSignal("new command received",1);
    if(!containCheck(fileName))
    {
        makeFile(fileName);
        readPicked(fileName);
        configInit();
    }
    else
    {
        emit statusSignal("File already exists",1);
    }
}

void fileHandler::copyFile(QString oldFile,QString fileName)
{
    emit statusSignal("copy command received",1);

    if(!containCheck(fileName))
    {
        QMap<QString,QString> fromFile = readFile(oldFile);
        makeFile(fileName);
        writeFile(fileName,fromFile);
        readPicked(fileName);
        configInit();
    }
    else
    {
        emit statusSignal("File already exists",1);
    }
}

void fileHandler::renameFile(QString oldFile,QString fileName)
{
    emit statusSignal("rename command received",1);
    if(!containCheck(fileName))
    {
        QString path("config/");
        QString newName = QCoreApplication::applicationDirPath()+"/"+path+QString(fileName);
        QString oldName = QCoreApplication::applicationDirPath()+"/"+path+QString(oldFile);

        bool renameCheck = QFile::rename ( oldName, newName );
        if(renameCheck)
        {
            readPicked(fileName);
            configInit();
        }
        else
        {
            emit statusSignal("Something went wrong",1);
        }
    }
    else
    {
        emit statusSignal("File already exists",1);
    }
}

void fileHandler::deleteFile(QString oldFile)
{

    qInfo()<<oldFile;
    QString path("config/");
    QFile file = QCoreApplication::applicationDirPath()+"/"+path+QString(oldFile);
    if(!file.remove())
    {
        emit statusSignal("Something went wrong",1);
    }
    checkDir();
    QStringList configFiles=fileCounter("*.csv");
    QString topOfList = configFiles[0];
    readPicked(topOfList);
    configInit();
    emit statusSignal("File deleted",1);
}

void fileHandler::midiReceive(int byte0, int byte1, int byte2)
{
    QString key = QString::number(byte0)+QString::number(byte1)+"_btn";
    int recID = (QString::number(byte0)+QString::number(byte1)).toInt();
    // to the sim
    bool found = false;
    int lastByte;
    if(activeConfig.contains(key))
    {
        QString value=activeConfig[key];
        emit commandSend(recID,value,"_btn",true);
        found=true;
    }
    if(byte2<64)
    {
        lastByte = 1;
    }else
    {
        lastByte = 127;
    }
    key = QString::number(byte0)+QString::number(byte1)+QString::number(lastByte)+"_rel";
    recID = (QString::number(byte0)+QString::number(byte1)+QString::number(lastByte) ).toInt();
    if(activeConfig.contains(key))
    {
        QString value=activeConfig[key];
        emit commandSend(recID,value,"_rel",true );
        found=true;
    }
    if(!found)
    {
        recID=0;
        QString value="";
        emit commandSend(recID,value,"reset",false );
        qInfo()<<"nothing linked to this midi key";
    }
    found=false;
}

void fileHandler::assignReceive(QString id, QString value, QString command)
{
    if(command=="Assign")
    {
        if(activeConfig.contains(id))
        {
            emit statusSignal("Link already exists",1);
        }
        else
        {
            activeConfig.insert(id,value);
            writeFile(activeConfigName,activeConfig);
            emit statusSignal("Link saved",1);
        }
    }
    if(command=="Delete")
    {
        if(!activeConfig.contains(id))
        {
            emit statusSignal("No such link",1);
        }
        else
        {
            activeConfig.remove(id);
            writeFile(activeConfigName,activeConfig);
            emit statusSignal("Link removed",1);
        }
    }
}

void fileHandler::midiChange(int portPicked)
{
    QMap<QString,QString> updateIni = readFile("program.ini");
    updateIni["lastMidi"]=QString::number(portPicked);
    writeFile("program.ini",updateIni);

}
