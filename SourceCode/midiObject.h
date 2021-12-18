#ifndef MIDIOBJECT_H
#define MIDIOBJECT_H

#include <QObject>

class midiObject : public QObject
{
    Q_OBJECT
public:
    explicit midiObject(QObject *parent = nullptr);
    ~midiObject();

signals:
    void midiSend(int, int, int);
    void statusSignal(QString,int);
    void portList(QList<QString>);

public slots:
    void midiPick(int);
    void midiReceive(void);
    void doneCheck();
private:
    void portCheck(int);

};

#endif // MIDIOBJECT_H
