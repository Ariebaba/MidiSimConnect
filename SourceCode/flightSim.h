#ifndef FLIGHTSIM_H
#define FLIGHTSIM_H

#include <QObject>

class flightSim : public QObject
{
    Q_OBJECT

public:
    explicit flightSim(QObject *parent = nullptr);
    ~flightSim();

signals:
    //void simSend(QString);
    void statusSignal(QString,int);

public slots:
    void initSim(void);
    void commandReceived(int,QString,QString,bool);
    void quitCheck();

private:
    void simReceive();
    void SendToSim(int, std::string);

};

#endif // FLIGHTSIM_H
