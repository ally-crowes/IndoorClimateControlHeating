#ifndef __CONTROL_CLIMATE_H__
#define __CONTROL_CLIMATE_H__
#include "ControllerEngine.h"

class ControllerIndoor
{
private:
    SensorTypeNTC* sensorInternal;
    ControllerEngine* controlEngine;
    Relay* pump;
    bool flagDriversWait = false;
    bool flagPumpWait = false;
    unsigned long delayPumpOff = 10000;   // Затримка перед вимкненям насосу після повного нагріву кімнат (10 сек)
    unsigned long delayPumpOn = 3000;   // Затримка увимкненям нагрівачів після увімкниням насосу (3 сек)
    
protected:
    float* eeprom_minTemperature;
    float* eeprom_maxTemperature;

public:
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine);
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, Relay* pump);
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, Relay* pump, float* eeprom_minTemperature, float* eeprom_maxTemperature);

    float GetTemperature();
    void Update();
    bool GetCondition();
    float GetMinTemperature();
    void SetMinTemperature(float min);
    float GetMaxTemperature();
    void SetMaxTemperature(float max);
    bool GetConditionPump();
    void ChangeMinUpTemperature(float step = 0.1);
    void ChangeMinDownTemperature(float step = 0.1);
    void ChangeMaxUpTemperature(float step = 0.1);
    void ChangeMaxDownTemperature(float step = 0.1);
    void SetConditionPump(bool value);
    unsigned long GetDelayPumpOff();
    void SetDelayPumpOff(unsigned long time);
    unsigned long GetDelayPumpOn();
    void SetDelayPumpOn(unsigned long time);
};

#endif