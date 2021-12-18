#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

class fileHandler : public QObject
{
    Q_OBJECT
public:
    explicit fileHandler(QObject *parent = nullptr);
    ~fileHandler();
    QList<QString> fileCounter(QString);
    void configInit();
    void makeFile(QString);
    QMap<QString,QString> readFile(QString);
    void writeFile(QString, QMap<QString,QString>);

signals:
    void configAmount(QList<QString>);
    void statusSignal(QString,int);
    void commandSend(int,QString,QString,bool);
    void commandReset();
    void midiPick(int);

public slots:
    void readPicked(QString);
    void readUICommand(QString,QString,QString);
    void midiReceive(int,int,int);
    void assignReceive(QString,QString,QString);
    void midiChange(int);

private:
    void checkDir();
    bool containCheck(QString);
    void newFile(QString);
    void copyFile(QString,QString);
    void renameFile(QString,QString);
    void deleteFile(QString);

};


#endif // FILEHANDLER_H
