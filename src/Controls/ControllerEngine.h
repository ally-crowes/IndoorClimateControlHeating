#ifndef __CONTROL_ENGINE_H__
#define __CONTROL_ENGINE_H__
#include "SensorTypeNTC.h"
#include "Relay.h"

enum ModeAction // heat, cool, range in/out
{
    Heat,
    Coll,
    RangeMatch,
    //RangeOut,
};

enum ModeSwitchingDevice // Switching relays: First/Second Only, TicTac, Parallel/Serial
{
    FirstOnly,
    SecondOnly,
    TicTac,
    Parallel, // connected in parallel
    //Serial, // connected in series
};

struct CONTROL_FLAGS // NOT USED - for moved to create example
{
    public:
        int relayA:1;
        int relayB:1;
        int sensorWaitRangeEngine:1;
        int sensorWaitRangeClimat:1;
        int reserved:4;
//}control_flag;             // multiple definition of `control_flag'
};

//CONTROL_FLAGS control_flag; // multiple definition of `control_flag'

class ControllerEngine
{
private:
    SensorTypeNTC *sensorEngine;
    Relay *relayA;
    Relay *relayB;
    ModeAction action = ModeAction::Heat;
    ModeSwitchingDevice switching = ModeSwitchingDevice::FirstOnly;
    unsigned int count = 0;
    bool flagRelay = false;
    bool flagDriversWait = false;

    void RelaysOn();
    void RelaysOff();

public:
    ControllerEngine(SensorTypeNTC *sensor, Relay *relayA, Relay *relayB, ModeAction action, ModeSwitchingDevice switching);
    ControllerEngine(SensorTypeNTC *sensor, Relay *relayA, Relay *relayB);
    ControllerEngine(SensorTypeNTC *sensor, Relay *relay, ModeAction action);
    
    ModeAction GetAction() const;
    void SetAction(ModeAction action);
    ModeSwitchingDevice GetSwitchingDevice() const;
    void SetSwitchingDevice(ModeSwitchingDevice switching);
    
    float GetTemperature();
    void Update();
    void Wait();     // чекати
    // void Continue(); // продовжити
    unsigned int GetCount();
    bool GetConditionRelay(int n = 1);
    bool GetCondition();
    
    float GetMinTemperature();
    void SetMinTemperature(float min);
    float GetMaxTemperature();
    void SetMaxTemperature(float max);
};

#endif