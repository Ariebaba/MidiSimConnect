#include "midiObject.h"
#include "mainwindow.h"
#include "RtMidi.h"
#include <QObject>
#include <QThread>
#include <QDebug>

int portIndex;
midiObject::midiObject(QObject *parent) : QObject(parent)
{
    //
}

midiObject::~midiObject()
{
    //
}

bool done;
void midiObject::doneCheck()
{
    done= true;
}

void midiObject::midiPick(int index)
{
    portIndex=index;
    qInfo()<<"midi port"<<index;
}

void midiObject::portCheck(int index)
{
    if(index!= portIndex)
    {
        qInfo()<<"port changed";
        done=true;
    }
}
void midiObject::midiReceive()
{

    RtMidiIn *midiin = new RtMidiIn();
    std::vector<unsigned char> message;
    int nBytes, i;
    double stamp;

    // Check available ports.
    QList<QString> midiPorts;
    std::string portName;
    int nPorts = midiin->getPortCount();
    if ( nPorts == 0 ) {       
        emit statusSignal("No ports available!",1);
        emit statusSignal("No Midi",2);
        goto cleanup;
    }    
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );    
    for (int i=0; i<nPorts; i++ ) {
      try {
        midiPorts.append(QString::fromStdString(midiin->getPortName(i)));
       // qInfo()<<QString::fromStdString(midiin->getPortName(i))<<midiPorts;
      }
      catch ( RtMidiError &error ) {
        emit statusSignal("Something went wrong with MIDI",1);
        goto cleanup;
      }
    }
    emit portList(midiPorts);

    midiin->openPort( portIndex );
    emit statusSignal("Reading Midi from port",1);
    emit statusSignal("Midi OK",2);
    done = false;
    int byte0;
    int byte1;
    int byte2;
    while ( !done )
    {
        stamp = midiin->getMessage( &message );
        nBytes = message.size();

        for ( i=0; i<nBytes; i++ )
        {
            //std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
            byte0 =(int)message[0];
            byte1 =(int)message[1];
            byte2 =(int)message[2];
            if(i==2)
            {
                emit midiSend(byte0,byte1,byte2);
            }
        }

        if ( nBytes > 0 )
        {
           // std::cout << "stamp = " << stamp << std::endl;
        }
        portCheck(portIndex);
        //qInfo()<<done;
        QThread::currentThread()->msleep(10);

    }
    // Clean up
    emit statusSignal("Ending Midi Connection",1);
    emit statusSignal("Midi Stopped",2);
    cleanup:
    delete midiin;
}
