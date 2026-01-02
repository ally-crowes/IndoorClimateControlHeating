#ifndef __CONTROL_ENGINE_H__
#define __CONTROL_ENGINE_H__

// #include "\lib\T-Sensor\TypeNTC_Thermistor\tSensorTypeNTC.h"
#include <tSensorTypeNTC.h>
#include "relayDevice.h"

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
    SensorTypeNTC *sensorEngine = NULL;
    RelayDevice *relayA = NULL;
    RelayDevice *relayB = NULL;
    ModeAction action = ModeAction::Heat;
    ModeSwitchingDevice switching = ModeSwitchingDevice::FirstOnly;
    unsigned int count = 0;
    bool flagRelay = false;
    bool flagDriversWait = false;

    void relaysOn();
    void relaysOff();

protected:
    float* eeprom_minTemperature = NULL;
    float* eeprom_maxTemperature = NULL;

public:
    //ControllerEngine(SensorTypeNTC *sensor, Relay *relayA, Relay *relayB, ModeAction action, ModeSwitchingDevice switching, float* eeprom_minTemperature, float* eeprom_maxTemperature);
    ControllerEngine(SensorTypeNTC *sensor, RelayDevice *relayA, RelayDevice *relayB, float* eeprom_minTemperature, float* eeprom_maxTemperature);
    /* TODO  
    // ControllerEngine(SensorTypeNTC *sensor, Relay *relayA, Relay *relayB);
    // ControllerEngine(SensorTypeNTC *sensor, Relay *relay, ModeAction action);
    */

    ModeAction getAction() const;
    void setAction(ModeAction action);
    ModeSwitchingDevice getSwitchingDevice() const;
    void setSwitchingDevice(ModeSwitchingDevice switching);
    
    float getTemperature();
    void update();
    void wait();     // чекати
    // void Continue(); // продовжити
    unsigned int getCount();
    bool getConditionRelay(int n = 1);
    bool getCondition();
    
    float getMinTemperature();
    void setMinTemperature(float min);
    float getMaxTemperature();
    void setMaxTemperature(float max);
    void changeMinUpTemperature(float step = 0.1);
    void changeMinDownTemperature(float step = 0.1);
    void changeMaxUpTemperature(float step = 0.1);
    void changeMaxDownTemperature(float step = 0.1);

};

#endif