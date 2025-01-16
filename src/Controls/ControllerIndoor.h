#ifndef __CONTROL_CLIMATE_H__
#define __CONTROL_CLIMATE_H__
#include "ControllerEngine.h"

class ControllerIndoor
{
private:
    SensorTypeNTC* sensorInternal;
    Relay* pump;
    ControllerEngine* controlEngine;
    bool flagDriversWait = false;
    bool flagPumpWait = false;
    unsigned long delayPumpOff = 10000;   // Затримка перед вимкненям насосу після повного нагріву кімнат (10 сек)
    unsigned long delayPumpOn = 3000;   // Затримка увимкненям нагрівачів після увімкниням насосу (3 сек)

public:
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine);
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, Relay* pump);

    float GetTemperature();
    void Update();
    bool GetCondition();
    bool GetConditionPump();
    void SetConditionPump(bool value);
    float GetMinTemperature();
    void SetMinTemperature(float min);
    float GetMaxTemperature();
    void SetMaxTemperature(float max);
    unsigned long GetDelayPumpOff();
    void SetDelayPumpOff(unsigned long time);
    unsigned long GetDelayPumpOn();
    void SetDelayPumpOn(unsigned long time);
};

#endif