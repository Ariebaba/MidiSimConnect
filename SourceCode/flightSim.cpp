#include "flightSim.h"
#include "mainwindow.h"
#include <iostream>
#include <Windows.h>
#include <WinDef.h>
#include "SimConnect.h"
#include <QMap>
#include <QObject>
#include <QDebug>



bool connected = false;

HANDLE hSimConnect = NULL;

HRESULT hr;

SIMCONNECT_CLIENT_DATA_ID ClientDataID = 1;

// Reference to user aircraft.
SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER;

DOUBLE data = 8;

//const char* FileEventsMobiFlight = "b36config.txt";

//std::vector<std::pair<std::string, std::string>> CodeEvents;

enum GROUP_ID {
    GROUP_A=1,
};

enum EVENT_ID {
    WASM_EVENT,
};

enum DATA_DEFINE_ID {
    DEFINITION_1 = 12,
};

enum DATA_REQUEST_ID {
    REQUEST_1 = 10,    
};

struct SimResponse {
   bool GEAR_HANDLE_POSITION;
};

// useless now until a receiving of variables from simconnect via a callback is implemented
bool quit;
void flightSim::quitCheck()
{
    quit= true;
    SimConnect_Close(hSimConnect);
    connected = false;
    emit statusSignal("Sim Connection Stopped",1);
    emit statusSignal("No Sim Connection",3);
}

void flightSim::SendToSim(int IDadd, std::string idVar)
{
    if(connected){
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, WASM_EVENT+IDadd, _strdup(idVar.c_str()));
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_A, WASM_EVENT + IDadd);
        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_A, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        // Perform callback routine.
        // SimConnect_CallDispatch(hSimConnect, dispatchRoutine, NULL);
        SimConnect_TransmitClientEvent(hSimConnect, objectID, WASM_EVENT + IDadd, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    }
}

void flightSim::initSim()
{
    hr = SUCCEEDED(SimConnect_Open(&hSimConnect, "--- Begin Send Event ---", NULL, 0, 0, 0));
    if (!hr)
    {
        connected = false;
        qInfo()<<"no connection";
        emit statusSignal("No Sim Connection",3);
        return;
    }
    connected = true;
    emit statusSignal("Connected to Sim",1);
    emit statusSignal("Sim Connection OK",3);
    // Map an ID to the Client Data Area.
    hr &= SimConnect_MapClientDataNameToID(hSimConnect, "Autopilot", ClientDataID);
    // Add a double to the data definition.
    hr &= SimConnect_AddToClientDataDefinition(hSimConnect, DEFINITION_1, SIMCONNECT_CLIENTDATAOFFSET_AUTO, sizeof(data));
    // Set up a custom Client Data Area.
    hr &= SimConnect_CreateClientData(hSimConnect, ClientDataID, sizeof(data), SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED);
    // Request the client data periodically.
    hr &= SimConnect_RequestClientData(hSimConnect, ClientDataID, REQUEST_1, DEFINITION_1, SIMCONNECT_CLIENT_DATA_PERIOD_SECOND, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT);
    hr &= SimConnect_SetClientData(hSimConnect, ClientDataID, DEFINITION_1, SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, sizeof(data), &data);   
}

void flightSim::commandReceived(int IDrec,QString command,QString type, bool found)
{
    if(found)
    {
        qInfo()<<command<< " received in simhandler";
        SendToSim(IDrec, command.toStdString());
    }
}

flightSim::flightSim(QObject *parent) : QObject(parent)
{
    //
}
flightSim::~flightSim()
{
    SimConnect_Close(hSimConnect);
}
